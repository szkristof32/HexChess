#include "pch.h"
#include "BoardGenerator.h"

namespace HexChess {

	BoardGenerator::BoardGenerator(BoardConfig& config)
		: m_Config(config)
	{
	}

	BoardGenerator::~BoardGenerator()
	{
	}

	void BoardGenerator::GenerateBoard()
	{
		m_Vertices.clear();
		m_Indices.clear();

		constexpr int32_t gridSize = 10;
		for (int32_t i = -(gridSize / 2);i < gridSize / 2;i++)
		{
			for (int32_t j = -(gridSize / 2);j < gridSize / 2;j++)
			{
				uint32_t colourIndex = (i + j + gridSize) % 3;

				glm::vec3 center = GetPositionForHex(i, j);
				glm::vec3 colour = glm::vec3((float)colourIndex / 2.0f);

				CreateHex(m_Config.InnerSize, m_Config.OuterSize, m_Config.Height, m_Config.IsFlatTopped, center, colour);
			}
		}

	}

	glm::vec3 BoardGenerator::GetPositionForHex(int32_t xPosition, int32_t yPosition)
	{
		int32_t column = xPosition;
		int32_t row = yPosition;

		bool shouldOffset = false;
		float size = m_Config.OuterSize;

		glm::vec3 finalPosition{};

		if (!m_Config.IsFlatTopped)
		{
			shouldOffset = (row % 2) == 0;

			float width = glm::sqrt(3.0f) * size;
			float height = 2.0f * size;

			float horizontalDistance = width;
			float verticalDistance = height * 3.0f / 4.0f;

			float offset = shouldOffset ? width / 2.0f : 0.0f;

			finalPosition.x = (column * horizontalDistance) + offset;
			finalPosition.z = row * verticalDistance;
		}
		else
		{
			shouldOffset = (column % 2) == 0;

			float width = 2.0f * size;
			float height = glm::sqrt(3.0f) * size;

			float horizontalDistance = width * 3.0f / 4.0f;
			float verticalDistance = height;

			float offset = shouldOffset ? height / 2.0f : 0.0f;

			finalPosition.x = column * horizontalDistance;
			finalPosition.z = (row * verticalDistance) - offset;
		}

		return finalPosition;
	}

	void BoardGenerator::CreateHex(float innerRad, float outerRad, float height, bool isFlatTopped, const glm::vec3& center, const glm::vec3& colour)
	{
		for (uint32_t i = 0;i < 6;i++)
		{
			CreateFace(innerRad, outerRad, height / 2.0f, height / 2.0f, i, center, colour, isFlatTopped);
		}

		for (uint32_t i = 0;i < 6;i++)
		{
			CreateFace(innerRad, outerRad, -height / 2.0f, -height / 2.0f, i, center, colour, isFlatTopped, true);
		}

		for (uint32_t i = 0;i < 6;i++)
		{
			CreateFace(outerRad, outerRad, height / 2.0f, -height / 2.0f, i, center, colour, isFlatTopped, true);
		}

		for (uint32_t i = 0;i < 6;i++)
		{
			CreateFace(innerRad, innerRad, height / 2.0f, -height / 2.0f, i, center, colour, isFlatTopped);
		}
	}

	void BoardGenerator::CreateFace(float innerRad, float outerRad, float heightA, float heightB, uint32_t point, const glm::vec3& center, const glm::vec3& colour, bool flatTopped, bool reverse)
	{
		uint32_t indexOffset = (uint32_t)m_Vertices.size();
		uint32_t indices[] = { 0, 1, 2, 2, 3, 0 };

		auto pointA = GetPoint(innerRad, heightB, point, center, flatTopped);
		auto pointB = GetPoint(innerRad, heightB, (point < 5) ? point + 1 : 0, center, flatTopped);
		auto pointC = GetPoint(outerRad, heightA, (point < 5) ? point + 1 : 0, center, flatTopped);
		auto pointD = GetPoint(outerRad, heightA, point, center, flatTopped);

		glm::vec3 normal = GetNormal(pointA, pointB, pointC);
		normal = reverse ? -normal : normal;
		pointA.Normal = pointB.Normal = pointC.Normal = pointD.Normal = normal;
		pointA.Colour = pointB.Colour = pointC.Colour = pointD.Colour = colour;

		m_Vertices.emplace_back(pointA);
		m_Vertices.emplace_back(pointB);
		m_Vertices.emplace_back(pointC);
		m_Vertices.emplace_back(pointD);

		for (uint32_t i = 0;i < 6;i++)
		{
			uint32_t index = reverse ? 6 - i - 1 : i;
			m_Indices.emplace_back(indexOffset + indices[index]);
		}
	}

	BoardVertex BoardGenerator::GetPoint(float size, float height, uint32_t index, const glm::vec3& center, bool isFlatTopped)
	{
		float angle = index * (glm::pi<float>() / 3.0f) - (!isFlatTopped ? glm::pi<float>() / 6.0f : 0.0f);

		BoardVertex vertex{};
		vertex.Position = center + glm::vec3{ glm::cos(angle) * size, height, glm::sin(angle) * size };
		vertex.Colour = { 0.2f, 0.4f, 0.8f };
		vertex.Normal = { 0.0f, 1.0f, 0.0f };

		return vertex;
	}

	glm::vec3 BoardGenerator::GetNormal(const BoardVertex& point1, const BoardVertex& point2, const BoardVertex& point3)
	{
		glm::vec3 a, b;

		a = point3.Position - point2.Position;
		b = point1.Position - point2.Position;

		glm::vec3 normal = glm::cross(a, b);

		return normal;
	}

}
