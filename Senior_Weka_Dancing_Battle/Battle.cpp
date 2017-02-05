#include "Battle.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <functional>
#include "..\..\Senior_Weka_Dancing_Battle\Senior_Weka_Dancing_Battle\Agent.h"
#include "Initial_val_mapper.h"

using namespace std;

void Battle::put_rangetree_boundaries()
{
	int total_size = ivm.AgentList.size();
	int center = total_size / 2;

	for (int i = center; i >= 0; --i) {
		Agent *a = ivm.AgentList.at(i);
		rsTree.put(a, (*a).getPos().at(0), (*a).getPos().at(1));
	}
	for (int i = center + 1; i < total_size; ++i)
	{
		Agent *a = ivm.AgentList.at(i);
		rsTree.put(a, (*a).getPos().at(0), (*a).getPos().at(1));
	}	
}

void Battle::populate(int poisoned_well, int marching_from_Constantinople)
{
	cr.loadAltitude();
	ivm.populate_battlefield(poisoned_well, marching_from_Constantinople, cr);
}

/*
When choose to move/attack/run for life, this agent has already updated direction and position
Here properties need to update:
	Size
	Morale
	State
	Shooting_range
	Sight_range
	clear neighbor and enemy list
	free the tree
	neighbors (rebuild the tree)
	enemies

*/
void Battle::updateAgent_otherProperties(Agent * a)
{

}

/*Based on current situation, choose among IDLE, MOVE, ATTACK, RETREAT
  If defensive = 1; Ottoman will take the defensive position until ALL agent can see some enemies
*/
void Battle::choose_and_Execute_Action(Agent * a, int defensive){
	int current_state = (*a).getAgentState();

	switch (current_state) {
	case READY: 
		
		//If no enemy in sight range, MOVE if offensive, IDLE if defensive.
		if ((*a).getEnemies().empty()) {

			if (idle_or_move(a, defensive) == Battle::Actions::IDLE) {} //no thing to change.
			else {
				//move to built-in direction.
				move_to_built_in_dir(a);}
		}
		else {
			Agent * e_in_neighbor = find_enemy_in_neighbor(a);

			//if there is an enemy in neighbor range, attack
			if (e_in_neighbor != nullptr) {

				//move to it

				//attack

				//change state to ENGAGED
				(*a).changeAgentState(ENGAGED);

			}
			else {
				double shooting_range = (*a).getMissileRange();
				Agent *enemy = chooseTheWeakestEnemy(a);
				vector<int> enemy_pos = (*enemy).getPos();

				// if the chosen enemy is in shooting range
				if (distance_between_two_points(enemy_pos, (*a).getPos()) <= shooting_range) {
					
					//shoot this enemy

					
				}
				else {
					// else we need to range search enemies in shooting range
					rsTree.findAgent_within_range(a, shooting_range, enemies_in_shooting_search);
					
					int random = rand() % ((*a).getEnemies_in_missile().size()); // randomly pick one
					Agent *enemy_to_shoot = (*a).getEnemies_in_missile()[random];

					//shoot the enemy

				}

				//and move to it.
				move_to_chosen_enemy(a, enemy);
			}



		}
		break;
	case ENGAGED:
		break;
	case BROKEN: 
		break;
	case FIGHT_TO_DEATH: 
		break;
	case RETREAT: 
		break;
	}
}




/*
If this enemy choose to move, move to the chosen enemy's direction with a random distance 
(it should be the same pace with its neighbors, BUT so far still not know how to do
*/
void Battle::move_to_chosen_enemy(Agent * a, Agent * chosenEnemy)
{
	//If this agent cannot see any enemy within sight range
	if (chosenEnemy == nullptr) {
		
	}
	else {
		Agent::Direction dir_after_move = find_new_dir_after_move((*a).getPos(), (*chosenEnemy).getPos());
		int distance = find_distance_to_move(a, chosenEnemy);
		//Have not consider the problem of overlapping
		vector<int> newPos = find_new_pos_after_move((*a).getPos(), distance, dir_after_move);
		
		(*a).changeDirection(dir_after_move);
		(*a).changePos(newPos);
	}
}

