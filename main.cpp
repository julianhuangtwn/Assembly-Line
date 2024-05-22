#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Station.h"
#include "Workstation.h"
#include "CustomerOrder.h"
#include "Utilities.h"
#include "LineManager.h"


template<typename T>
static void loadFromFile(const char*, std::vector<T>&);

template<typename T>
static void loadFromFile(const char*, std::vector<T*>&);

int main(int argc, char** argv)
{
	std::vector<AssemblyLine::Workstation*> theStations;
	std::vector<AssemblyLine::CustomerOrder> theOrders;

	try {
		AssemblyLine::Utilities::setDelimiter(',');
		::loadFromFile("Stations1.txt", theStations);
		AssemblyLine::Utilities::setDelimiter('|');
		::loadFromFile("Stations2.txt", theStations);

		std::cout << "========================================" << std::endl;
		std::cout << "=         Stations (summary)           =" << std::endl;
		std::cout << "========================================" << std::endl;
		for (const auto* station : theStations)
			station->Station::display(std::cout, false);
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=          Stations (full)             =" << std::endl;
		std::cout << "========================================" << std::endl;
		for (const auto* station : theStations)
			station->Station::display(std::cout, true);
		std::cout << std::endl << std::endl;

		//Select an object and verify all the functionality it working
		std::cout << "========================================" << std::endl;
		std::cout << "=          Manual Validation           =" << std::endl;
		std::cout << "========================================" << std::endl;
		std::cout << "getItemName(): " << theStations[0]->getItemName() << std::endl;
		std::cout << "getNextSerialNumber(): " << theStations[0]->getNextSerialNumber() << std::endl;
		std::cout << "getNextSerialNumber(): " << theStations[0]->getNextSerialNumber() << std::endl;
		std::cout << "getQuantity(): " << theStations[0]->getQuantity() << std::endl;
		theStations[0]->updateQuantity();
		std::cout << "getQuantity(): " << theStations[0]->getQuantity() << std::endl;
		std::cout << std::endl << std::endl;


		::loadFromFile<AssemblyLine::CustomerOrder>("CustomerOrders.txt", theOrders);

		std::cout << "========================================" << std::endl;
		std::cout << "=                Orders                =" << std::endl;
		std::cout << "========================================" << std::endl;
		for (auto& order : theOrders) {
			order.display(std::cout);
			AssemblyLine::g_pending.push_back(std::move(order));
		}
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=       Display Stations (loaded)      =" << std::endl;
		std::cout << "========================================" << std::endl;
		AssemblyLine::LineManager lm("AssemblyLine.txt", theStations);
		lm.display(std::cout);
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=      Display Stations (ordered)      =" << std::endl;
		std::cout << "========================================" << std::endl;
		lm.reorderStations();
		lm.display(std::cout);
		std::cout << std::endl << std::endl;

		std::cout << "========================================" << std::endl;
		std::cout << "=           Filling Orders             =" << std::endl;
		std::cout << "========================================" << std::endl;
		//run the assembly line until all orders processed
		while (!lm.run(std::cout));
		std::cout << std::endl << std::endl;
	}
	catch (const std::string& msg) {
		std::cerr << msg << '\n';
		std::exit(2);
	}

	std::cout << "========================================" << std::endl;
	std::cout << "=      Processed Orders (complete)     =" << std::endl;
	std::cout << "========================================" << std::endl;
	for (const auto& o : AssemblyLine::g_completed)
		o.display(std::cout);
	std::cout << std::endl << std::endl;

	std::cout << "========================================" << std::endl;
	std::cout << "=     Processed Orders (incomplete)    =" << std::endl;
	std::cout << "========================================" << std::endl;
	for (const auto& o : AssemblyLine::g_incomplete)
		o.display(std::cout);
	std::cout << std::endl << std::endl;

	std::cout << "========================================" << std::endl;
	std::cout << "=          Inventory (full)            =" << std::endl;
	std::cout << "========================================" << std::endl;
	for (const AssemblyLine::Station* theItem : theStations)
		theItem->display(std::cout, true);
	std::cout << std::endl;

	// cleanup
	for (auto station : theStations)
		delete station;

	return 0;
}

template<typename T>
static void loadFromFile(const char* filename, std::vector<T>& theCollection)
{
	if (!filename) {
		throw std::string("ERROR: No filename provided.");
	}
	std::ifstream file(filename);
	if (!file)
		throw std::string("Unable to open [") + filename + "] file.";

	std::string record;
	while (!file.eof())
	{
		std::getline(file, record);
		T elem(record);
		theCollection.push_back(std::move(elem));
	}

	file.close();
}

template<typename T>
static void loadFromFile(const char* filename, std::vector<T*>& theCollection)
{
	if (!filename) {
		throw std::string("ERROR: No filename provided.");
	}
	std::ifstream file(filename);
	if (!file)
		throw std::string("Unable to open [") + filename + "] file.";

	std::string record;
	while (!file.eof())
	{
		std::getline(file, record);
		theCollection.push_back(new T(record));
	}

	file.close();
}
