#include "Workstation.h"

using namespace std;

namespace AssemblyLine
{
	std::deque<CustomerOrder> g_pending{};
	std::deque<CustomerOrder> g_completed{};
	std::deque<CustomerOrder> g_incomplete{};

	//Fill the order by calling fillItem on the CustomerOrder
	void Workstation::fill(std::ostream& os)
	{
		if (m_orders.size() > 0)
		{
			m_orders.front().fillItem(*this, os);
		}
	}

	//Attempt to move the current order to the next station, or if is the last station will move into completed or incomplete
	bool Workstation::attemptToMoveOrder()
	{
		if (m_orders.size() != 0)
		{
			if (getQuantity() == 0 || m_orders.front().isItemFilled(getItemName()))
			{
				if (m_pNextStation)
				{
					m_pNextStation->m_orders.push_back(move(m_orders.front()));
					m_orders.pop_front();
				}
				else
				{
					if (m_orders.front().isOrderFilled())
					{
						g_completed.push_back(move(m_orders.front()));
					}
					else
					{
						g_incomplete.push_back(move(m_orders.front()));
					}
					m_orders.pop_front();
				}

				return true;
			}
		}
		
		return false;
	}

	//Set the next station to station
	void Workstation::setNextStation(Workstation* station)
	{
		m_pNextStation = station;
	}

	//Return m_pNextStation
	Workstation* Workstation::getNextStation() const
	{
		return m_pNextStation;
	}

	//Display chain station info
	void Workstation::display(std::ostream& os) const
	{
		os << getItemName() << " --> " << (m_pNextStation ? m_pNextStation->getItemName() : "End of Line") << endl;
	}

	//Overloaded += that adds newOrder to the end of m_orders
	Workstation& Workstation::operator +=(CustomerOrder&& newOrder)
	{
		//push_back will move item if newOrder is rvalue
		m_orders.push_back(move(newOrder));

		return *this;
	}
}