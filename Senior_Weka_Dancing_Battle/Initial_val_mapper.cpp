#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <functional>
#include "Agent.h"
#include "Initial_val_mapper.h"
#include "Battle.h"

using namespace std;

void Initial_val_mapper::populate_battlefield(int poisoned_well, int marching_from_Constantinople, ConReader cr)
{
	initialVal_fileReader(poisoned_well, marching_from_Constantinople);
	setRanges(cr);
}

void Initial_val_mapper::fancyPrint()
{
	Agent * a;
		printf("Total number of agent is %d\n", AgentList.size());
		printf("\n---------------\nOttoman side has %d agents, they are: \n", Ottoman_last_index + 1);
		printf("________________________________________________________________________________________________\n");
		printf("NAME		CATEGORY	SIZE	MORALE	FATIGUE	   DIRECTION	AD	MD	RANGE	    ACCURACY	DEFEND	INDEX	POS		Sight_Range	Width	Depth\n");
		for (int i = 0; i <= Ottoman_last_index; ++i) {
			a = AgentList.at(i);
			(*a).print();
		}

		printf("\n\n---------------\nTamerlane side has %d agents, they are: \n", AgentList.size() -1- Ottoman_last_index);
		printf("________________________________________________________________________________________________\n");
		printf("NAME			CATEGORY	SIZE	MORALE	FATIGUE	   DIRECTION	AD	MD	RANGE	        ACCURACY	DEFEND	INDEX	POS		Sight_Range	Width	Depth\n");
		for (int i = Ottoman_last_index + 1; i < AgentList.size(); ++i) {
			a = AgentList.at(i);
			(*a).print();
		}

}

