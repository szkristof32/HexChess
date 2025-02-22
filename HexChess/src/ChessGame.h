#pragma once

#include <ChessEngine/Core/Client.h>

#include <ChessEngine/Rendering/Pipeline.h>

namespace HexChess {

	class ChessGame : public ChessEngine::Client
	{
	public:
		virtual void OnInit() override;

		virtual void OnUpdate(float deltaInSeconds) override;
	private:
		std::shared_ptr<ChessEngine::Renderer> m_Renderer;
		std::shared_ptr<ChessEngine::Pipeline> m_BoardPipeline;
	};

}
