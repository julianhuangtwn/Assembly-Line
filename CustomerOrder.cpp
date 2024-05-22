#include <iomanip>

#include "CustomerOrder.h"
#include "Utilities.h"

using namespace std;

namespace AssemblyLine
{
	size_t CustomerOrder::m_widthField = 0;

	//Constructor that extracts line info
	CustomerOrder::CustomerOrder(const std::string& line)
	{
		Utilities ut;
		size_t pos = 0;
		bool more = true;
		string lineStr = line, itemName;
		m_name = ut.extractToken(lineStr, pos, more);
		m_product = ut.extractToken(lineStr, pos, more);
		if (m_widthField < m_product.size())
		{
			m_widthField = m_product.size();
		}
		for (size_t i = pos; i < lineStr.size(); i++)
		{
			if (lineStr[i] == ut.getDelimiter())
			{
				m_cntItem++;
			}
		}
		m_cntItem++;

		m_lstItem = new Item * [m_cntItem];
		for (size_t i = 0; i < m_cntItem; i++)
		{
			itemName = ut.extractToken(lineStr, pos, more);
			Item* item = new Item(itemName);
			m_lstItem[i] = item;
		}

		if (m_product.size() > ut.getFieldWidth())
		{
			ut.setFieldWidth(m_product.size());
		}
	}

	//Copying is now allowed
	CustomerOrder::CustomerOrder(const CustomerOrder& order)
	{
		throw "CustomerOrder copying is not allowed";
	}

	//Move constructor
	CustomerOrder::CustomerOrder(CustomerOrder&& order) noexcept
	{
		*this = move(order);
	}

	//Move operator
	CustomerOrder& CustomerOrder::operator =(CustomerOrder&& order) noexcept
	{
		if (this != &order)
		{
			m_name = order.m_name;
			m_product = order.m_product;
			m_widthField = order.m_widthField;

			if (m_lstItem)
			{
				for (size_t i = 0; i < m_cntItem; i++)
				{
					delete m_lstItem[i];
					m_lstItem[i] = nullptr;
				}
				delete[] m_lstItem;
			}
			m_cntItem = order.m_cntItem;
			order.m_cntItem = 0;
			m_lstItem = order.m_lstItem;
			
			order.m_lstItem = nullptr;
		}

		return *this;
	}

	//Destructor, each element is also dynamic
	CustomerOrder::~CustomerOrder()
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			delete m_lstItem[i];
		}
		delete[] m_lstItem;
	}

	//Returns if order is filled or not
	bool CustomerOrder::isOrderFilled() const
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (!m_lstItem[i]->m_isFilled)
			{
				return false;
			}
		}
		return true;
	}

	//Returns if item is filled or not
	bool CustomerOrder::isItemFilled(const std::string& itemName) const
	{
		for (size_t i = 0; i < m_cntItem; i++)
		{
			if (m_lstItem[i]->m_itemName == itemName)
			{
				if (!m_lstItem[i]->m_isFilled)
				{
					return false;
				}
			}
		}
		return true;
	}

	//Fills the item based on criteria and station given
	void CustomerOrder::fillItem(Station& station, std::ostream& os)
	{
		bool filled = false;

		for (size_t i = 0; i < m_cntItem && !filled; i++)
		{
			if (m_lstItem[i]->m_isFilled == false && m_lstItem[i]->m_itemName == station.getItemName())
			{
				if (station.getQuantity() != 0)
				{
					station.updateQuantity();
					m_lstItem[i]->m_isFilled = true;
					m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
					cout << "    Filled " << m_name << ", " << m_product << " [" << station.getItemName() << "]" << endl;
					filled = true;
				}
				else
				{
					cout << "    Unable to fill " << m_name << ", " << m_product << " [" << station.getItemName() << "]" << endl;
				}
			}
		}
	}

	void CustomerOrder::display(std::ostream& os) const
	{
		os << m_name << " - " << m_product << endl;
		for (size_t i = 0; i < m_cntItem; i++)
		{
			os << "[" << right << setw(6) << setfill('0') << m_lstItem[i]->m_serialNumber << "] "
				<< left << setw(m_widthField) << setfill(' ') << m_lstItem[i]->m_itemName << " - "
				<< ((m_lstItem[i] -> m_isFilled == true) ? "FILLED" : "TO BE FILLED") << endl;
		}
	}
}