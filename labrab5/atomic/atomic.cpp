#include <iostream>
#include <thread>
#include <atomic>

using namespace std;

int main()
{
    for (int i = 0; i < 10; i++) {
        
        cout << i << ":\t";
        atomic<float> variable{ 0 };

        thread thread_plus([&variable]()
            {
                // fetch_add() только для atomic<int> и др.;
                // с Floating типами незя
                variable = variable + 1.0;
                cout << "+2.0 ";
            });

        thread thread_minus([&variable]()
            {
                variable = variable - 1.0;
                cout << "-1.0 ";
            });

        thread thread_multiplier([&variable]()
            {
                variable = variable * 1.5;
                cout << "*1.5 ";
            });

        // Ждем завершения потоков
        thread_plus.join();
        thread_minus.join();
        thread_multiplier.join();

        cout << "\t" << variable << endl;
    }

    return 0;
}