/*
If this agent decides to attack (pre-condition: enemy is in neighbor range)
 
*/
void Battle::attack_chosen_enemy(Agent * a, Agent * enemy)
{
	// If this agent is already attacking someOne
	(*a).setCurrentEnemyIndex((*enemy).getIndex());
}

void Battle::move_to_built_in_dir(Agent * a)
{
	Agent::Direction dir;
	int distance = find_distance_to_move(a, nullptr);

	if ((*a).getSide() == Bayezid) dir = Agent::Direction::SOUTH;	
	else dir = Agent::Direction::NORTH;

	(*a).changeDirection(dir);
	(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, dir));
}

/* Pre-requirement: this agent decided to retreat.
If it is a Otoman agent, retreat to north.
If it is a bayezid agent, retreat to south
*/
void Battle::retreat_to_built_in_dir(Agent * a)
{
	Agent::Direction dir;
	int distance = find_distance_to_move(a, nullptr);

	if ((*a).getSide() == Bayezid) dir = Agent::Direction::NORTH;
	else dir = Agent::Direction::SOUTH;

	(*a).changeDirection(dir);
	(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, dir));
}

/*
If this agent can not see an enemy, return nullptr
Else return the enemy chosen by a random advantage type.
*/
Agent * Battle::chooseTheWeakestEnemy(Agent * a)
{
	if ((*a).getEnemies().size() == 0) return nullptr;
	else {
		int random = rand() % 3;
		Agent *chosenEnemy;

		//0 --> choose the closest enemy
		if (random == 0) chosenEnemy = find_the_closest_enemy(a);
		//1 --> choose the height bonus
		else if (random == 1) chosenEnemy = find_the_enemy_with_highest_HEIGHT_BONUS(a);
		//2--> choose the special bonus 
		else {

			chosenEnemy = find_the_enemy_with_SPECIAL_BONUS(a);

			//if this agent does not have special bonus on any of the enemies
			if (chosenEnemy == nullptr) {
				//roll the dice again
				random = rand() % 2;

				if (random == 0) chosenEnemy = find_the_closest_enemy(a);
				else chosenEnemy = find_the_enemy_with_highest_HEIGHT_BONUS(a);
			}
		}
		return chosenEnemy;
	}
}

/*
Return the closest enemy
*/
Agent * Battle::find_the_closest_enemy(Agent * a)
{
	double mindis = distance_between_two_points((*a).getPos(), (*(*a).getEnemies()[0]).getPos());
	double dis;
	Agent * closestEnemy = (*a).getEnemies()[0];

	for (Agent *enemy : (*a).getEnemies()) {			
		dis = distance_between_two_points((*a).getPos(), (*enemy).getPos());

		if (dis < mindis) {
			mindis = dis;
			closestEnemy = enemy;
		}
	}

	return closestEnemy;
	
}

/*
Find the enemy with the highest height bonus.
*/
Agent * Battle::find_the_enemy_with_highest_HEIGHT_BONUS(Agent * a)
{
	int maxHeightB = find_height_bonus_against(a, (*a).getEnemies()[0]);
	int heightB;
	Agent * toReturn = (*a).getEnemies()[0];

	for (Agent * enemy : (*a).getEnemies()) {
		heightB = find_height_bonus_against(a, enemy);

		if (heightB > maxHeightB) {
			maxHeightB = heightB;
			toReturn = enemy;
		}		}
	return toReturn;
	
}

/*
If this agent does not have enemies or does not have an enemy with special bonus, return nullptr
Else return the random enemy with special bonus;
*/
Agent * Battle::find_the_enemy_with_SPECIAL_BONUS(Agent * a)
{
	vector<Agent *> enemies_with_special_bonus;
	vector<Agent *> enemies_running_away;

	for (Agent * enemy : (*a).getEnemies()) {
		if (has_special_bonus_against(a, enemy) != 0) {
			enemies_with_special_bonus.push_back(enemy);
		}
		if ((*enemy).getAgentState() == BROKEN || (*enemy).getAgentState() == RETREAT) {
			enemies_running_away.push_back(enemy);
		}
	}

	if (enemies_with_special_bonus.size() == 0 && enemies_running_away.size() == 0) return nullptr;
	else {
		int random = rand() % 2;
		//choose the enemy who is running for its life
		if (random == 0 && enemies_running_away.size() != 0) {
			random = rand() % (enemies_running_away.size());
			return enemies_running_away[random];
		}
		// choose the enemy with special bonus
		else {
			random = rand() % (enemies_with_special_bonus.size());
			return enemies_with_special_bonus[random];
		}
	}
	
}

