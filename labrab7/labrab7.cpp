/*
 * Вариант #3
 * Необходимо написать программу, используя знания, полученные в лабораторной работе №5.
 * Потоки должны быть синхронизированы для выполнения поставленной задачи или код должен выполнять задачи последовательно.
 * Создать симулятор рынка с минимум 3 торговцами.
 * Торговцы обслуживают по одному человеку в очереди,
 * а после того, как очередь закончилась – начинаю обслуживать людей в очередях других торговцев.
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>

const int PEOPLE = 30;

std::mutex coutMutex;

class Trader {

private:
	std::string name;
	std::queue<std::pair<int, int>> customers;

public:
	std::mutex mtx;

	Trader(std::string name) : name(name) {}

	void serveCustomers();

	void addCustomer(int id, int customerDifficulty)
	{
		std::unique_lock<std::mutex> key(mtx);
		customers.push({ id, customerDifficulty });
		key.unlock();
	}

	std::pair<int, int> handOverCustomer() 
	{
		std::unique_lock<std::mutex> key(mtx);
		if (!customers.empty()) {
			std::pair<int, int> customer = customers.front();
			customers.pop();
			return customer;
		}
		key.unlock();
		return { -1, -1 };
	}

	void serveCustomer(std::pair<int, int> customer)
	{
		std::unique_lock<std::mutex> key(coutMutex);
		std::cout << name << " обслужет клиента #" << customer.first << " через " << customer.second << " ms." << std::endl;
		key.unlock();

		std::this_thread::sleep_for(std::chrono::milliseconds(customer.second));
		
		key.lock();
		std::cout << name << " обслужил клиента #" << customer.first << std::endl;
		key.unlock();
	}
};

Trader trader1("Торговец 1");
Trader trader2("Торговец 2");
Trader trader3("Торговец 3");

void Trader::serveCustomers()
{
	while (true) {

		if (!customers.empty()) {
			serveCustomer(this->handOverCustomer());
		}
		else
		{
			std::pair<int, int> customer = { -1, -1 };

			customer = trader1.handOverCustomer();
			if (customer.second < 0) customer = trader2.handOverCustomer();
			if (customer.second < 0) customer = trader3.handOverCustomer();

			if (customer.second < 0) {
				break;
			}

			serveCustomer(customer);
		}
	}
}

int main() {

	for (int i = 1; i <= PEOPLE; ++i) {
		int rand = std::rand() % 300;
		std::cout << "Человек #" << i << " - " << rand;

		if (i <= PEOPLE / 3) {
			trader1.addCustomer(i, rand);
			std::cout << " - 1";
		}
		else if (i <= PEOPLE / 3 * 2) {
			trader2.addCustomer(i, rand);
			std::cout << " - 2";
		}
		else {
			trader3.addCustomer(i, rand);
			std::cout << " - 3";
		}

		std::cout << std::endl;
	}

	std::thread thread1(&Trader::serveCustomers, &trader1);
	std::thread thread2(&Trader::serveCustomers, &trader2);
	std::thread thread3(&Trader::serveCustomers, &trader3);

	thread1.join();
	thread2.join();
	thread3.join();

	return 0;
}
