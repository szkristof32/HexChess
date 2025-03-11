#include <pch.h>
#include "ModelRepository.h"

namespace HexChess {

	ModelRepository::ModelRepository(const std::shared_ptr<ChessEngine::Renderer>& renderer)
		: m_Loader(renderer)
	{
	}

	ModelRepository::~ModelRepository()
	{
	}

	const Model& ModelRepository::GetModel(const std::filesystem::path& path, const glm::vec3& colour)
	{
		if (!m_Models.contains(path))
			m_Models[path] = m_Loader.LoadModel(path, colour);

		return m_Models.at(path);
	}

}
