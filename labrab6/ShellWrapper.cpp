#include "ShellWrapper.h"

using namespace std;

ShellWrapper::ShellWrapper(const string& dbName) : carTable(dbName) {}

void ShellWrapper::Run() {

    cout << "Welcome!\n";

    int choice;
    vector<string> cars_list;

    string brandModel, color;
    int id, year, mileage;

    while (true) {
        cout << endl;
        cout << "1. Display all cars\n";
        cout << "2. Add a new car\n";
        cout << "3. Update car information\n";
        cout << "4. Delete a car\n";
        cout << "5. Exit\n";
        cout << endl << "Choose an action: ";
        cin >> choice;
        cout << endl;

        switch (choice) {
        case 1:
            cars_list = carTable.GetCars();
            cout << "id\t|\tbrandModel\t|\tyear\t|\tcolor\t|\tmileage\n";
            cout << "-----------------------------------------------------------------------------------------------\n";
            for (const auto& car : cars_list) {
                cout << car << endl;
            }
            break;
        case 2:
            cout << "Enter brand model (text): ";
            cin.ignore();
            getline(cin, brandModel);

            cout << "Enter year (int): ";
            cin >> year;

            cin.ignore();

            cout << "Enter color (string): ";
            getline(cin, color);

            cout << "Enter mileage (int): ";
            cin >> mileage;

            cout << endl << (carTable.InsertCar(brandModel, year, color, mileage) ? "Ok." : "Error.") << endl;

            break;
        case 3:
            cout << "Choose a car (id: int): ";
            cin >> id;

            cin.ignore();

            cout << "Enter brand model (text): ";
            getline(cin, brandModel);

            cout << "Enter year (int): ";
            cin >> year;

            cin.ignore();

            cout << "Enter color (string): ";
            getline(cin, color);

            cout << "Enter mileage (int): ";
            cin >> mileage;

            cout << endl << (carTable.UpdateCar(id, brandModel, year, color, mileage) ? "Ok." : "Error.") << endl;

            break;
        case 4:
            cout << "Choose a car (id: int): ";
            cin >> id;

            cout << endl << (carTable.DeleteCar(id) ? "Ok." : "Error.") << endl;

            break;
        case 5:
        case 0:
            cout << "Exiting the program.\n";
            return;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
}
