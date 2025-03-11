#pragma once

#include "Model.h"
#include "Loading/ModelLoader.h"

namespace HexChess {

	class ModelRepository
	{
	public:
		ModelRepository(const std::shared_ptr<ChessEngine::Renderer>& renderer);
		~ModelRepository();

		const Model& GetModel(const std::filesystem::path& path, const glm::vec3& colour);
	private:
		ModelLoader m_Loader;
		std::unordered_map<std::filesystem::path, Model> m_Models;
	};

}
