#include <pch.h>

#include <ChessEngine/Core/Engine.h>

#include "ChessGame.h"

static int Main(int argc, char** argv)
{
	HexChess::ChessGame game;

	ChessEngine::Engine engine(&game);
	engine.Run();

	return EXIT_SUCCESS;
}

#if _WIN32 && RELEASE

#include <Windows.h>

int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	return Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Main(argc, argv);
}

#endif