/*If there is enemy in neighbor, return the first one found
Else return nullptr*/
Agent * Battle::find_enemy_in_neighbor(Agent * a)
{
	for (Agent *n : (*a).getNeighbor()) {
		if ((*a).getSide() != (*n).getSide()) return n;
	}
	return nullptr;
}

/*Return 1 if Agent a has special Bonus against enemy, Otherwise return 0
*/
int Battle::has_special_bonus_against(Agent * a, Agent * enemy)
{
	if ((*a).getCategory() == Agent::Category::HEAVY_INF && (*enemy).getCategory() == Agent::Category::HEAVY_CAL) {
		return 1;
	}
	else if ((*a).getCategory() == Agent::Category::HEAVY_CAL && (*enemy).getCategory() == Agent::Category::LIGHT_INF) {
		return 1;
	}
	else if ((*a).getCategory() == Agent::Category::LIGHT_INF && (*enemy).getCategory() == Agent::Category::LIGHT_CAL) {
		return 1;
	}
	else if ((*a).getCategory() == Agent::Category::LIGHT_CAL && (*enemy).getCategory() == Agent::Category::HEAVY_INF) {
		return 1;
	}
	else if ((*a).getCategory() == Agent::Category::WAR_ELEPHANT) return 1;
	else return 0;

}

/*
Return a height bonus score of agnet A against enemy E. If A and E are at same altitude, score is 0. If A is lower than E, score is a negative number;
  If A is higher, score is evaluated as (A_height - E_height)/25.
*/
int Battle::find_height_bonus_against(Agent * a, Agent * enemy)
{
	double a_height = (*a).getHeight(cr);
	double enemy_height = (*enemy).getHeight(cr);

	return (a_height - enemy_height) / HEIGHT_SCORE_INTERVAL;

}

/*
Simply calculate the distance between two points with given x-y coordinates.
*/
double Battle::distance_between_two_points(std::vector<int> A, std::vector<int> B)
{
	if (A.empty() || B.empty()) printf("EMPTY VECTOR**************\n");
	else return sqrt(pow(B[0] - A[0], 2) + pow(B[1] - A[1], 2));
}

/*
IF This agent's state is BROKEN,
	all abilities to attack, defend and shoot are disabled; 
	this agent will move in the opposite direction from the closest enemy.
*/
void Battle::running_for_life(Agent * a)
{
	Agent * closestEnemy = find_the_closest_enemy(a);
	Agent::Direction direction_toward_enemy = find_new_dir_after_move((*a).getPos(), (*closestEnemy).getPos());
	
	int distance = 100; //Dont know how to handle
	Agent::Direction direction_from;
	switch (direction_toward_enemy) {
		case Agent::Direction::NORTH: direction_from = Agent::Direction::SOUTH; break;
		case Agent::Direction::SOUTH: direction_from = Agent::Direction::NORTH; break;
		case Agent::Direction::WEST: direction_from = Agent::Direction::EAST; break;
		case Agent::Direction::EAST: direction_from = Agent::Direction::WEST; break;
		case Agent::Direction::NE: direction_from = Agent::Direction::SW; break;
		case Agent::Direction::SW: direction_from = Agent::Direction::NE; break;
		case Agent::Direction::NW: direction_from = Agent::Direction::SE; break;
		default: direction_from = Agent::Direction::NW; break;
	}

	(*a).changeDirection(direction_from);
	(*a).disableAttackAbility();
	(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, direction_from));
}

