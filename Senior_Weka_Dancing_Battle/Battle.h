#pragma once
#ifndef _BATTLE_H
#define _BATTLE_H

#include <cstdio>
#include <ctime>
#include <vector>
#include "Agent.h"
#include "Initial_val_mapper.h"
#include "RangeSearch.h"

#define NEIGHBOR_RANGE 270
#define SHIT_NULL -100
#define HEIGHT_SCORE_INTERVAL 25
#define DIRECTION_STANDARD 25

#define OTTOMAN_WITHDRAW_DIRECTION Agent::Direction::NORTH
#define TAMERLANE_WITHDRAW_DIRECTION Agent::Direction::SOUTH

/*The precondition of ATTACK is WITHIN ChargeRange. If within ChargeRange, more possibility to ATTACK,
  If an eney's status is ENGAGED, ATTACK.
  
  First Choice: choose between IDLE and any other. A agent is supposed to keep the same pace as its neighbor, so whether
				to remain IDLE or MOVE leave to 1) a random number 2) the neighbor's action
  Second Choice: if not IDLE, throw a random number to decide which is the weakest enemy (one of the three advantage types);
  Third Choice: choose between MOVE or ATTACK				 
*/
class Battle {
public:
	/*IDLE means not moving at all*/
	/*WITHDRAW means that this agent will not intentionally attack the enemy,
		will not choose to move toward enemy's direction, and will move to a built-in direction*/
	/*RUNING_FOR_LIFE means running for life*/
	enum class Actions{IDLE, ATTACK, WITHDRAW, MOVE, RUNING_FOR_LIFE, MAKE_LAST_STAND, DISENGAGE};

	Initial_val_mapper ivm; // for test
	
	void add_Enemies_within_sight_range(double dis, Agent *, Agent *);
	std::vector<Battle::Actions> getPossibleMoves(Agent *a);
	
	std::vector<Agent *> getEnemies_within_shooting_range(Agent *a);

	void populate(int poisoned_well, int marching_from_Constantinople);
	void updateAgent_otherProperties(Agent *a);
	void updateBattleField();
	void deleteAllAgent();
	void initial_chart_print();
	void delete_searchTree();

	void map_Graph_and_Agent();


private:
	
	std::clock_t start;
	double duration;
	RangeSearch rsTree;
	ConReader cr;
	int OttomanStatus[5]; // READY, ENGAGED, BETRAY, BROKEN, RETREAT;
	int TamerlaneStatus[5];


	void insertionSort(Agent * thisAgent, std::vector<Agent *> enemies);
	
	int evaluation_function(Agent *a, Agent * enemy, int specialB, int heightB);//not used now
	
	void choose_and_Execute_Action(Agent * a);

	void move_to_chosen_enemy(Agent *a, Agent * chosenEnemy);
	void running_for_life_if_state_is_broken(Agent * a);
	void attack_chosen_enemy(Agent *a, Agent * chosenEnemy);

	Agent * chooseTheWeakestEnemy(Agent * a);
	Agent * find_the_closest_enemy(Agent *a);
	Agent * find_the_enemy_with_highest_HEIGHT_BONUS(Agent * a);
	Agent * find_the_enemy_with_SPECIAL_BONUS(Agent * a);
	int has_special_bonus_against(Agent * a, Agent * enemy);
	int find_height_bonus_against(Agent * a, Agent * enemy);

	double distance_between_two_points(std::vector<int> A, std::vector<int> B);
	int is_enemy_within_range(double dis, std::vector<int>A, std::vector<int> B, double sight_range_A, Agent::Direction dir_A);

	Agent::Direction find_direction_toward_enemy(std::vector<int> pos, std::vector<int> enemy_pos);
	std::vector<int> find_new_pos_after_move(std::vector<int> pos, int distance, Agent::Direction dir);

	void move_with_default_enemy_direciton(Agent *a);
	Agent * neighbor_is_enemy(Agent *a);

	bool is_surrounded(Agent *a);
};


#endif