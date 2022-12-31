#pragma once
#include <string>
#include <memory>

#include "Model.h"

struct aiMesh;

enum class ModelLoadingPrefab
{
	Default,
	Optimize
};

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	/*!
	 * @brief Load model from file
	 * @param path
	 * @param preDefineTransform Apply the transform matrix on the root node
	 * @return
	 */
	Model LoadFromFile(const std::string &path, ModelLoadingPrefab prefab = ModelLoadingPrefab::Default, glm::mat4 preDefineTransform = glm::mat4(1.0f), bool computeBoundingSphere = false);
	Model LoadFromSingleMesh(const std::shared_ptr<aiMesh> mesh, glm::mat4 preDefineTransform = glm::mat4(1.0f), bool computeBoundingSphere = false, const Material &material = Material());

	void SetDefaultMaterial(const Material &material);

private:
	struct Impl;
	std::unique_ptr<Impl> impl;
};
