#include <iomanip>

#include "Station.h"
#include "Utilities.h"

using namespace std;

namespace AssemblyLine
{
	//Initialize static variables
	size_t Station::m_widthField = 0;
	size_t Station::id_generator = 0;

	//Constructor, takes a line and parse all the fields with Utilities
	Station::Station(const std::string& line)
	{
		Utilities ut;
		size_t pos = 0;
		bool more = true;
		m_id = ++id_generator;
		m_name = ut.extractToken(line, pos, more);
		m_serial = stoul(ut.extractToken(line, pos, more));
		m_numOfItems = stoul(ut.extractToken(line, pos, more));

		m_widthField = (m_widthField < ut.getFieldWidth()) ? ut.getFieldWidth() : m_widthField;
		m_desc = ut.extractToken(line, pos, more);
	}

	//Return m_name
	const std::string& Station::getItemName() const
	{
		return m_name;
	}

	//Return next m_serial and increments it
	size_t Station::getNextSerialNumber()
	{
		return m_serial++;
	}

	//Return m_numOfitems
	size_t Station::getQuantity() const
	{
		return m_numOfItems;
	}

	//Decrease m_numOfItems by 1 less it goes below 0 
	void Station::updateQuantity()
	{
		((m_numOfItems - 1) >= 0) ? m_numOfItems-- : m_numOfItems;
	}

	//Display Station info
	void Station::display(std::ostream& os, bool full) const
	{
		os << right << setw(3) << setfill('0') << m_id << " | "
			<< left << setw(m_widthField) << setfill(' ') << m_name << " | "
			<< right << setw(6) << setfill('0') << m_serial << " | ";

		if (full)
		{
			os << right << setw(4) << setfill(' ') << m_numOfItems << " | "
				<< m_desc;
		}

		os << endl;
	}
}