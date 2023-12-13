#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

//A good tutorial: https://learn.microsoft.com/en-us/cpp/build/walkthrough-creating-and-using-a-dynamic-link-library-cpp?view=msvc-170
#ifdef MAINLIBRARY_EXPORTS
#define HOSPITALLIB_API __declspec(dllexport)
#else
#define HOSPITALLIB_API __declspec(dllimport)
#endif

std::mutex coutMutex;
std::mutex patientsMutex;

extern "C++" HOSPITALLIB_API class Patient {
public:
	std::string name;
	int treatmentTime;
	bool isTreated;

	enum class PatientType {
		Base,
		Adult,
		Child
	};

	Patient(const std::string& name = "", const int treatmentTime = 500)
		: name(name), treatmentTime(treatmentTime), isTreated(false) {}

	void receiveTreatment(int time) {
		std::unique_lock<std::mutex> coutLock(coutMutex);
		std::cout << "Patient " << name << " is receiving treatment (" << treatmentTime << " left).\n\n";
		coutLock.unlock();
		std::this_thread::sleep_for(std::chrono::milliseconds(time));
		treatmentTime -= time;
		coutLock.lock();
		if (treatmentTime <= 0) {
			isTreated = true;
			std::cout << "Patient " << name << " has been successfully treated.\n";
		}
		else {
			std::cout << "Patient " << name << " need additional attention (" << treatmentTime << " left).\n";
		}
		coutLock.unlock();
	}

	virtual PatientType getType() const {
		return PatientType::Base;
	}
};

extern "C++" HOSPITALLIB_API class AdultPatient : public Patient {
public:
	AdultPatient(const std::string& name = "", const int treatmentTime = 500)
		: Patient(name, treatmentTime) {}

	virtual PatientType getType() const {
		return PatientType::Adult;
	}
};
extern "C++" HOSPITALLIB_API class ChildPatient : public Patient {
public:
	ChildPatient(const std::string& name = "", const int treatmentTime = 500)
		: Patient(name, treatmentTime) {}

	virtual PatientType getType() const {
		return PatientType::Child;
	}
};

extern "C++" HOSPITALLIB_API class Doctor {
public:
	std::string name;
	int maxTreatmentTime;

	Doctor(const std::string& name = "", int maxTreatmentTime = 300) : name(name), maxTreatmentTime(maxTreatmentTime) {}

	void treatPatient(Patient& patient) {
		std::unique_lock<std::mutex> coutLock(coutMutex);
		std::cout << "Doctor " << name << " is treating patient " << patient.name << ".\n";
		coutLock.unlock();
		patient.receiveTreatment((((maxTreatmentTime) < (patient.treatmentTime)) ? (maxTreatmentTime) : (patient.treatmentTime)));
		coutLock.lock();
		std::cout << "Doctor " << name << " has finished treating patient " << patient.name << ".\n";
		coutLock.unlock();
	}

	void work(std::vector<std::reference_wrapper<Patient>>& patientsQueue) {
		while (true) {
			std::unique_lock<std::mutex> lock(patientsMutex);
			if (!patientsQueue.empty()) {
				Patient& patient = getNextPatient(patientsQueue);
				lock.unlock();

				if (patient.isTreated) {
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
					continue;
				}

				treatPatient(patient);

				if (!patient.isTreated) {
					patientsQueue.push_back(patient);
				}
			}
			else {
				lock.unlock();
				break;  // No more patients to treat
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(10));
		}
	}

	virtual Patient& getNextPatient(std::vector<std::reference_wrapper<Patient>>& patientsQueue) {
		if (!patientsQueue.empty()) {
			Patient& patient = patientsQueue.front().get();
			patientsQueue.erase(patientsQueue.begin());
			return patient;
		}
		// Return null if no patient is found
		static Patient nullPatient("", 0);
		nullPatient.isTreated = true;
		return nullPatient;
	}
};

extern "C++" HOSPITALLIB_API class AdultDoctor : public Doctor {
public:
	AdultDoctor(const std::string& name = "", int maxTreatmentTime = 300) : Doctor(name, maxTreatmentTime) {}

	Patient& getNextPatient(std::vector<std::reference_wrapper<Patient>>& patientsQueue) override {
		auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(),
			[](const std::reference_wrapper<Patient>& patient) {
				return patient.get().getType() == Patient::PatientType::Adult;
			});

		if (it != patientsQueue.end()) {
			Patient& patient = it->get();
			patientsQueue.erase(it);
			return patient;
		}
		// Return null if no patient is found
		static Patient nullPatient("", 0);
		nullPatient.isTreated = true;
		return nullPatient;
	}
};
extern "C++" HOSPITALLIB_API class Pediatrician : public Doctor {
public:
	Pediatrician(const std::string& name = "", int maxTreatmentTime = 300) : Doctor(name, maxTreatmentTime) {}

	Patient& getNextPatient(std::vector<std::reference_wrapper<Patient>>& patientsQueue) override {
		auto it = std::find_if(patientsQueue.begin(), patientsQueue.end(),
			[](const std::reference_wrapper<Patient>& patient) {
				return patient.get().getType() == Patient::PatientType::Child;
			});

		if (it != patientsQueue.end()) {
			Patient& patient = it->get();
			patientsQueue.erase(it);
			return patient;
		}
		// Return null if no patient is found
		static Patient nullPatient("", 0);
		nullPatient.isTreated = true;
		return nullPatient;
	}
};