#pragma once

#include "ChessEngine/Core/Keys.h"

namespace ChessEngine {

	class Input
	{
	public:
		Input();
		~Input();

		void Update();

		bool IsKeyDown(uint32_t key) const { return m_KeysDown.contains(key); }
		bool IsKeyPressed(uint32_t key) const { return m_KeysPressed.contains(key); }
		bool IsKeyReleased(uint32_t key) const { return m_KeysReleased.contains(key); }

		bool IsButtonDown(uint32_t button) const { return m_ButtonsDown.contains(button); }
		bool IsButtonPressed(uint32_t button) const { return m_ButtonsPressed.contains(button); }
		bool IsButtonReleased(uint32_t button) const { return m_ButtonsReleased.contains(button); }

		float GetMouseX() const { return m_MouseX; }
		float GetMouseY() const { return m_MouseY; }
		float GetMouseDx() const { return m_MouseDx; }
		float GetMouseDy() const { return m_MouseDy; }

		float GetScroll() const { return m_Scroll; }
	public:
		void OnKeyPress(uint32_t key);
		void OnKeyRelease(uint32_t key);

		void OnButtonPress(uint32_t button);
		void OnButtonRelease(uint32_t button);

		void OnMouseMove(float x, float y);
		void OnScroll(float scroll);
	private:
		std::set<uint32_t> m_KeysDown;
		std::set<uint32_t> m_KeysPressed;
		std::set<uint32_t> m_KeysReleased;

		std::set<uint32_t> m_ButtonsDown;
		std::set<uint32_t> m_ButtonsPressed;
		std::set<uint32_t> m_ButtonsReleased;

		float m_MouseX = 0, m_MouseY;
		float m_MouseXPrevFrame, m_MouseYPrevFrame;
		float m_MouseDx, m_MouseDy;
		float m_Scroll = 0;
	};

}
