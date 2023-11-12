#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <sqlite3.h>

using namespace std;

class Car {
public:
    Car(const std::string& dbName);
    ~Car();

    bool CreateTable();
    bool InsertCar(const string& brandModel, int year, const string& color, int mileage);
    bool UpdateCar(int id, const string& brandModel, int year, const string& color, int mileage);
    bool DeleteCar(int id);
    vector<string> GetCars();

private:
    sqlite3* db;
};