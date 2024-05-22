#pragma once
#ifndef LINEMANAGER_H
#define LINEMANAGER_H

#include <vector>
#include <string>
#include "Workstation.h"

namespace AssemblyLine
{
	class LineManager
	{
		std::vector<Workstation*> m_activeLine{};
		size_t m_cntCustomerOrder{};
		Workstation* m_firstStation{};

	public:
		//Constructor that gets from a file a chain of station names
	//Puts each station in the collection and finds which station is the first
		LineManager(const std::string& file, const std::vector<Workstation*>& stations);
	
		//Sorts the stations based on their chain order (m_pNextStation)
		void reorderStations();

		//Run the production line, fills all stations and then move everything to the next position
		bool run(std::ostream& os);

		//Display each station info
		void display(std::ostream& os) const;
	};
}

#endif 
