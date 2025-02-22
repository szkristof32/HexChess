#pragma once

#include "ChessEngine/Rendering/Renderer.h"

#include <functional>

namespace ChessEngine {

	class Client
	{
	public:
		using GetRendererFn = std::function<std::shared_ptr<Renderer>()>;
	public:
		virtual ~Client() = default;

		virtual void OnInit() {}
		virtual void OnShutdown() {}

		virtual void OnUpdate(float deltaInSeconds) {}
	public:
		GetRendererFn GetRenderer;
	};

}
