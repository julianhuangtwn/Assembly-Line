#pragma once
#ifndef WORKSTATION_H
#define WORKSTATION_H

#include <deque>
#include <string>
#include "CustomerOrder.h"
#include "Station.h"

namespace AssemblyLine
{
	extern std::deque<CustomerOrder> g_pending;
	extern std::deque<CustomerOrder> g_completed;
	extern std::deque<CustomerOrder> g_incomplete;

	class Workstation : public Station
	{
		std::deque<CustomerOrder> m_orders{};
		Workstation* m_pNextStation{};

	public:
		Workstation(const Workstation& src) = delete;
		Workstation(Workstation&& src) = delete;
		Workstation& operator =(const Workstation& src) = delete;
		Workstation& operator =(Workstation&& src) = delete;

		//Constructor, calls Station constructor with line passed 
		Workstation(const std::string line) : Station(line) {};

		//Fill the order by calling fillItem on the CustomerOrder
		void fill(std::ostream& os);

		//Attempt to move the current order to the next station, or if is the last station will move into completed or incomplete
		bool attemptToMoveOrder();

		//Set the next station to station
		void setNextStation(Workstation* station = nullptr);

		//Return m_pNextStation
		Workstation* getNextStation() const;

		//Display chain station info
		void display(std::ostream& os) const;

		//Overloaded += that adds newOrder to the end of m_orders
		Workstation& operator +=(CustomerOrder&& newOrder);
	};
}

#endif 
