#include <fstream>
#include <algorithm>

#include "LineManager.h"
#include "Utilities.h"

using namespace std;

namespace AssemblyLine
{
	//Constructor that gets from a file a chain of station names
	//Puts each station in the collection and finds which station is the first
	LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations)
	{
		string line, currWS, nextWS, lastItem;
		size_t delimPos;
		bool stationMatched = false;
		ifstream fileW(file);
		if (fileW)
		{
			while (getline(fileW, line, '\n'))
			{
				delimPos = line.find('|');

				currWS = line.substr(0, delimPos);
				if (delimPos != string::npos)
				{
					nextWS = line.substr(delimPos + 1);
				}
				else
				{
					nextWS = "";
					lastItem = currWS;
				}

				for_each(stations.begin(), stations.end(), [this, &currWS, &nextWS, &stations, &stationMatched](Workstation* station) {
					if (station->getItemName() == currWS)
					{
						stationMatched = true;
						if (nextWS != "")
						{
							auto nextWSPtr = find_if(stations.begin(), stations.end(), [&](Workstation* nextStation) {
								return (nextStation->getItemName() == nextWS);
								});

							if (nextWSPtr != stations.end())
							{
								station->setNextStation(*nextWSPtr);
							}
						}
						m_activeLine.push_back(station);
					}
				});
				m_cntCustomerOrder = g_pending.size();
			}

			for_each(m_activeLine.begin(), m_activeLine.end(), [this](Workstation* station) {
				auto matchedNext = find_if(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* otherStation) {
					if (otherStation->getNextStation())
					{
						return otherStation->getNextStation()->getItemName() == station->getItemName();
					}
					else
					{
						return false;
					}
					});
				if (matchedNext == m_activeLine.end())
				{
					m_firstStation = station;
				}
			});

			fileW.close();
		}
		else
		{
			throw "File couldn't be opened";
		}
	}

	//Sorts the stations based on their chain order (m_pNextStation)
	void LineManager::reorderStations()
	{
		size_t nextStationPos, actualFirstItemPos;

		for (size_t i = 0; i < m_activeLine.size(); i++)
		{
			if (m_activeLine[i]->getItemName() == m_firstStation->getItemName())
			{
				actualFirstItemPos = i;
			}
		};

		swap(m_activeLine[0], m_activeLine[actualFirstItemPos]);

		for (size_t i = 0; i < m_activeLine.size() - 1; i++)
		{
			for (size_t j = 1; j < m_activeLine.size(); j++)
			{
				if (m_activeLine[i]->getNextStation())
				{
					if ((m_activeLine[i]->getNextStation()->getItemName()) == (m_activeLine[j]->getItemName()))
					{
						nextStationPos = j;
						swap(m_activeLine[i + 1], m_activeLine[nextStationPos]);
					}
				}
			}

			nextStationPos = -1;
		}
	}

	//Run the production line, fills all stations and then move everything to the next position
	bool LineManager::run(std::ostream& os)
	{
		static size_t count = 0;

		os << "Line Manager Iteration: " << count + 1 << endl;
		if (!g_pending.empty())
		{
			*m_firstStation += move(g_pending.front());
			g_pending.pop_front();
		}
		for (size_t i = 0; i < m_activeLine.size(); i++)
		{
			m_activeLine[i]->fill(os);
		}
		for (size_t i = 0; i < m_activeLine.size(); i++)
		{
			m_activeLine[i]->attemptToMoveOrder();
		}
		count++;

		return (g_completed.size() + g_incomplete.size()) == m_cntCustomerOrder;
	}

	//Display each station info
	void LineManager::display(std::ostream& os) const
	{
		for_each(m_activeLine.begin(), m_activeLine.end(), [&os](Workstation* station) {
			station->display(os);
			});
	}
}