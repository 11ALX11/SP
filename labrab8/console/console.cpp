// Выполнение действий должно происходить в разных потоках.
// Все классы должны быть подключены, используя DLL.
// В каждом варианте должно быть использовано наследование 
// и должна быть дополнительная программа - клиент, написанная, используя WinAPI, 
// служащая для заполнения данных(основная программа - консольная).
// Создать симулятор больницы.
// Необходимые классы : разные виды врачей, пациенты.
// Заполняемые с помощью WinAPI данные : вся информация о врачах и пациентах.
// Суть работы : врачи посещают пациентов, за которыми закреплены и лечат определённые 
// симптомы в течение некоторого времени в зависимости от специальности.
// У врача есть максимальное время лечения, после которого он уходит к следующему пациенту.
// Два врача не могут одновременно обслуживать одного пациента.
// Как только пациент вылечен – он выписывается из больницы и врачи больше к нему не ходят.
// Программа завершается, когда все пациенты вылечены.

#include <iostream>
#include <vector>
#include <thread>

#include "HospitalLib.h"

int main() {
	std::vector<std::reference_wrapper<Patient>> patientsQueue;

	// Create adult patients
	std::vector<AdultPatient> adultPatients = { {"AdultPatient1", 4000}, {"AdultPatient2", 1000}, {"AdultPatient3", 7500} };
	for (auto& adultPatient : adultPatients) {
		patientsQueue.push_back(std::ref(adultPatient));
	}

	// Create child patients
	std::vector<ChildPatient> childPatients = { {"ChildPatient1", 3000}, {"ChildPatient2", 2000}, {"ChildPatient3", 5000} };
	for (auto& childPatient : childPatients) {
		patientsQueue.push_back(std::ref(childPatient));
	}

	std::vector<std::unique_ptr<Doctor>> doctors;
	doctors.push_back(std::make_unique<Doctor>("Doctor1", 2500));
	doctors.push_back(std::make_unique<AdultDoctor>("AdultDoctor2", 4000));
	doctors.push_back(std::make_unique<Pediatrician>("Pediatrician3", 3500));


	std::vector<std::thread> threads;

	for (auto& doctor : doctors) {
		threads.emplace_back(&Doctor::work, doctor.get(), std::ref(patientsQueue));
	}

	for (auto& thread : threads) {
		thread.join();
	}

	std::cout << "All patients have been treated. Simulation completed.\n";

	return 0;
}