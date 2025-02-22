#include <pch.h>

#include <ChessEngine/Core/Engine.h>

#include "ChessGame.h"

int main(int argc, char** argv)
{
	HexChess::ChessGame game;

	ChessEngine::Engine engine(&game);
	engine.Run();
}
