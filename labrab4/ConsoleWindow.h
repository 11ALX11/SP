#pragma once

#include <string>
#include <vector>

#include "Window.h"
#include "Command.h"

using namespace std;

class ConsoleWindow : public Window
{
protected:
	vector<Command*> commandsList = { new CopyCommand(), new CopyCommand("xcopy", 5), new DeleteCommand(),
									new MoveFileCommand(), new RenameFileCommand(), new CreateFileCommand() };
	int color;

public:
	ConsoleWindow(int id, int height, int width, bool areAdminRightsGranted, int color)
		: Window(id, height, width, areAdminRightsGranted), color(color) {}

	~ConsoleWindow() {
		for (auto command : commandsList) {
			delete command;
		}
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
};