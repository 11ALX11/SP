#pragma once

#include <string>
#include <iostream>

using namespace std;

class Command
{
protected:
	string name;
	int memoryNeeded;

public:
	Command(const string name, int memoryNeeded)
		: name(name), memoryNeeded(memoryNeeded) {}

	// this virtual method ( = 0 ) makes this class abstruct and creates a need to override this method
	virtual int doAction() = 0;

	string getName() {
		return name;
	}
};



class CopyCommand : public Command
{
public:
	CopyCommand(const string name, int memoryNeeded)
		: Command(name, memoryNeeded) {}

	CopyCommand()
		: Command("copy", 10) {}

	int doAction() override {
		cout << "Copying data. Memory needed: " << memoryNeeded << "\n";
		return memoryNeeded;
	}
};

class DeleteCommand : public Command
{
public:
	DeleteCommand(const string name, int memoryNeeded)
		: Command(name, memoryNeeded) {}

	DeleteCommand()
		: Command("del", 0) {}

	int doAction() override {
		cout << "Deleting data. Memory needed: " << memoryNeeded << "\n";
		return memoryNeeded;
	}
};

class MoveFileCommand : public Command
{
public:
	MoveFileCommand(const string name, int memoryNeeded)
		: Command(name, memoryNeeded) {}

	MoveFileCommand()
		: Command("move", 30) {}

	int doAction() override {
		cout << "Moving file. Memory needed: " << memoryNeeded << "\n";
		return memoryNeeded;
	}
};

class RenameFileCommand : public Command
{
public:
	RenameFileCommand(const string name, int memoryNeeded)
		: Command(name, memoryNeeded) {}

	RenameFileCommand()
		: Command("rename", 12) {}

	int doAction() override {
		cout << "Renaming file. Memory needed: " << memoryNeeded << "\n";
		return memoryNeeded;
	}
};

class CreateFileCommand : public Command
{
public:
	CreateFileCommand(const string name, int memoryNeeded)
		: Command(name, memoryNeeded) {}

	CreateFileCommand()
		: Command("create", 23) {}

	int doAction() override {
		cout << "Creating file. Memory needed: " << memoryNeeded << "\n";
		return memoryNeeded;
	}
};
