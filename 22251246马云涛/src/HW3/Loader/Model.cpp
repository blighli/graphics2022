#include <glm/gtx/quaternion.hpp>
#include <glad/glad.h>

#include "Model.h"

Model::Model() :data(std::make_shared<Model::Data>()) { }

Model::Data::~Data()
{
	for (const auto& keypair : meshesTransform)
	{
		const auto& [buffer, transforms] = keypair;

		if (buffer)	glDeleteBuffers(1, &buffer);
	}
}

glm::vec3 interpolate(glm::vec3 a, glm::vec3 b, float lerp)
{
	return glm::mix(a, b, lerp);
}

glm::quat interpolate(glm::quat a, glm::quat b, float lerp)
{
	return glm::slerp(a, b, lerp);
}

template <class ValType, class KeyType>
ValType interpolateKeyframes(const std::vector<KeyType>& keys, float time, unsigned& keyCache)
{
	unsigned keysSize = keys.size();
	unsigned keyIndex = keysSize - 1;

	if (time <= keys[0].time) {
		keyIndex = 0;
	}
	else if (time >= keys[keysSize - 1].time) {
		keyIndex = keysSize - 1;
	}
	else {
		for (unsigned int i = keyCache; i < keyCache + keysSize - 1; i++) {
			const KeyType& ka = keys[i % keysSize];
			const KeyType& kb = (i + 1 != keysSize ? keys[(i + 1) % keysSize] : ka);
			if (ka.time <= time && time <= kb.time) {
				keyIndex = i % keysSize;
				break;
			}
		}
	}

	keyCache = keyIndex;
	float t1 = keys[keyIndex].time;
	float t2 = (keyIndex + 1 < keysSize ? keys[keyIndex + 1].time : t1);
	const ValType& p1 = keys[keyIndex].value;
	const ValType& p2 = (keyIndex + 1 < keysSize ? keys[keyIndex + 1].value : p1);

	float lerp = (time - t1) / (t2 - t1);
	if (t2 == t1) {
		lerp = 1.0f;
	}

	return interpolate(p1, p2, lerp);
}

std::vector<glm::mat4> Model::GetNodeTransforms(const std::string& animName, float time, AnimationContext& context)
{
	const auto& animationData = data->animationData;

	auto iter = animationData.animations.find(animName);
	if (iter == animationData.animations.end())
	{
		return cachedNodeTransforms;
	}

	/*
	if (nodeTransforms.size() < animationData.nodes.size())
	{
		nodeTransforms.resize(animationData.nodes.size());
	}*/

	const Animation& animation = iter->second;
	time += animation.startTime;

	for (unsigned i = 0; i < animationData.nodes.size(); i++) {
		const ModelNode& node = animationData.nodes[i];

		glm::mat4 parentTransform(1.0f);
		if (node.parent < animationData.nodes.size()) {
			parentTransform = cachedNodeTransforms[node.parent];
		}

		glm::mat4 nodeTransform(1.0f);

		auto channelIdIter = animation.channelIdMap.find(i);
		if (channelIdIter == animation.channelIdMap.end()) {
			// Not an animated node
			nodeTransform = node.transform;
		}
		else {
			unsigned channelId = channelIdIter->second;
			const Channel& channel = animation.channels[channelId];
			ChannelContext& channelContext = context.channelContexts[channelId];

			glm::vec3 pos = interpolateKeyframes<glm::vec3, PositionKey>(channel.positionKeys, time, channelContext.positionKey);
			glm::quat rot = interpolateKeyframes<glm::quat, RotationKey>(channel.rotationKeys, time, channelContext.rotationKey);
			glm::vec3 scale = interpolateKeyframes<glm::vec3, ScaleKey>(channel.scaleKeys, time, channelContext.scaleKey);

			glm::mat4 posMatrix = glm::mat4(
				1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				pos.x, pos.y, pos.z, 1.0f
			);
			glm::mat4 rotMatrix(glm::toMat4(rot));
			glm::mat4 scaleMatrix = glm::mat4(
				scale.x, 0.0f, 0.0f, 0.0f,
				0.0f, scale.y, 0.0f, 0.0f,
				0.0f, 0.0f, scale.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f
			);
			nodeTransform = posMatrix * rotMatrix * scaleMatrix;
		}
		glm::mat4 globalTransform = parentTransform * nodeTransform;

		cachedNodeTransforms[i] = globalTransform;
	}

	return cachedNodeTransforms;
}