int Initial_val_mapper::initialVal_fileReader(int poisoned_well, int marching_from_Constantinople)
{
	ifstream inFile("basicData.txt");
	if (!inFile) {
		cerr << "File basicData.txt not found." << endl;
			return -1;
	}

	int count = 0;
	int index = 0;
	int side = Bayezid;

	string line, nam, cat, dir;
	int size, morale, fatigue, ad, md, defend, num_of_agent_for_this_type;
	double width, depth,shooting_r, accuracy;
	vector<int> pos = { OTTO_FIRST_LINE_START_X*SAMLE_POINT_DISTANCE, OTTO_FIRST_LINE_START_Y*SAMLE_POINT_DISTANCE };

	getline(inFile, line); //skip header

	while (getline(inFile, line)) {
		if (line.empty()) continue;
		stringstream line_stream(line);

		if (line_stream >> nam >> cat >> size >> morale >> fatigue >> dir >> ad >>
			md >> shooting_r >> accuracy >> defend) {

			if (nam == "TamurHorseArcher") {
				num_of_agent_for_this_type = size / TAMER_HORSE_ARCHER_AGENT_SIZE;
				size = TAMER_HORSE_ARCHER_AGENT_SIZE;
				width = sqrt(size * TAMER_HORSE_ARCHER_RATIO);
			}
			else if (nam == "Reserves") {
				num_of_agent_for_this_type = size / TAMER_RESERVES_AGENT_SIZE;
				size = TAMER_RESERVES_AGENT_SIZE;
				width = sqrt(size * TAMER_RESERVE_RATIO);
			}
			else if (nam == "Azaps") {
				num_of_agent_for_this_type = size / DEFAULT_AGENT_SIZE;
				size = DEFAULT_AGENT_SIZE;
				width = sqrt(size * LIGHT_INF_WIDTH_DEPTH_RATIO);
			}
			else {
				num_of_agent_for_this_type = size / DEFAULT_AGENT_SIZE;
				size = DEFAULT_AGENT_SIZE;
				width = sqrt(size * DEFAULT_WIDTH_DEPTH_RATIO);
			}

			depth = size / width;
			if (count == 1) { width *= LIGHT_INF_WIDTH; depth *= LIGHT_INF_DEPTH; }
			else if (count == 2 || count == 4) { width *= HEAVY_INF_WIDTH; depth *= HEAVY_INF_DEPTH; }
			else { width *= CAVALRY_WIDTH; depth *= CAVALRY_DEPTH; }

			if (count == 9) { side = Tamerlane; pos = { TAMERLANE_DEFAULT_START_X * SAMLE_POINT_DISTANCE, (int)(TAMERLANE_DEFAULT_START_Y * SAMLE_POINT_DISTANCE )}; }
			if (count == 10 || count == 11) pos = { TAMERLANE_SECOND_LINE_X * SAMLE_POINT_DISTANCE, TAMERLANE_SECOND_LINE_Y * SAMLE_POINT_DISTANCE };
			if (count == 12) pos = { TAMERLANE_RESERVE_X *SAMLE_POINT_DISTANCE, TAMERLANE_RESERVE_Y * SAMLE_POINT_DISTANCE };
			if (count >= 4 && count < 9) pos = { OTTO_FIRST_LINE_START_X * SAMLE_POINT_DISTANCE , (int)(OTTO_SECOND_LINE_START_Y*SAMLE_POINT_DISTANCE) };
			if (count == 1) pos = { (int)(AZAPS_START_X * SAMLE_POINT_DISTANCE), (int)(AZAPS_START_Y * SAMLE_POINT_DISTANCE) };
			if (count == 5) pos = { (int)(SERB_CAL_START_X * SAMLE_POINT_DISTANCE), (int)(SERB_CAL_START_Y * SAMLE_POINT_DISTANCE) };


			for (int i = 0; i < num_of_agent_for_this_type; ++i) {
				pos[0] += width + MINIMUM_GAPE;
				Agent* ag = Agent::Builder().initName(chooseName(count))
											.initCate(chooseCat(cat))
											.initSize(size)
											.initMorale(morale - poisoned_well)
											.initFatigue(fatigue + marching_from_Constantinople)
											.initDir(chooseDir(dir))
											.initAD(ad)
											.initMD(md)
											.initMissile_Range(shooting_r)
											.initAcc(accuracy)
											.initArmor(defend)
											.Side(side)
											.initPos(pos)
											.initSight(0)
											.initAgentIndex(index)
											.initAgentState(READY)
											.initMissileRangeRatio(shooting_r)
											.initWidth(width)
											.initDepth(depth)
											.initCurrentEenemy(-1)
											.initBetrayBoolean(false)
										    .initInitialMorale (morale - poisoned_well)
											.build();

				AgentList.push_back(ag);
				++index;

				if (count < 9) ++Ottoman_last_index;
			}
		}
		
		++count;
	}
	return 0;
}

void Initial_val_mapper::setRanges(ConReader contour)
{
	for (Agent * ag : AgentList) {
		//(*ag).print();
		(*ag).setShootingRange(contour);
		(*ag).setSightRange(contour);
		if ((*ag).is_standing_on_high_ground(contour)) (*ag).increaseAttackDamage(1.2);
	}

}


Agent::Name Initial_val_mapper::chooseName(int count)
{
	switch (count)
	{
	case 0:return Agent::Name::ANATOLIAN;
		case 1: return Agent::Name::AZAPS;
		case 2: return Agent::Name::JANISSARY;
		case 3: return Agent::Name::RUMELIAN;
		case 4: return Agent::Name::SERB_INF;
		case 5: return Agent::Name::SERB_CAL;
		case 6: return Agent::Name::KAPUKULU ;
		case 7: return Agent::Name::TARTAR;
		case 8: return Agent::Name::OTTO_RESERVES;
		case 9: return Agent::Name::TAMER_HORSE_ARCHER;
		case 10: return Agent::Name::TAMER_GAURDS;
		case 11: return Agent::Name::TAMERLANE;
		case 12: return Agent::Name::TAMER_RESERVES;

		default:
			return Agent::Name::TAMER_RESERVES;
	}
}

Agent::Direction Initial_val_mapper::chooseDir(std::string dir)
{
	if (dir == "S") return Agent::Direction::SOUTH;
	else if (dir == "N") return Agent::Direction::NORTH;
	else if (dir == "E") return Agent::Direction::EAST;
	else if (dir == "W") return Agent::Direction::WEST;
	else if (dir == "NE") return Agent::Direction::NE;
	else if (dir == "NW") return Agent::Direction::NW;
	else if (dir == "SE") return Agent::Direction::SE;
	else return Agent::Direction::SW;

}

