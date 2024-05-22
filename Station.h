#pragma once
#ifndef STATION_H
#define STATION_H

#include <string>
#include <iostream>

namespace AssemblyLine
{
	class Station
	{
		//Class variables shared by all Stations
		static size_t m_widthField;
		static size_t id_generator;

		int m_id{};
		std::string m_name{};
		std::string m_desc{};
		size_t m_serial{};
		size_t m_numOfItems{};
		
	public:
		//Constructor, takes a line and parse all the fields with Utilities
		Station(const std::string& line);

		//Return m_name
		const std::string& getItemName() const;

		//Return next m_serial and increments it
		size_t getNextSerialNumber();

		//Return m_numOfitems
		size_t getQuantity() const;

		//Decrease m_numOfItems by 1 less it goes below 0 
		void updateQuantity();

		//Display Station info
		void display(std::ostream& os, bool full) const;
	};
}

#endif
