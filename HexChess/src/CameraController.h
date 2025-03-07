#pragma once

#include "ChessEngine/Core/Input.h"

#include <glm/glm.hpp>

namespace HexChess {

	class CameraController
	{
	public:
		CameraController(const std::shared_ptr<ChessEngine::Input>& input);
		~CameraController();

		void Update(float delta);

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }

		void OnResize(uint32_t width, uint32_t height);
	private:
		void RenderUI();

		void CreateProjectionMatrix();
		void CreateViewMatrix();
		void CalculatePosition();
	private:
		std::shared_ptr<ChessEngine::Input> m_Input;

		float m_Pitch = 30.0f;
		float m_AngleAroundCenter = 0.0f;
		float m_Distance = 10.0f;

		glm::vec3 m_Center;
		glm::vec3 m_Position;

		uint32_t m_CurrentWidth = 1280, m_CurrentHeight = 720;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewMatrix;

		float m_HorizontalSensitivity = 8.5f;
		float m_VerticalSensitivity = 7.5f;
		float m_ScrollSensitivity = 20.0f;
	};

}
