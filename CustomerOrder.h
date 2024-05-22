#ifndef CUSTOMERORDER_H
#define CUSTOMERORDER_H

#include <string>
#include "Station.h"

namespace AssemblyLine
{
	class CustomerOrder
	{
		struct Item
		{
			std::string m_itemName;
			size_t m_serialNumber{ 0 };
			bool m_isFilled{ false };

			Item(const std::string& src) : m_itemName(src) {};
		};

		std::string m_name{};
		std::string m_product{};
		size_t m_cntItem{};
		Item** m_lstItem{};
		static size_t m_widthField;

	public:
		CustomerOrder() {};

		CustomerOrder(const std::string& line);

		CustomerOrder(const CustomerOrder& order);

		CustomerOrder& operator =(const CustomerOrder& order) = delete;

		CustomerOrder(CustomerOrder&& order) noexcept;

		CustomerOrder& operator =(CustomerOrder&& order) noexcept;

		~CustomerOrder();

		bool isOrderFilled() const;

		bool isItemFilled(const std::string& itemName) const;

		void fillItem(Station& station, std::ostream& os);

		void display(std::ostream& os) const;
	};
}

#endif 
