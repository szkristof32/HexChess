#include <pch.h>
#include "CameraController.h"

#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>

namespace HexChess {

	CameraController::CameraController(const std::shared_ptr<ChessEngine::Input>& input)
		: m_Input(input), m_Center(), m_Position()
	{
		CreateProjectionMatrix();
		CreateViewMatrix();
	}

	CameraController::~CameraController()
	{
	}

	void CameraController::Update(float delta)
	{
		bool changed = false;

		if (m_Input->IsButtonDown(2))
		{
			m_AngleAroundCenter -= m_Input->GetMouseDx() * delta * m_HorizontalSensitivity;
			m_Pitch += m_Input->GetMouseDy() * delta * m_VerticalSensitivity;
			m_Pitch = glm::clamp(m_Pitch, 0.0f, 85.0f);

			changed = true;
		}
		if (m_Input->GetScroll() != 0)
		{
			m_Distance -= m_Input->GetScroll() * delta * m_ScrollSensitivity;

			changed = true;
		}

		if (changed)
			CreateViewMatrix();

		RenderUI();
	}

	void CameraController::OnResize(uint32_t width, uint32_t height)
	{
		if (width == m_CurrentWidth && height == m_CurrentHeight)
			return;

		m_CurrentWidth = width;
		m_CurrentHeight = height;
		CreateProjectionMatrix();
	}

	void CameraController::RenderUI()
	{
		ImGui::Begin("Camera settings");

		ImGui::DragFloat("Horizontal sensitivity", &m_HorizontalSensitivity, 0.01f, 0.1f, 100.0f);
		ImGui::DragFloat("Vertical sensitivity", &m_VerticalSensitivity, 0.01f, 0.1f, 100.0f);
		ImGui::DragFloat("Scroll sensitivity", &m_ScrollSensitivity, 0.01f, 0.1f, 100.0f);

		if (ImGui::Button("Reset camera"))
		{
			m_Pitch = 60.0f;
			m_Distance = 15.5f;
			m_AngleAroundCenter = 0.0f;

			CreateViewMatrix();
		}

		ImGui::End();
	}

	void CameraController::CreateProjectionMatrix()
	{
		m_ProjectionMatrix = glm::perspectiveFov(glm::radians(70.0f), (float)m_CurrentWidth, (float)m_CurrentHeight, 0.1f, 100.0f);
	}

	void CameraController::CreateViewMatrix()
	{
		CalculatePosition();

		m_ViewMatrix = glm::lookAt(m_Position, m_Center, { 0.0f, 1.0f, 0.0f });
	}

	void CameraController::CalculatePosition()
	{
		float horizontalDistance = glm::cos(glm::radians(m_Pitch)) * m_Distance;
		float verticalDistance = glm::sin(glm::radians(m_Pitch)) * m_Distance;

		m_Position = {
			glm::sin(glm::radians(m_AngleAroundCenter)) * horizontalDistance,
			verticalDistance,
			glm::cos(glm::radians(m_AngleAroundCenter)) * horizontalDistance };
	}

}
