#pragma once

#include "ChessEngine/Core/Input.h"
#include "ChessEngine/Rendering/Renderer.h"

#include <functional>

namespace ChessEngine {

	class Client
	{
	public:
		using GetRendererFn = std::function<std::shared_ptr<Renderer>()>;
		using GetInputFn = std::function<std::shared_ptr<Input>()>;
	public:
		virtual ~Client() = default;

		virtual void OnInit() {}
		virtual void OnShutdown() {}

		virtual void OnUpdate(float deltaInSeconds) {}
		virtual void OnResize(uint32_t width, uint32_t height) {}
	public:
		GetRendererFn GetRenderer;
		GetInputFn GetInput;
	};

}
