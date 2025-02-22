#include <iostream>

#include <ChessEngine/Core/Engine.h>
#include <ChessEngine/Core/Client.h>

using namespace ChessEngine;

class ChessGame : public Client
{
public:
	virtual void OnUpdate(float deltaInSeconds) override
	{
		std::cout << "OnUpdate\n";
		std::cout << GetRenderer() << "\n";
	}
};

int main(int argc, char** argv)
{
	ChessGame game;

	Engine engine(&game);
	engine.Run();
}