/*
Return the direction at which this agent should face after moving toward the enemy
*/
Agent::Direction Battle::find_new_dir_after_move(std::vector<int> pos, std::vector<int> enemy_pos)
{
	if (enemy_pos[0] <= pos[0] + DIRECTION_STANDARD || enemy_pos[0] >= pos[0] - DIRECTION_STANDARD) {

		if (enemy_pos[1] <= pos[1]) return Agent::Direction::NORTH;
		else return Agent::Direction::SOUTH;
	}
	else if (enemy_pos[1] <= pos[1] + DIRECTION_STANDARD || enemy_pos[1] >= pos[1] - DIRECTION_STANDARD) {
		if (enemy_pos[0] <= pos[0]) return Agent::Direction::WEST;
		else return Agent::Direction::EAST;
	}
	else if (enemy_pos[0] < pos[0] - DIRECTION_STANDARD && enemy_pos[1] < pos[1] - DIRECTION_STANDARD) {
		return Agent::Direction::NW;
	}
	else if (enemy_pos[0] > pos[0] + DIRECTION_STANDARD && enemy_pos[1] > pos[1] + DIRECTION_STANDARD) {
		return Agent::Direction::SE;
	}
	else if (enemy_pos[0] < pos[0] - DIRECTION_STANDARD && enemy_pos[1] > pos[1] + DIRECTION_STANDARD) {
		return Agent::Direction::SW;
	}
	else return Agent::Direction::NE;
}

/*
pos: current position
distance: distance to move
dir: direction to move
*/
std::vector<int> Battle::find_new_pos_after_move(std::vector<int> pos, int distance, Agent::Direction dir)
{
	vector<int> newPos;
	newPos.push_back(pos[0]);
	newPos.push_back(pos[1]);

	if (dir == Agent::Direction::EAST) newPos[0] += distance;

	else if (dir == Agent::Direction::WEST) newPos[0] -= distance;
	
	else if (dir == Agent::Direction::NORTH) newPos[1] -= distance;

	else if (dir == Agent::Direction::SOUTH) newPos[1] += distance;

	else {
		int x_dis, y_dis = sqrt(pow(distance, 2) / 2);

		if (dir == Agent::Direction::NW) { newPos[0] -= x_dis; newPos[1] -= y_dis; }
		else if (dir == Agent::Direction::SE) { newPos[0] += x_dis; newPos[1] += y_dis; }
		else if (dir == Agent::Direction::NE) { newPos[0] -= x_dis; newPos[1] += y_dis; }
		else { newPos[0] += x_dis; newPos[1] -= y_dis; }
	}

	return newPos;	
}



void Battle::move_to_default_enemy_direciton(Agent * a)
{
	int distance = rand() % (NEIGHBOR_RANGE + 1) + 50;
	//Ottomans are supposed to move south
	if ((*a).getSide() == Bayezid) {
		(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, Agent::Direction::SOUTH));
	}
	//Tamerlane are supposed to move north
	else {
		(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, Agent::Direction::NORTH));
	}
}


Battle::Actions Battle::idle_or_move(Agent * a, int defensive)
{
	if (defensive == 1) {
		if ((*a).getSide() == Bayezid) { return Battle::Actions::IDLE; }
		else { return Battle::Actions::MOVE; }
	}
	else {
		if ((*a).getSide() == Bayezid) { return Battle::Actions::MOVE; }
		else { return Battle::Actions::IDLE; }
	}
}

int Battle::find_distance_to_move(Agent * a, Agent * chosenEnemy)
{
	int max_to_move, distance;
	if (chosenEnemy == nullptr) max_to_move = -1;
	else max_to_move = (int)distance_between_two_points((*a).getPos(), (*chosenEnemy).getPos());

	distance = rand() % (NEIGHBOR_RANGE + 1) + 50; //So it will be easier to check whether overlap 
	
	// If the distance between this agent and its enemy is smaller than the distance decide to move
	if (max_to_move != -1 && distance > max_to_move) { //I assume that max_to_move will not be smaller than 50
		distance = rand() % (max_to_move + 1) + 50;
	}
	return distance;

}

void Battle::deleteAllAgent()
{
	ivm.deleteAllAgent();
}

void Battle::initial_chart_print()
{
	ivm.fancyPrint();
}

void Battle::delete_searchTree()
{
	rsTree.~RangeSearch();
}
