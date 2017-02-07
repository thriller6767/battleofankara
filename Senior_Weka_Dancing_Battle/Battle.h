#pragma once
#ifndef _BATTLE_H
#define _BATTLE_H

#include <cstdio>
#include <ctime>
#include <vector>
#include "Agent.h"
#include "Initial_val_mapper.h"
#include "RangeSearch.h"

#define BATTLEFIELD_Y_MAX 12000
#define BATTLEFIELD_X_MAX 9500
#define NEIGHBOR_RANGE 270
#define SHIT_NULL -100
#define HEIGHT_SCORE_INTERVAL 25
#define DIRECTION_STANDARD 50

#define SPECIAL_BONUS_TO_ATTACK 5
#define FATIGUE_INCREASE_IF_ATTACK 2
#define FATIGUE_INCREASE_IF_MOVE 1

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
	enum class Actions{IDLE, ATTACK, WITHDRAW, MOVE, RUNING_FOR_LIFE, MAKE_LAST_STAND, DISENGAGE, SHOOT};

	Initial_val_mapper ivm; // for test
	
	void one_battle(int offensive, int betray, int marching_from_constantinople, int is_water_poisoned, int increase_amount, int rounds);

	void initiate_battle(int poisoned_well, int marching_from_Constantinople, int increase_amount);
	void deleteAllAgent();
	
	/*-----range tree----------------*/
	void delete_searchTree();
	void put_rangetree_boundaries();
	void map_neighbor_and_enemies();

	/*--------print-----------------*/
	void initial_chart_print();

	/*---------just for test-----------*/
	void choose_and_Execute_Action(Agent * a, int offensive, int betray);

private:
	
	std::clock_t start;
	double duration;
	RangeSearch rsTree;
	ConReader cr;
	
	/*Agent still alive and in the battle field
	(alive_in_battle + left_battle + dead = n)
	*/
	int Ottoman_alive_in_battle, Tamerlane_alive_in_battle;

	/*Agent has already left the battle field*/
	int Ottoman_left_battle, Tamerlane_left_battle = 0;

	/*Agent are broken or treat 
	(broken_or_retreat + still_fighting_in_battle = n - dead
	*/
	int Ottoman_broken_or_retreat, Tamerlane_broken_or_retreat = 0;

	/*just for statistcs*/
	int Ottoman_fight_to_death, Tamerlane_fight_to_death = 0;

	/*-----------decide which action, and do it------------------*/

	void move_to_chosen_enemy(Agent *a, Agent * chosenEnemy);
	void running_for_life(Agent * a);
	void charge_newly_chosen_enemy(Agent *a, Agent * chosenEnemy);
	void attack_enemy(Agent *a, Agent * enemy);
	void move_to_built_in_dir(Agent *);
	void withdraw_to_built_in_dir(Agent *a);
	void shoot_the_enemy(Agent * a, Agent * enemy);

	/*--------three ways to choose enemy------------------*/
	Agent * chooseTheWeakestEnemy(Agent * a);
	Agent * find_the_closest_enemy(Agent *a);
	Agent * find_the_enemy_with_highest_HEIGHT_BONUS(Agent * a);
	Agent * find_the_enemy_with_SPECIAL_BONUS(Agent * a);
	Agent * find_enemy_in_neighbor(Agent * a);
	int has_special_bonus_against(Agent * a, Agent * enemy);
	int find_height_bonus_against(Agent * a, Agent * enemy);


	double distance_between_two_points(std::vector<int> A, std::vector<int> B);
	
	/* ------------- base on offensive/defensive attribute, decide to remain idle or move--------*/
	Battle::Actions idle_or_move(Agent *a, int defensive);
	
	/*-------choose a distance to move. follow the direction, and change the pos-----------*/
	int find_distance_to_move(Agent *a, Agent * enemy);	
	std::vector<int> find_new_pos_after_move(std::vector<int> pos, int distance, Agent::Direction dir, bool out_field_waiver);
	Agent::Direction find_new_dir_after_move(std::vector<int> pos, std::vector<int> enemy_pos);
	
	void move_to_default_enemy_direciton(Agent *a);

	/*There more more than 30% of this side agent in broken or retreat*/
	bool more_than_70percent_in_flight(int side);
	bool no_alive_agent_still_fighting(int side);

	
	void populate(int poisoned_well, int marching_from_Constantinople);
	void updateMorale_shootingR_sightR(Agent *a);
};


#endif