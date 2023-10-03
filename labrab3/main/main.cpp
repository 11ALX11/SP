#include <iostream>
#include "MainLibrary.h"

using namespace std;

int main()
{
    string text;
    char lookup;

    cout << "Enter text:" << endl;
    cin >> text;
    cout << "Enter char for lookup: ";
    cin >> lookup;

    cout << "Amount of \'" << lookup << "\' in a text is " << count_сharacters(text, lookup) << endl;
    cout << "Most frequent character is \'" << most_frequent_character(text) << "\'" << endl;

    return 0;
}
