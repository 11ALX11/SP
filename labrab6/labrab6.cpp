/* 
 * Вариант #5
 * Создать консольную программу для работы с базой данных. 
 * Программа должна уметь выводить в консоль данные из БД,
 * записывать новые данные, а также редактировать и удалять уже существующие. 
 */

#include "ShellWrapper.h"

int main(int argc, char* argv[])
{
    try {
        std::string dbFilename;

        if (argc > 1) {
            dbFilename = argv[1];
        }
        else {
            cout << "Enter dbFilename as a program argument!\n";
            return 0;
        }

        ShellWrapper sw(dbFilename);
        sw.Run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception caught: " << e.what() << std::endl;
        return 10;
    }
    catch (...) {
        std::cerr << "Unknown exception caught." << std::endl;
        return 100;
    }

    return 0;
}