Agent::Category Initial_val_mapper::chooseCat(std::string cat)
{
	if (cat == "HI") return Agent::Category::HEAVY_INF;
	else if (cat == "HC") return Agent::Category::HEAVY_CAL;
	else if (cat == "LI") return Agent::Category::LIGHT_INF;
	else if (cat == "LC") return Agent::Category::LIGHT_CAL;
	else return Agent::Category::WAR_ELEPHANT;
}

int Initial_val_mapper::agentNum(int totalSize, int sizePerAgent)
{
	return totalSize / sizePerAgent;
}

bool Initial_val_mapper::valueInRange(double value, double min, double max)
{
	return (value > min) && (value < max);
}

/*
Return true if two agents overlap 
*/
bool Initial_val_mapper::are_two_agents_overlap(Agent * a, Agent * b)
{
	double a_y2, a_x1, b_x1, b_y2;

	double a_x0 = (*a).getPos().at(0);
	double a_y0 = (*a).getPos().at(1);
	double b_x0 = (*b).getPos().at(0);
	double b_y0 = (*b).getPos().at(1);

	if ((*a).getDirection() == Agent::Direction::NE || (*a).getDirection() == Agent::Direction::SW) {	
		a_x1 = a_x0 + (*a).getWidth() / sqrt(2);
		a_x0 -= (*a).getDepth() / sqrt(2);
		a_y2 = a_y0 + (*a).getDepth() / sqrt(2) + (*a).getWidth() / sqrt(2);
	}
	else if ((*a).getDirection() == Agent::Direction::NW || (*a).getDirection() == Agent::Direction::SE) {
		a_y2 = a_y0 - (*a).getWidth() / sqrt(2);
		a_y0 += (*a).getDepth() / sqrt(2);
		a_x1 = a_x0 + (*a).getDepth() / sqrt(2) + (*a).getWidth() / sqrt(2);
	}
	else {
		a_y2 = a_y0 + (*a).getDepth();
		a_x1 = a_x0 + (*a).getWidth();
	}

	if ((*b).getDirection() == Agent::Direction::NE || (*b).getDirection() == Agent::Direction::SW) {
		b_x1 = b_x0 + (*b).getWidth() / sqrt(2);
		b_x0 -= (*b).getDepth() / sqrt(2);
		b_y2 = b_y0 + (*b).getDepth() / sqrt(2) + (*b).getWidth() / sqrt(2);
	}
	else if ((*b).getDirection() == Agent::Direction::NW || (*b).getDirection() == Agent::Direction::SE) {
		b_y2 = b_y0 - (*b).getWidth() / sqrt(2);
		b_y0 += (*b).getDepth() / sqrt(2);
		b_x1 = b_x0 + (*b).getDepth() / sqrt(2) + (*b).getWidth() / sqrt(2);
	}
	else {
		b_y2 = b_y0 + (*b).getDepth();
		b_x1 = b_x0 + (*b).getWidth();
	}


	bool xOverlap = valueInRange(a_x0, b_x0, b_x1 )
		|| valueInRange(b_x0, a_x0, a_x1);

	bool yOverlap = valueInRange(a_y0, b_y0, b_y2 )
		|| valueInRange(b_y0, a_y0, a_y2);

	return xOverlap && yOverlap;
				
}

bool Initial_val_mapper::is_thos_agent_overlap_with_any_other(Agent * target)
{
	bool toReturn = false;
	for (Agent* agent : AgentList) { if (are_two_agents_overlap(agent, target)) toReturn = true; }
	return toReturn;
}

bool Initial_val_mapper::is_overlap()
{
	return false;
}


void Initial_val_mapper::deleteAllAgent()
{
	for (Agent* agent : AgentList) { delete agent; }
}

void Initial_val_mapper::printOneUnit()
{
	printf("Pos of first agent is %d", (*AgentList.front()).getPos()[0]);
}


