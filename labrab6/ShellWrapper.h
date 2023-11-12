#pragma once

#include <iostream>
#include <string>

#include "Car.h"

using namespace std;

class ShellWrapper {
public:
    ShellWrapper(const string& dbName);

    void Run();

private:
    Car carTable;
};


