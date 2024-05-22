#include <sstream>

#include "Utilities.h"

using namespace std;

namespace AssemblyLine
{
	//Initializes static m_delimiter
	char Utilities::m_delimiter = '\0';

	//Set m_widthField = newWidth
	void Utilities:: setFieldWidth(size_t newWidth)
	{
		m_widthField = newWidth;
	}

	//Return m_widthField
	size_t Utilities::getFieldWidth() const
	{
		return m_widthField;
	}

	//Parse str and returns parsed string. Sets next_pos and more accordingly
	std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more)
	{
		string token;

		if (str[next_pos] != m_delimiter && more)
		{
			more = true;

			size_t nextDelimiterPos = str.find(m_delimiter, next_pos);
			//If delimiter isn't found anymore, next_pos is end of str
			if (nextDelimiterPos == string::npos)
			{
				nextDelimiterPos = str.length();
				//No more str to parse
				more = false;
			}
			token = str.substr(next_pos, nextDelimiterPos - next_pos);
			token = token.substr(token.find_first_not_of(' '));
			token = token.substr(0, token.find_last_not_of(' ') + 1);

			if (m_widthField < token.length())
			{
				m_widthField = token.length();
			}
			next_pos = nextDelimiterPos + 1;
		}
		else
		{
			more = false;
			throw "ERROR: No token.";
		}

		return token;
	}

	//Set m_delimiter to newDelimiter
	void Utilities::setDelimiter(char newDelimiter)
	{
		m_delimiter = newDelimiter;
	}

	//Return m_delimiter
	char Utilities::getDelimiter()
	{
		return m_delimiter;
	}
}