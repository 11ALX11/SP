#include "Car.h"

using namespace std;

Car::Car(const string& dbName) {
	int rc = sqlite3_open(dbName.c_str(), &db);

	if (rc != SQLITE_OK) {
		throw runtime_error("Cannot open database: " + string(sqlite3_errmsg(db)));
	}

	this->CreateTable();
}

Car::~Car() {
	sqlite3_close(db);
}

bool Car::CreateTable() {
	string checkQuery = "SELECT id FROM cars;";
	int checkResult = sqlite3_exec(db, checkQuery.c_str(), nullptr, nullptr, nullptr);

	if (checkResult == SQLITE_OK) {
		return true;
	}

	string createQuery = "CREATE TABLE IF NOT EXISTS cars (id INTEGER PRIMARY KEY, brandModel TEXT, year INTEGER, color TEXT, mileage INTEGER);";
	int createResult = sqlite3_exec(db, createQuery.c_str(), nullptr, nullptr, nullptr);

	if (createResult == SQLITE_OK) {
		InsertCar("Toyota Camry", 2022, "Blue", 5000);
		InsertCar("Honda Civic", 2021, "Silver", 12000);
		InsertCar("Ford Mustang", 2020, "Red", 15000);
	}

	return createResult == SQLITE_OK;
}

bool Car::InsertCar(const string& brandModel, int year, const string& color, int mileage) {
	string query = "INSERT INTO cars (brandModel, year, color, mileage) VALUES ('" + brandModel + "', " + to_string(year) + ", '" + color + "', " + to_string(mileage) + ");";

	int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

bool Car::UpdateCar(int id, const string& brandModel, int year, const string& color, int mileage) {
	string query = "UPDATE cars SET brandModel='" + brandModel + "', year=" + to_string(year) + ", color='" + color + "', mileage=" + to_string(mileage) + " WHERE id=" + to_string(id) + ";";

	int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

bool Car::DeleteCar(int id) {
	string query = "DELETE FROM cars WHERE id=" + to_string(id) + ";";

	int rc = sqlite3_exec(db, query.c_str(), nullptr, nullptr, nullptr);

	return rc == SQLITE_OK;
}

vector<string> Car::GetCars() {
	vector<string> result;
	string query = "SELECT * FROM cars;";

	sqlite3_exec(db, query.c_str(), [](void* data, int argc, char** argv, char** /*azColName*/) -> int {
		string rowData;
		for (int i = 0; i < argc; ++i) {
			rowData += argv[i];
			if (i+1 < argc) rowData += "\t|\t";
		}
		reinterpret_cast<vector<string>*>(data)->emplace_back(rowData);
		return 0;
	}, &result, nullptr);

	return result;
}