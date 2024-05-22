#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <string>

namespace AssemblyLine
{
	class Utilities
	{
		size_t m_widthField { 1 };
		static char m_delimiter;

	public:
		//Set m_widthField to newWidth
		void setFieldWidth(size_t newWidth);

		//Return m_widthField
		size_t getFieldWidth() const;

		//Parse str and returns parsed string. Sets next_pos and more accordingly
		std::string extractToken(const std::string& str, size_t& next_pos, bool& more);

		//Set m_delimiter to newDelimiter
		static void setDelimiter(char newDelimiter);

		//Return m_delimiter
		static char getDelimiter();
	};
}

#endif 

