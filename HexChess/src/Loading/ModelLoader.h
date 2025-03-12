#pragma once

#include "Model.h"

#include <ChessEngine/Rendering/Renderer.h>

#include <glm/glm.hpp>
#include <assimp/Importer.hpp>

struct aiNode;
struct aiMesh;
struct aiScene;

namespace HexChess {

	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
	};

	class ModelLoader
	{
	public:
		ModelLoader(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~ModelLoader();

		Model LoadModel(const std::filesystem::path& filepath);
	private:
		void ProcessNode(aiNode* node, const aiScene* scene);
		void ProcessMesh(aiMesh* mesh, const aiScene* scene);
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;
		Assimp::Importer m_Importer;

		std::vector<Vertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};

}
