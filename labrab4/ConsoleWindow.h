#pragma once

#include <string>
#include <vector>

// color and HWND with handle
#include <windows.h> 

#include "Window.h"
#include "Command.h"

const int DEFAULT_CONSOLE_COLOR = 15;

using namespace std;

// класс-надстройка для окна простого консольного окна
class ConsoleWindow : public Window
{
protected:
	vector<Command*> commandsList;
	HANDLE windowHandle;
	int color;

public:
	ConsoleWindow(int id, vector<Command*> commandsList, int color)
		: Window(id, GetConsoleWindow()), windowHandle(GetStdHandle(STD_OUTPUT_HANDLE))
		, commandsList(commandsList)
	{
		setColor(color);
	}

	ConsoleWindow(int id, int width, int height, vector<Command*> commandsList, int color)
		: Window(id, GetConsoleWindow(), width, height), windowHandle(GetStdHandle(STD_OUTPUT_HANDLE))
		, commandsList(commandsList)
	{
		setColor(color);
	}

	~ConsoleWindow() {
		for (auto command : commandsList) {
			delete command;
		}
		// Revert color to normal
		SetConsoleTextAttribute(windowHandle, DEFAULT_CONSOLE_COLOR);
	}

	void executeCommand(const string commandName) {
		for (auto command : commandsList) {
			if (command->getName() == commandName) {
				this->doWork(command->doAction());
				return;
			}
		}

		cout << "Command '" << commandName << "' is unknown.\n";
	}

	void printAvailableCommands() {
		cout << "Available Commands:\n";
		for (auto command : commandsList) {
			cout << "\t" << command->getName() << "\n";
		}
	}

	void printMemoryUsed() {
		cout << "Memory used: " << memoryNeeded << "\n";
	}

	void setColor(int color) {
		this->color = color;
		SetConsoleTextAttribute(windowHandle, this->color);
	}
};