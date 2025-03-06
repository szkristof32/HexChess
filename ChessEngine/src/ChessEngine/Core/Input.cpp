#include "pch.h"
#include "Input.h"

namespace ChessEngine {

	Input::Input()
	{
	}

	Input::~Input()
	{
	}

	void Input::Update()
	{
		m_KeysPressed.clear();
		m_KeysReleased.clear();
		m_ButtonsPressed.clear();
		m_ButtonsReleased.clear();

		m_MouseDx = m_MouseX - m_MouseXPrevFrame;
		m_MouseDy = m_MouseY - m_MouseYPrevFrame;
		m_MouseXPrevFrame = m_MouseX;
		m_MouseYPrevFrame = m_MouseY;
		m_Scroll = 0;
	}

	void Input::OnKeyPress(uint32_t key)
	{
		m_KeysDown.insert(key);
		m_KeysPressed.insert(key);
	}

	void Input::OnKeyRelease(uint32_t key)
	{
		m_KeysDown.erase(key);
		m_KeysReleased.insert(key);
	}

	void Input::OnButtonPress(uint32_t button)
	{
		m_ButtonsDown.insert(button);
		m_ButtonsPressed.insert(button);
	}

	void Input::OnButtonRelease(uint32_t button)
	{
		m_ButtonsDown.erase(button);
		m_ButtonsReleased.insert(button);
	}

	void Input::OnMouseMove(float x, float y)
	{
		m_MouseX = x;
		m_MouseY = y;
	}

	void Input::OnScroll(float scroll)
	{
		m_Scroll = scroll;
	}

}
