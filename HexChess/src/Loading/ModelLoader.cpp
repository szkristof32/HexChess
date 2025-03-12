#include <pch.h>
#include "ModelLoader.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace HexChess {

	ModelLoader::ModelLoader(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Renderer(renderer)
	{
	}

	ModelLoader::~ModelLoader()
	{
	}

	Model ModelLoader::LoadModel(const std::filesystem::path& filepath)
	{
		m_Vertices.clear();
		m_Indices.clear();

		const auto& path = filepath.string();
		const aiScene* scene = m_Importer.ReadFile(path, aiProcess_Triangulate | aiProcess_JoinIdenticalVertices);
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cerr << m_Importer.GetErrorString() << "\n";
			assert(false);
		}

		ProcessNode(scene->mRootNode, scene);

		Model model{};
		model.VertexBuffer = m_Renderer->CreateVertexBuffer(m_Vertices.size() * sizeof(Vertex), m_Vertices.data());
		model.IndexBuffer = m_Renderer->CreateIndexBuffer(m_Indices.size(), m_Indices.data());
		model.IndexCount = (uint32_t)m_Indices.size();

		return model;
	}

	void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene)
	{
		for (uint32_t i = 0;i < node->mNumMeshes;i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			ProcessMesh(mesh, scene);
		}

		for (uint32_t i = 0;i < node->mNumChildren;i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}

	void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene)
	{
		m_Vertices.reserve(mesh->mNumVertices);

		for (uint32_t i = 0;i < mesh->mNumVertices;i++)
		{
			Vertex vertex{};
			vertex.Position = {
				mesh->mVertices[i].x,
				mesh->mVertices[i].y,
				mesh->mVertices[i].z
			};
			vertex.Normal = {
				mesh->mNormals[i].x,
				mesh->mNormals[i].y,
				mesh->mNormals[i].z
			};

			m_Vertices.emplace_back(vertex);
		}

		m_Indices.reserve(mesh->mNumFaces * 3); // Rough estimate of the total amount of indices

		for (uint32_t i = 0;i < mesh->mNumFaces;i++)
		{
			aiFace face = mesh->mFaces[i];
			for (uint32_t j = 0;j < face.mNumIndices;j++)
				m_Indices.emplace_back(face.mIndices[j]);
		}
	}

}
