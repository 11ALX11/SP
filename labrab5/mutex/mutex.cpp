#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

int main()
{
    for (int i = 0; i < 10; i++) {

        cout << i << ":\t";
        float variable = 0;
        mutex variable_mutex;

        thread thread_plus([&variable, &variable_mutex]()
            {
                variable_mutex.lock();
                
                variable = variable + 2.0;
                cout << "+2.0 ";

                variable_mutex.unlock();
            });

        thread thread_minus([&variable, &variable_mutex]()
            {
                variable_mutex.lock();

                variable = variable - 1.0;
                cout << "-1.0 ";

                variable_mutex.unlock();
            });

        thread thread_multiplier([&variable, &variable_mutex]()
            {
                variable_mutex.lock();

                variable = variable * 1.5;
                cout << "*1.5 ";

                variable_mutex.unlock();
            });

        thread_plus.join();
        thread_minus.join();
        thread_multiplier.join();

        cout << "\t" << variable << endl;
    }

    return 0;
}