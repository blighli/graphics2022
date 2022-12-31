#include <glad/glad.h>

#include "Material.h"
#include "RenderUtil.h"

static uint64_t nextId;
static std::unordered_map<uint64_t, GLint> shaderUniformCache;

MaterialProperty::MaterialProperty() :type(MaterialPropertyType::Invalid), value(), propertyId(0)
{ }

MaterialProperty::MaterialProperty(glm::vec3 vec3)
	: type(MaterialPropertyType::Vec3)
	, value(new MaterialPropertyValue(vec3))
	, propertyId(0)
{ }

MaterialProperty::MaterialProperty(glm::vec4 vec4)
	: type(MaterialPropertyType::Vec4)
	, value(new MaterialPropertyValue(vec4))
	, propertyId(0)
{ }

MaterialProperty::MaterialProperty(float flt)
	: type(MaterialPropertyType::Float)
	, value(new MaterialPropertyValue(flt))
	, propertyId(0)
{ }

MaterialProperty::MaterialProperty(int val)
	: type(MaterialPropertyType::Int)
	, value(new MaterialPropertyValue(val))
	, propertyId(0)
{ }

MaterialProperty::MaterialProperty(const Texture& texture)
	: type(MaterialPropertyType::Texture)
	, value(new MaterialPropertyValue(texture))
	, propertyId(0)
{ }

MaterialProperty::~MaterialProperty()
{
	if (value && type == MaterialPropertyType::Texture)
		value->texture.~Texture();
}

MaterialProperty::MaterialProperty(const MaterialProperty& property)
{
	*this = property;
}

MaterialProperty& MaterialProperty::operator=(const MaterialProperty& other)
{
	type = other.type;
	propertyId = other.propertyId;
	switch (type)
	{
	case MaterialPropertyType::Vec3:
		value = std::make_unique<MaterialPropertyValue>(other.value->vec3);
		break;
	case MaterialPropertyType::Vec4:
		value = std::make_unique<MaterialPropertyValue>(other.value->vec4);
		break;
	case MaterialPropertyType::Float:
		value = std::make_unique<MaterialPropertyValue>(other.value->flt);
		break;
	case MaterialPropertyType::Int:
		value = std::make_unique<MaterialPropertyValue>(other.value->integer);
		break;
	case MaterialPropertyType::Texture:
		value = std::make_unique<MaterialPropertyValue>(other.value->texture);
		break;
	case MaterialPropertyType::Invalid:
	default:
		break;
	}

	return *this;
}

Material::Material()
	:DrawOrder(MaterialDrawOrder::Less), DrawType(MaterialDrawType::Triangles)
{ }

void Material::SetProperty(const std::string& key, const MaterialProperty& property)
{
	auto iter = properties.find(key);

	if (iter != properties.end()) {
		iter->second = MaterialProperty(property);
	}
	else {
		MaterialProperty copyProp(property);
		copyProp.propertyId = nextId++;

		properties.insert(std::make_pair(key, copyProp));
	}
}

void Material::SetProperty(const std::string& key, MaterialProperty&& property)
{
	auto iter = properties.find(key);

	if (iter != properties.end()) {
		iter->second = std::move(property);
	}
	else {
		property.propertyId = nextId++;
		properties.insert(std::make_pair(key, std::move(property)));
	}
}

void Material::SetTextures(const std::vector<Texture>& textures)
{
	for (unsigned int i = 0; i < textures.size(); i++) {
		MaterialProperty textureProperty(textures[i]);
		std::string key;
		if (textures[i].GetType() == TextureType::Diffuse) {
			key = "texture_diffuse";
		}
		else if (textures[i].GetType() == TextureType::Specular) {
			key = "texture_specular";
		}
		else if (textures[i].GetType() == TextureType::Cubemap) {
			key = "cubemap";
			// not all cubemaps are skyboxes, but they are for now!
			this->DrawOrder = MaterialDrawOrder::LEqual;
		}
		this->SetProperty(key, textureProperty);
	}
}

void Material::Apply(const Shader& shader) const
{
	// preserve 0 for shadow map
	unsigned int curTexture = 1;

	for (auto iter = properties.begin(); iter != properties.end(); ++iter) {
		glCheckError();

		const std::string& propertyName = iter->first;
		const MaterialProperty& property = iter->second;

		glCheckError();

		// Simple mapping - assume we'll never have more than 2^56 total material properties and 2^8
		// shaders and that shaders are assigned ids sequentially (foolish to assume on all implementations?)
		assert(property.propertyId < ((uint64_t)1 << 56) && shader.GetID() < (1 << 8));

		uint64_t hash = property.propertyId << 8 | shader.GetID();
		auto cacheIter = shaderUniformCache.find(hash);

		if (cacheIter == shaderUniformCache.end()) {
			// Miss - get the shader uniform and store it
			GLint shaderUniformId = shader.GetUniformLocation(propertyName);
			auto insertIterPair = shaderUniformCache.insert(std::make_pair(hash, shaderUniformId));
			cacheIter = insertIterPair.first;
		}
		unsigned shaderUniformId = cacheIter->second;

		switch (property.type) {
		case MaterialPropertyType::Vec3:
			glUniform3f(shaderUniformId, property.value->vec3.x, property.value->vec3.y, property.value->vec3.z);
			break;
		case MaterialPropertyType::Vec4:
			glUniform4f(shaderUniformId, property.value->vec4.x, property.value->vec4.y, property.value->vec4.z, property.value->vec4.w);
			break;
		case MaterialPropertyType::Texture:
			glActiveTexture(GL_TEXTURE0 + curTexture); // Activate proper texture unit before binding
			property.value->texture.Bind();
			glUniform1i(shaderUniformId, curTexture);
			curTexture++;
			break;
		case MaterialPropertyType::Float:
			glUniform1f(shaderUniformId, property.value->flt);
			break;
		case MaterialPropertyType::Int:
			glUniform1i(shaderUniformId, property.value->integer);
			break;
		}

		glCheckError();
	}

	int depthFunc = GL_LESS;

	if (DrawOrder == MaterialDrawOrder::Less) {
		depthFunc = GL_LESS;
	}
	else if (DrawOrder == MaterialDrawOrder::LEqual) {
		depthFunc = GL_LEQUAL;
	}
	glDepthFunc(depthFunc);

	glCheckError();
}

std::map<std::string, MaterialProperty>& Material::GetProperties()
{
	return properties;
}