#pragma once
#ifndef  _INITIAL_VAL_MAPPER_H
#define _INITIAL_VAL_MAPPER_H


#include <vector>
#include "Agent.h"

#define DEFAULT_AGENT_SIZE 1500
#define TAMER_HORSE_ARCHER_AGENT_SIZE 3000
#define TAMER_RESERVES_AGENT_SIZE 2000

#define INPUT_FILE_COLUMN_LENGTH 11
#define OTTO_FIRST_LINE_START_Y 15
#define OTTO_FIRST_LINE_START_X 2
#define AZAPS_START_Y 15.5
#define AZAPS_START_X 4
#define OTTO_SECOND_LINE_START_Y 15
#define SERB_CAL_START_Y 15.2
#define SERB_CAL_START_X 1.5
#define TARTAR_X 13
#define RESERV_Y 14.7

#define TAMERLANE_DEFAULT_START_Y 22
#define TAMERLANE_DEFAULT_START_X 2

#define TAMERLANE_SECOND_LINE_Y 22.5
#define TAMERLANE_SECOND_LINE_X 3
#define TAMERLANE_RESERVE_Y 23
#define TAMERLANE_RESERVE_X 5

class Initial_val_mapper {
public:

	void remapping(int poisoned_well, int marching_from_Constantinople, bool betray,ConReader cr);

	void populate_battlefield(int poisoned_well, int marching_from_Constantinople, bool betray, int size_increase_Ottoman,  ConReader cr);
	void fancyPrint();

	void deleteAllAgent();
	void printOneUnit();


	int Ottoman_last_index = 0;
	std::vector<Agent*> AgentList;

private:

	int remapper(int poisoned_well, int marching_from_Constantinople, bool betray);
	int initialVal_fileReader(int poisoned_well, int marching_from_Constantinople, bool betray, int size_increase_Ottoman);

	void setRanges(ConReader contour);

	Agent::Direction chooseDir(std::string dir);
	Agent::Category chooseCat(std::string cat);
	int agentNum(int totalSize, int sizePerAgent);

	bool valueInRange(double, double, double);
	Agent::Name chooseName(int count);

	template <typename Enumeration>
	auto enum_to_int(Enumeration const value);

};





#endif // ! _INITIAL_VAL_MAPPER_H

template<typename Enumeration>
inline auto Initial_val_mapper::enum_to_int(Enumeration const value)
{
	return static_cast<int>(value);
}
