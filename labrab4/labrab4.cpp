/*
 * Variant #5
 */

#include <iostream>

#include "ConsoleWindow.h"

int main()
{
	vector<Command*> commandsList = { new CopyCommand(), new CopyCommand("xcopy", 5), new DeleteCommand(),
									new MoveFileCommand(), new RenameFileCommand(), new CreateFileCommand() };

	ConsoleWindow w(1, 800, 600, commandsList, 10);
	// ConsoleWindow w(1, commandsList, 10);
	w.printAvailableCommands();
	Sleep(100);
	w.showOrHide();
	w.setColor(14);

	cout << "\n";

	w.executeCommand("xcopy");
	w.executeCommand("copy");
	w.executeCommand("move");
	w.executeCommand("delete");
	Sleep(100);
	w.showOrHide();
	w.executeCommand("del");
	w.executeCommand("rename");
	w.executeCommand("create");

	cout << "\n";

	w.printMemoryUsed();

	cout << "\n" << "Are Administrator Rights Granted?: " << (w.getAreAdministratorRightsGranted() ? "Yes" : "No") << "\n";

	return 0;
}
