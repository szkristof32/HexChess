#pragma once

#include <glm/glm.hpp>
#include <glm/ext/scalar_constants.hpp>

namespace HexChess {

	struct BoardVertex
	{
		glm::vec3 Position;
		glm::vec3 Colour;
		glm::vec3 Normal;
	};

	struct BoardConfig
	{
		float InnerSize = 0.001f;
		float OuterSize = 1.0f;
		float Height = 0.4f;
		bool IsFlatTopped = false;
		glm::vec3 Colours[3] = {
			{ 1.00f, 0.81f, 0.62f },
			{ 0.91f, 0.68f, 0.44f },
			{ 0.82f, 0.55f, 0.27f }
		};
	};

	class BoardGenerator
	{
	public:
		BoardGenerator(BoardConfig& config);
		~BoardGenerator();

		void GenerateBoard();

		const std::vector<BoardVertex>& GetVertices() const { return m_Vertices; }
		size_t GetVertexCount() const { return m_Vertices.size(); }
		const std::vector<uint32_t>& GetIndices() const { return m_Indices; }
		size_t GetIndexCount() const { return m_Indices.size(); }
	private:
		glm::vec3 GetPositionForHex(int32_t xPosition, int32_t yPosition);
		bool ShouldSkip(int32_t xPosition, int32_t yPosition);

		void CreateHex(float innerRad, float outerRad, float height, bool isFlatTopped, const glm::vec3& center, const glm::vec3& colour);
		void CreateFace(float innerRad, float outerRad, float heightA, float heightB, uint32_t point, const glm::vec3& center, const glm::vec3& colour, bool flatTopped = false, bool reverse = false);
		BoardVertex GetPoint(float size, float height, uint32_t index, const glm::vec3& center, bool isFlatTopped = false);
		glm::vec3 GetNormal(const BoardVertex& point1, const BoardVertex& point2, const BoardVertex& point3);
	private:
		BoardConfig& m_Config;

		std::vector<BoardVertex> m_Vertices;
		std::vector<uint32_t> m_Indices;
	};

}