void Model::SetMeshTransform(unsigned int meshIndex, unsigned int meshInstanceIndex, const glm::mat4& transform)
{
	auto& [buffer, transforms] = data->meshesTransform[meshIndex];
	transforms[meshInstanceIndex] = transform;

	// We dont't deal with transform buffer here because of culling => see renderer.cpp
}

void Model::SetMeshCulling(unsigned int meshIndex, bool val)
{
	data->meshes[meshIndex].hasBoundingSphere = val;
}

Material Model::GetMeshMaterial(unsigned int meshIndex) const
{
	return data->meshes[meshIndex].material;
}

void Model::SetMeshMaterial(unsigned int meshIndex, const Material& material)
{
	data->meshes[meshIndex].material = material;
}

void Model::GenVAO() const
{
	for (int i = 0; i != data->meshesTransform.size(); i++)
	{
		const auto& [buffer, transforms] = data->meshesTransform[i];

		auto& mesh = data->meshes[i];

		mesh.GenVAO();

		if (buffer)
		{
			glBindVertexArray(mesh.GetVAO());

			glBindBuffer(GL_ARRAY_BUFFER, buffer);
			// set attribute pointers for matrix (4 times vec4)
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(0, 1);
			glVertexAttribDivisor(1, 1);
			glVertexAttribDivisor(2, 1);
			glVertexAttribDivisor(3, 1);

			glBindVertexArray(0);
		}
	}
}

std::vector<std::string> Model::GetAnimationName() const
{
	std::vector<std::string> animationName;
	for (const auto& anim : data->animationData.animations) {
		animationName.push_back(anim.first);
	}
	return std::move(animationName);
}

Model ModelBuilder::BuildFromMesh(const std::vector<Mesh>& meshes, const std::vector<std::vector<glm::mat4>>& meshesTransform)
{
	Model model;
	model.data->meshes = meshes;

	for (int i = 0; i != meshesTransform.size(); i++)
	{
		const auto& transforms = meshesTransform[i];
		auto& mesh = model.data->meshes[i];

		if (transforms.size() < 2)
		{
			// Don't use instance buffer
			model.data->meshesTransform.push_back(std::tuple<GLuint, std::vector<glm::mat4>>(0, transforms));
			mesh.material.SetProperty("useInstance", 0);
		}
		else
		{
			GLuint buffer;
			glGenBuffers(1, &buffer);
			glBindBuffer(GL_ARRAY_BUFFER, buffer);

			// Just create the maxium space for future use
			// because we are going go culling
			glBufferData(GL_ARRAY_BUFFER, transforms.size() * sizeof(glm::mat4), 0, GL_STREAM_DRAW);

			model.data->meshesTransform.push_back(std::tuple<GLuint, std::vector<glm::mat4>>(buffer, transforms));
			mesh.material.SetProperty("useInstance", 1);
		}
	}

	return std::move(model);
}

Model ModelBuilder::BuildFromSingleMesh(const Mesh& mesh, const glm::mat4& transform)
{
	Model model;
	model.data->meshes = std::vector<Mesh>{ mesh };

	model.data->meshesTransform = std::vector<std::tuple<GLuint, std::vector<glm::mat4>>>{
		std::tuple<GLuint, std::vector<glm::mat4>>(0, std::vector<glm::mat4>{ std::vector<glm::mat4>{ transform } })
	};

	model.data->meshes[0].material.SetProperty("useInstance", 0);

	return std::move(model);
}