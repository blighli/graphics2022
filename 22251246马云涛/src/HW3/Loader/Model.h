#pragma once
#include <glm/glm.hpp>

#include <vector>
#include <map>
#include <memory>

#include "Mesh.h"
#include "Material.h"
#include "Animation.h"

/*! Node in the model's hierarchy. This is strongly tied to an AnimationData struct.
	An "index" here refers to an index into AnimationData.nodes. */
struct ModelNode
{
	/*! Name of this node, purely for debugging purposes. */
	std::string name;

	/*! Transform of this node. */
	glm::mat4 transform;

	/*! True if the node is the root node. */
	bool isRoot;

	/*! Index of this node's parent. */
	unsigned int parent;

	/*! Indices of this node's children. */
	std::vector<unsigned int> children;
};

/*! Data for all the animations which a model contains. */
struct AnimationData
{
	/*! The animations themselves. */
	std::unordered_map<std::string, Animation> animations;

	/*! A map of node names to node IDs. (Is this necessary?) */
	std::unordered_map<std::string, unsigned int> nodeIdMap;

	/*! The node hierarchy. */
	std::vector<ModelNode> nodes;
};

class Model
{
public:
	struct Data;

	friend class ModelLoader;
	friend class ModelBuilder;
	friend class Box;
	friend class Renderer;

	Model();

	void SetMeshTransform(unsigned int meshIndex, unsigned int meshInstanceIndex, const glm::mat4 &transform);
	void SetMeshCulling(unsigned int meshIndex, bool val);

	Material GetMeshMaterial(unsigned int meshIndex) const;
	void SetMeshMaterial(unsigned int meshIndex, const Material &material);

	std::vector<glm::mat4> GetNodeTransforms(const std::string &animation, float time, AnimationContext &context);

	void GenVAO() const;

	std::vector<std::string> GetAnimationName() const;

	// Material of Model Level (Maybe override by material of mesh)
	Material material;

	std::shared_ptr<Model::Data> data;

private:
	/*! Cached transforms returned from getNodeTransforms. */
	std::vector<glm::mat4> cachedNodeTransforms;
};

struct Model::Data
{
	Data() : meshes(), meshesTransform(), animationData(){};
	~Data();
	/*!
	 * @brief Mesh
	 */
	std::vector<Mesh> meshes;

	/*!
	 * @brief Transform of the mesh in the hierarhy (when with animation for bindpose)
	 * and instanced model transform buffer
	 */
	std::vector<std::tuple<GLuint, std::vector<glm::mat4>>> meshesTransform;

	/*! All the animations. */
	AnimationData animationData;
};

class ModelBuilder
{
public:
	static Model BuildFromMesh(const std::vector<Mesh> &meshes, const std::vector<std::vector<glm::mat4>> &meshesTransform);
	static Model BuildFromSingleMesh(const Mesh &mesh, const glm::mat4 &transform);
};