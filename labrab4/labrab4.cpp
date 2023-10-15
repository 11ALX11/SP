/*
 * Variant #5
 */

#include <iostream>

#include "ConsoleWindow.h"

int main()
{
	ConsoleWindow w(1, 250, 400, false, 0x000000);
	w.printAvailableCommands();

	cout << "\n";

	w.executeCommand("xcopy");
	w.executeCommand("copy");
	w.executeCommand("move");
	w.executeCommand("delete");
	w.executeCommand("del");
	w.executeCommand("rename");
	w.executeCommand("create");

	cout << "\n";

	w.printMemoryUsed();

	return 0;
}
