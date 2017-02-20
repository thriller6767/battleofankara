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

void Battle::one_battle(std::ofstream& RESULTFILE, ofstream& file, ofstream & agentstat, ofstream& file3, int offensive, int betray, int marching_from_constantinople, int is_water_poisoned, int increase_amount, int rounds)
{
	start = time(0);
	initiate_battle(is_water_poisoned, marching_from_constantinople, increase_amount);
	
	/*-------------Write stats-------------------------------------*/
	int r = 0;
	for (Agent *a : ivm.AgentList) {
		if ((*a).getSide() == Bayezid) Ottoman_soldier += (*a).getInitialSize();
		else Tamerlane_soldier += (*a).getInitialSize();
		write_agent_stats(agentstat, r, a);
	}

	file << "side, rounds, Alive, Dead, In Battlefield, Left Battlefield, Broken, Retreat, Ready, Engaged, Fight to Death, \n";
	file << "Ottoman, " << r << "," << Ottoman_size - Ottoman_dead << "," << Ottoman_dead << "," << Ottoman_alive_in_battle << "," << Ottoman_left_battle << ","
			<< Ottoman_broken << "," << Ottoman_retreat << "," << Ottoman_ready << "," << Ottoman_still_fighting << ","
			<< Ottoman_fight_to_death << ", \n";

	file << "Tamerlane, " << r << "," << Tamerlane_size - Tamerlane_dead << "," << Tamerlane_dead << "," << Tamerlane_alive_in_battle << "," << Tamerlane_left_battle << ","
			<< Tamerlane_broken << "," << Tamerlane_retreat << "," << Tamerlane_ready << "," << Tamerlane_still_fighting << ","
			<< Tamerlane_fight_to_death << ", \n";

	file << "\n";

	++r;
	
	/*----------------------battle------------------------------------*/
	while (r <= rounds &&
		(!no_one_is_able_to_fight(Bayezid) && !no_one_is_able_to_fight(Tamerlane))) {

		file << "side, rounds, Alive, Dead, In Battlefield, Left Battlefield, Broken, Retreat, Ready, Engaged, Fight to Death, Casualty\n";

		printf("\nr is %d", r);

		Ottoman_still_fighting = 0, Ottoman_ready = 0, Tamerlane_ready = 0, Tamerlane_still_fighting = 0;
		int O_alive = 0, T_alive = 0;

		for (Agent * a : ivm.AgentList) {

			updateMorale_shootingR_sightR(a);

			if ((*a).getSide() == Bayezid) {
				if ((*a).is_alive && (*a).getAgentState() != DEAD)	O_alive += (*a).getSize();
			}
			if ((*a).getSide() == Tamerlane) {
				if ((*a).is_alive && (*a).getAgentState() != DEAD)	T_alive += (*a).getSize();
			}
		}

		put_rangetree_boundaries();

		rsTree.printSize();

		map_neighbor_and_enemies();

		for (Agent * a : ivm.AgentList) {
			if ((*a).getIndex() == 6) {
				printf(" size is %d, current enemy is %d, position is (%d, %d), state is %s\n", (*a).getSize(),
					(*a).getCurrentEnemyIndex(), (*a).getPos()[0], (*a).getPos()[1], (*a).printState().c_str());
				printf("#6's neighbor list size is %d***\n", (*a).getNeighbor().size());


				if ((*a).getCurrentEnemyIndex() != -1) {
					Agent * e = ivm.AgentList[(*a).getCurrentEnemyIndex()];
					printf(" ENEMY size is %d, current enemy is %d, position is (%d, %d), state is %s\n", (*e).getSize(),
						(*e).getCurrentEnemyIndex(), (*e).getPos()[0], (*e).getPos()[1], (*e).printState().c_str());
				}
			}
			choose_and_Execute_Action(a, offensive, betray,r );

			if ((*a).getIndex() == 6) {
				printf("after choose action, #6 is now at (%d, %d)\n", (*a).getPos()[0], (*a).getPos()[1]);
			}

			write_agent_stats(agentstat, r, a);

			if ((*a).getSide() == Bayezid && (*a).getAgentState() == ENGAGED) ++Ottoman_still_fighting;
			else if ((*a).getSide() == Bayezid && (*a).getAgentState() == READY) ++Ottoman_ready;
			else if ((*a).getSide() == Tamerlane && (*a).getAgentState() == ENGAGED) ++Tamerlane_still_fighting;
			else if ((*a).getSide() == Tamerlane && (*a).getAgentState() == READY) ++Tamerlane_ready;
		}

		for (Agent * a : ivm.AgentList) {
			if ((*a).is_alive && (*a).in_battlefield && (*a).getAgentState() != DEAD) {
				(*a).clear_enemies();
				(*a).clear_enemies_to_shoot();
				(*a).clear_neighbor();
			}
		}

		delete_searchTree();

		Ottoman_dead = Ottoman_size - Ottoman_alive_in_battle - Ottoman_left_battle;
		Tamerlane_dead = Tamerlane_size - Tamerlane_alive_in_battle - Tamerlane_left_battle;

		if (file) {
			file << "Ottoman, " << r << "," << Ottoman_size - Ottoman_dead << "," << Ottoman_dead << "," << Ottoman_alive_in_battle << "," << Ottoman_left_battle << ","
				<< Ottoman_broken << "," << Ottoman_retreat << "," << Ottoman_ready << "," << Ottoman_still_fighting << ","
				<< Ottoman_fight_to_death << ",\n";

			file << "Tamerlane, " << r << "," << Tamerlane_size - Tamerlane_dead << "," << Tamerlane_dead << "," << Tamerlane_alive_in_battle << "," << Tamerlane_left_battle << ","
				<< Tamerlane_broken << "," << Tamerlane_retreat << "," << Tamerlane_ready << "," << Tamerlane_still_fighting << ","
				<< Tamerlane_fight_to_death <<   ", \n";

			file << "\n";

		}
		file3 <<r << ","<< (double)O_alive / (double)Ottoman_soldier << "," << (double)T_alive / (double)Tamerlane_soldier << ",\n";
		agentstat << "\n";
		++r;
	}

	RESULTFILE << r-1 << "," << rounds << ",";

	 if (no_one_is_able_to_fight(Bayezid))
	{
		RESULTFILE << "Bayezid LOST,";
		printf("Bayezid lost.");
	}
	else if (no_one_is_able_to_fight(Tamerlane)) {
		RESULTFILE << "Tamerlane LOST,";
		printf("Tamerlane lost");
	}
	else {
		RESULTFILE << "DRAW, ";
		printf("\n==========DRAW===================");
	}

	write_statistics(RESULTFILE, file, r, rounds);

	int seconds_since_start = difftime(time(0), start);
	printf("when finish, r is %d, time is %d seconds", r, seconds_since_start);
}


void Battle::put_rangetree_boundaries()
{
	int total_size = ivm.AgentList.size();
	int center = total_size / 2;

	for (int i = center; i >= 0; --i) {
		Agent *a = ivm.AgentList.at(i);
		if ((*a).is_alive && (*a).in_battlefield && (*a).getAgentState() != DEAD) {rsTree.put(a, (*a).getPos().at(0), (*a).getPos().at(1)); }
	}
	for (int i = center + 1; i < total_size; ++i)
	{
		Agent *a = ivm.AgentList.at(i);
		if ((*a).is_alive && (*a).in_battlefield && (*a).getAgentState() != DEAD) { rsTree.put(a, (*a).getPos().at(0), (*a).getPos().at(1)); }
	}	
}

void Battle::map_neighbor_and_enemies()
{
	//range search for neighbors
	for (Agent *a : ivm.AgentList) {
		if ((*a).is_alive && (*a).in_battlefield && (*a).getAgentState() != DEAD) {
			rsTree.findAgent_within_range(a, NEIGHBOR_RANGE, neighbor_search);
		}
		//(*a).print_neighbors();
	}
	//range search for enemies
	for (Agent *a : ivm.AgentList) {
		int sightrange = (*a).getSightRange();
		if ((*a).is_alive && (*a).in_battlefield && (*a).getAgentState() != DEAD) {
			rsTree.findAgent_within_range(a, sightrange, enemies_in_sight_search);
		}
		//(*a).print_enemies();
	}
}

void Battle::first_time_populate(int is_water_poisoned, int march_from_constantinople, int agent_increase, bool any_betrayal)
{
	ivm.populate_battlefield(is_water_poisoned, march_from_constantinople, any_betrayal, agent_increase , cr);
}

int Battle::simple_result_of_one_battle(std::ofstream& RESULTFILE, int fileIndex,  int offensive, bool betray, int marching_from_constantinople, int is_water_poisoned, int increase_amount, int rounds)
{
	ofstream file1, file2, file3;
	string n = "one_battle_" + to_string(fileIndex) + ".csv";
	file1.open(n.c_str());
	string m = "agent_stat_per_round_" + to_string(fileIndex) + ".txt";
	file2.open(m.c_str());
	string s = "casualty_per_round_" + to_string(fileIndex) + ".csv";
	file3.open(s.c_str());

	if (file1 && file2 && file3) {
		printf("Successfully open a file, filename: %s \n", n.c_str());

		//first line of csv
		file1 << "one battle with attributes " << ": is_ottoman_offensive = " << offensive << ", betrayal = " << betray
			<< ", march_from_constantinople = " << marching_from_constantinople << ", is_water_poisoned = " << is_water_poisoned
			<< ", size_increase = " << increase_amount << ", rounds = " << rounds << "\n";

		file1 << "\n";
		file3 << "round, Ottoman Casualty, Tamerlane Casualty, \n";
		one_battle(RESULTFILE, file1, file2, file3,offensive, betray, marching_from_constantinople, is_water_poisoned, increase_amount, rounds);
	}

	annihilate_all_properties_of_battle();

	file1.close();
	file2.close();

	//ivm.remapping(is_water_poisoned, marching_from_constantinople, betray, cr);

	return 0;
}

void Battle::write_statistics(ofstream& RESULTFILE, ofstream& file, int r, int rounds)
{
	if (file) {
		if (r >= rounds) file << "\n\nDRAW,\n";
		else if (no_one_is_able_to_fight(Bayezid)) file << "\nBayezid lost,\n";
		else if (no_one_is_able_to_fight(Tamerlane)) file << "\nTamerlane lost,\n";

	/*	file << "\nBroken: ";
		for (Agent *a : ivm.AgentList) {
			if ((*a).getAgentState() == BROKEN) file << (*a).getIndex() << " , ";
		}*/

		//STATISTICS
		int total = 0, alive = 0,running = 0, left = 0;
		file << "\nOttoman statistics,\n";
		file << "name, index, size, morale, pos, fatigue, state,\n";
		for (Agent *a : ivm.AgentList) {
			if ((*a).getSide() ==Bayezid) {
				file << (*a).printName() <<", " << (*a).getIndex() <<"," << (*a).getSize() << "," << (*a).getMorale() << ", (" << (*a).getPos()[0] << "  " << (*a).getPos()[1] << ")," << (*a).getFatigue() << ","
					<< (*a).printState() << ",\n";

				total += (*a).getInitialSize();
				if ((*a).is_alive)	alive += (*a).getSize();

				//if ((*a).getAgentState() == BROKEN) running += (*a).getSize();
				if (!(*a).in_battlefield) left += (*a).getSize();
			}
		}
		double ottoman_casualty_rate = (double)(total - alive) / (double)total;
		double Ottoman_left_rate = (double)left / (double)total;
		file << "\ntotal Ottoman soldiers, killed, in battlefield, left, total kill rate,\n";
		file << total << "," << total - alive << "," << total - left << "," << left << "," << ottoman_casualty_rate << ",\n";
		RESULTFILE << ottoman_casualty_rate << "," << Ottoman_left_rate << ",";


		file << "\nTamerlane statistics,\n";
		file << "name, index, size, morale, pos, fatigue, state,\n";
		total= 0, alive =0, left = 0;
		for (Agent *a : ivm.AgentList) {

			if ((*a).getSide() == Tamerlane) {
				file << (*a).printName() << ", " << (*a).getIndex() << "," << (*a).getSize() << "," << (*a).getMorale() << ", (" << (*a).getPos()[0] << "  " << (*a).getPos()[1] << ")," << (*a).getFatigue() << ","
					<< (*a).printState() << ",\n";

				total += (*a).getInitialSize();
				if ((*a).is_alive)	alive += (*a).getSize();
				if (!(*a).in_battlefield) left += (*a).getSize();
			}
		}
		double Tamerlane_casualty_Rate = (double) (total - alive) / (double)total;
		double Tamerlane_left_rate = (double)left / (double)total;
		file << "\ntotal Tamerlane soldiers, killed, in battlefield, left, total kill rate,\n";
		file << total << "," << total - alive << "," << total - left << "," << left << "," << Tamerlane_casualty_Rate << ",\n";
		RESULTFILE << Tamerlane_casualty_Rate << "," << Tamerlane_left_rate<< ",";

		if (Tamerlane_casualty_Rate + Tamerlane_left_rate > ottoman_casualty_rate + Ottoman_left_rate) {
			RESULTFILE << "Tamerlane Might Lost, \n";
		}
		else {
			RESULTFILE << "Ottoman Might Lost, \n";
		}

	}
}

void Battle::write_agent_stats(std::ofstream & file, int r, Agent * a)
{
	if (file) {
		file << (*a).getIndex() << "	" << (*a).getPos()[0] << "	 " 
			<< (*a).getPos()[1] << "	" << (*a).getSide()<< "	" << (*a).getAgentState() <<"	"
	/*		<< (*a).getSize() << "," << (*a).getMorale() << "," << (*a).getFatigue() */
			<< "\n";
	}
}

void Battle::annihilate_all_properties_of_battle()
{
	ivm.Ottoman_last_index = 0;

	Ottoman_size = 0, Tamerlane_size = 0;
	Ottoman_alive_in_battle = 0, Tamerlane_alive_in_battle = 0;
	Ottoman_left_battle = 0, Tamerlane_left_battle = 0;
	Ottoman_betray = 0, Ottoman_broken = 0, Ottoman_retreat = 0, Tamerlane_retreat = 0, Tamerlane_broken = 0;
	Ottoman_fight_to_death = 0, Tamerlane_fight_to_death = 0;
	Ottoman_dead = 0, Tamerlane_dead = 0, Ottoman_still_fighting = 0, Tamerlane_still_fighting = 0, Ottoman_ready = 0, Tamerlane_ready = 0;
	Ottoman_soldier = 0, Tamerlane_soldier = 0;

	for (Agent* a : ivm.AgentList) {
		(*a).clear_enemies();
		(*a).clear_enemies_to_shoot();
		(*a).clear_neighbor();
	}
	delete_searchTree();
}


void Battle::initiate_battle(int is_water_poisoned, int marching_from_constantinople, int increase_amount)
{
	Ottoman_alive_in_battle = ivm.Ottoman_last_index;
	Tamerlane_alive_in_battle = ivm.AgentList.size() - ivm.Ottoman_last_index;

	Ottoman_size = Ottoman_alive_in_battle;
	Tamerlane_size = Tamerlane_alive_in_battle;
}

/*
When choose to move/attack/run for life, this agent has already updated direction and position
Here properties need to update:
*/
int Battle::updateMorale_shootingR_sightR(Agent * a)
{
	if ((*a).is_size_below_1_percent() && (*a).is_alive ) {

		if ((*a).getSide() == Bayezid) --Ottoman_alive_in_battle;
		else --Tamerlane_alive_in_battle;

		(*a).is_alive = false;
		(*a).changeAgentState(DEAD);
		printf("  Agent # %d is dead\n", (*a).getIndex());
		return -1;
	}
	else if (!(*a).is_alive || ! (*a).in_battlefield) return -1;

	(*a).updateMorale(cr);
	(*a).updateShootingRange(cr);
	(*a).updateSightRange(cr);

	return 0;
}

/*
It is only available for agents STILL ALIVE AND IN BATTLEFIELD

Based on current situation, choose among IDLE, MOVE, ATTACK, RETREAT
  If offensive = 0, Ottoman stay idle, and Tamerlane move.
  If offensive = 1, Ottoman move, Tamerlane stay.
  If offensive = 2, both move
*/
void Battle::choose_and_Execute_Action(Agent * a, int offensive, int betray, int round){
	
	//dead
	if ((*a).getAgentState() == DEAD) {
		return;
	}
	//out
	if (!(*a).in_battlefield) return;

	int current_state = (*a).getAgentState();
	switch (current_state) {
	case READY:

		/* if input attribute indicates that there is betray in the battle, it will have 50% percent
		chance to betray.
		It also requires that this agent belongs to betrayable_unit (Tartar or Anatolina) and satisfies
		either (morale < 10% or size < 50%)
		*/
		if (betray == 1) {
			int betray_rand = rand() % 2;// throw an int [0 , 1]

			if (betray_rand == 1 && (*a).is_betrayable_unit()
				&& ((*a).is_morale_below_20() || (*a).is_size_below_50_percent())) {

				(*a).changeSide();
				++Tamerlane_alive_in_battle;
				--Ottoman_alive_in_battle;
				++Ottoman_betray;
			}
		}
		// if less than 30% of agent in this side remain fighting, WITHDRAW
		if (more_than_70percent_in_flight((*a).getSide())
			&& (!(*a).is_morale_below_zero())) {

			withdraw_to_built_in_dir(a);
			(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);
			(*a).changeAgentState(RETREAT);

			if ((*a).getSide() == Bayezid) ++Ottoman_retreat;
			else if ((*a).getSide() == Tamerlane) ++Tamerlane_retreat;

		}

		//If no enemy in sight range, MOVE if offensive, IDLE if offensive.
		else if ((*a).getEnemies().empty()) {

			if (idle_or_move(a, offensive) == Battle::Actions::IDLE) {} //no thing to change.
			else {
				if (round > 15) {
					//move toward the center

					Agent::Direction dir = find_new_dir_after_move((*a).getPos(), { 4500, 8500 });
					int distance = rand() % (150 + 1) + 100;
					(*a).changeDirection(dir);
					//printf("----move toward center, index %d, (%d, %d) ", (*a).getIndex(), (*a).getPos()[0], (*a).getPos()[1]);
					(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, dir, false));
					(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);

					//printf("now pos (%d. %d)\n",  (*a).getPos()[0], (*a).getPos()[1]);
				}
				else {
					int random = rand() % 7;
					if ((*a).getSide() == Bayezid) {
						if (random <= 2) {} //idle
						else if (random > 2 && random < 5) { move_to_flank(a); }
						else { move_straight(a); }
					}
					else {
						move_to_built_in_dir(a);
						(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);
					}
				}
			}
		}

		//if there are enemies in sight range, and is not retreating
		else {
			int r = rand() % 9 + 4;
			if (idle_or_move(a, offensive) == Battle::Actions::IDLE && round < r) {}
			else {

				Agent * e_in_neighbor = find_enemy_in_neighbor(a);

				//if there is an enemy in neighbor range, ATTACK
				if (e_in_neighbor != nullptr && r < 6 && distance_between_two_points((*a).getPos(), (*e_in_neighbor).getPos()) <= NEIGHBOR_RANGE) {


					//int random = rand() % 2;

					//if (random == 0) 
					charge_newly_chosen_enemy(a, e_in_neighbor);
					/*else {
						charge_newly_chosen_enemy(a, find_the_closest_enemy(a));
					}*/

					//change state to ENGAGED, increase fatigue
					(*a).changeAgentState(ENGAGED);
					(*a).changeFatigue(FATIGUE_INCREASE_IF_ATTACK);
				}
				else {
					//shoot and move
					double shooting_range = (*a).getMissileRange();
					Agent *enemy = chooseTheWeakestEnemy(a);
					vector<int> enemy_pos = (*enemy).getPos();

					// if not able to shoot
					if ((*a).not_able_to_shoot()) {}

					// if the chosen enemy is in shooting range
					else if (distance_between_two_points(enemy_pos, (*a).getPos()) <= shooting_range) {

						//shoot this enemy
						shoot_the_enemy(a, enemy);

					}
					else {
						// else we need to range search enemies in shooting range
						rsTree.findAgent_within_range(a, shooting_range, enemies_in_shooting_search);

						// If we can find a enemy to shoot
						if (!(*a).getEnemies_in_missile().empty()) {
							//printf("# of enmies in missile range is %d\n", (*a).getEnemies_in_missile().size());
							//int random = rand() % ((*a).getEnemies_in_missile().size()); // randomly pick one
							Agent *enemy_to_shoot = (*a).getEnemies_in_missile()[0];

							//shoot the enemy
							shoot_the_enemy(a, enemy_to_shoot);
						}
					}

					//and move to it.
					move_to_chosen_enemy(a, enemy);
					(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);
				}
			}
		}
		break;
	case ENGAGED:
		//printf("*****ENGAGED!! Agent # %d", (*a).getIndex());

		//If (less than 30% of this side remains fighting AND HP >= 20% AND morale > 0) => WITHDRAW
		if (more_than_70percent_in_flight((*a).getSide())
			&& (!(*a).is_morale_below_zero())) {

			withdraw_to_built_in_dir(a);
			(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);
			(*a).changeAgentState(RETREAT);
			(*a).is_being_attacked = false;
			(*a).setCurrentEnemyIndex(-1);

			if ((*a).getSide() == Bayezid) ++Ottoman_retreat;
			if ((*a).getSide() == Tamerlane) ++Tamerlane_retreat;

			//Disadvantage on MORALE when go from ENGAGE to RETREAT
			(*a).weakenMorale();
		}

		//If(certain agents, AND HP <20 % AND morale > 0), FIGHT_TO_DEATH.
		else if (/*(*a).is_surrounded() && */
			(*a).is_able_to_fight_to_death()
			&& (*a).is_size_below_20_percent() && (!(*a).is_morale_below_zero())) {

			if ((*a).getSide() == Bayezid) ++Ottoman_fight_to_death;
			else ++Tamerlane_fight_to_death;

			(*a).changeAgentState(FIGHT_TO_DEATH);
			(*a).strengthenAbilities();
			(*a).strengthenMorale();

			// because it is engaged with someone, so keep attacking!!
			attack_enemy(a, ivm.AgentList[(*a).getCurrentEnemyIndex()]);

		}
		// RUN for life
		else if ((*a).is_morale_below_zero() && (*a).is_size_below_20_percent()) {

			printf("Agent # %d is broken from engaged ", (*a).getIndex());

			running_for_life(a); // running for life and disable all abiity
			(*a).changeAgentState(BROKEN);
			(*a).is_being_attacked = false;
			(*a).setCurrentEnemyIndex(-1);

			if ((*a).getSide() == Bayezid) ++Ottoman_broken;
			if ((*a).getSide() == Tamerlane) ++Tamerlane_broken;
		}
		else {
			Agent * current_enemy = ivm.AgentList[(*a).getCurrentEnemyIndex()];

			//if enemy routs or die, DISENGAGE
			if (!(*current_enemy).is_alive || (*current_enemy).getAgentState() == BROKEN || !(*current_enemy).is_being_attacked
				|| (*current_enemy).getAgentState() == RETREAT || (*current_enemy).getAgentState() == DEAD) {

				(*a).changeAgentState(READY);
				(*a).is_being_attacked = false;
				(*a).setCurrentEnemyIndex(-1);
			}
			else {
				//keep attacking the current enemy
				attack_enemy(a, ivm.AgentList[(*a).getCurrentEnemyIndex()]);
				(*a).changeFatigue(FATIGUE_INCREASE_IF_ATTACK);
			}
		}
		break;
	case BROKEN:
		running_for_life(a);
		break;
	case FIGHT_TO_DEATH:

		if ((*a).getCurrentEnemyIndex() != -1) {
			Agent * current_enemy = ivm.AgentList[(*a).getCurrentEnemyIndex()];

			if (!(*current_enemy).is_alive || (*current_enemy).getAgentState() == BROKEN || !(*current_enemy).is_being_attacked
				|| (*current_enemy).getAgentState() == RETREAT || (*current_enemy).getAgentState() == DEAD) {

				(*a).is_being_attacked = false;
				(*a).setCurrentEnemyIndex(-1);

				Agent * e_in_neighbor = find_enemy_in_neighbor(a);
				if (e_in_neighbor != nullptr) {

					charge_newly_chosen_enemy(a, e_in_neighbor);
				}
				else {
					Agent::Direction dir = find_new_dir_after_move((*a).getPos(), { 4500, 8500 });
					int distance = rand() % (150 + 1) + 100;
					(*a).changeDirection(dir);
					//printf("----move toward center, index %d, (%d, %d) ", (*a).getIndex(), (*a).getPos()[0], (*a).getPos()[1]);
					(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, dir, false));
				}

			}
			//remain still unless it is being attacked.
			else if ((*a).is_being_attacked && (*current_enemy).is_alive) {

				attack_enemy(a, current_enemy);
				//(*a).changeFatigue(FATIGUE_INCREASE_IF_ATTACK);

			}//WITHDRAW
			else if (more_than_70percent_in_flight((*a).getSide())
				&& (!(*a).is_morale_below_zero())
				&& (!(*a).is_surrounded())) {

				withdraw_to_built_in_dir(a);
				(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);
				(*a).changeAgentState(RETREAT);
				(*a).is_being_attacked = false;
				(*a).setCurrentEnemyIndex(-1);

				if ((*a).getSide() == Bayezid) ++Ottoman_retreat;
				if ((*a).getSide() == Tamerlane) ++Tamerlane_retreat;

			}
		}
		else {
				Agent * e_in_neighbor = find_enemy_in_neighbor(a);
				if (e_in_neighbor != nullptr) {

					charge_newly_chosen_enemy(a, e_in_neighbor);
				}
				else {
					Agent::Direction dir = find_new_dir_after_move((*a).getPos(), { 4500, 8500 });
					int distance = rand() % (100 + 1) + 50;
					(*a).changeDirection(dir);
					//printf("----move toward center, index %d, (%d, %d) ", (*a).getIndex(), (*a).getPos()[0], (*a).getPos()[1]);
					(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, dir, false));
				}
			}

		break;

	case RETREAT:
		// fight to death
		if ((*a).is_able_to_fight_to_death()
			&& (*a).is_size_below_20_percent() && (!(*a).is_morale_below_zero())) {

			if ((*a).getSide() == Bayezid) {
				++Ottoman_fight_to_death;
				--Ottoman_retreat;
			}
			else {
				++Tamerlane_fight_to_death;
				--Tamerlane_retreat;
			}
			(*a).changeAgentState(FIGHT_TO_DEATH);
			(*a).strengthenAbilities();
			(*a).strengthenMorale();

			//remain still. 

		}
		// RUN for life
		else if ((*a).is_morale_below_zero()) {

			if ((*a).getSide() == Bayezid) {
				--Ottoman_retreat; ++Ottoman_broken;
			}
			else {
				--Tamerlane_retreat; ++Tamerlane_broken;
			}
			printf("Agent # %d is broken from retreat ", (*a).getIndex());

			running_for_life(a); // running for life and disable all abiity
			(*a).changeAgentState(BROKEN);
			(*a).is_being_attacked = false;

		}
		// if being attacked
		else if ((*a).is_being_attacked) {

			if ((*a).getSide() == Bayezid) --Ottoman_retreat;
			else --Tamerlane_retreat;

			(*a).changeAgentState(ENGAGED);
			attack_enemy(a, ivm.AgentList[(*a).getCurrentEnemyIndex()]);
			(*a).changeFatigue(FATIGUE_INCREASE_IF_ATTACK);

		}
		// keep withdraw
		else {
			withdraw_to_built_in_dir(a);
			(*a).changeFatigue(FATIGUE_INCREASE_IF_MOVE);
		}
		break;
	}
}




void Battle::move_to_flank(Agent * a)
{
	Agent::Direction dir;
	if ((*a).getSide() == Bayezid) {

		if ((*a).getName() == Agent::Name::SERB_CAL || (*a).getName() == Agent::Name::SERB_INF
			|| (*a).getName() == Agent::Name::ANATOLIAN || (*a).getName() == Agent::Name::KAPIKULU) {
			dir = Agent::Direction::SW;
		}
		else if ((*a).getName() == Agent::Name::TARTAR || (*a).getName() == Agent::Name::RUMELIAN) {
			dir = Agent::Direction::SE;
		}

		int distance = rand() % (int)(NEIGHBOR_RANGE)+100;
		//Have not consider the problem of overlapping
		vector<int> newPos = find_new_pos_after_move((*a).getPos(), distance, dir, false);

		(*a).changeDirection(dir);
		(*a).changePos(newPos);
	}

}

void Battle::move_straight(Agent * a)
{
	Agent::Direction dir;
	if ((*a).getName() != Agent::Name::SERB_CAL
		&& (*a).getName() != Agent::Name::SERB_INF && (*a).getName() != Agent::Name::ANATOLIAN
		&& (*a).getName() != Agent::Name::TARTAR && (*a).getName() != Agent::Name::KAPIKULU) {

		dir = Agent::Direction::SOUTH;
		int distance = rand() % (int)(NEIGHBOR_RANGE)+100;
		//Have not consider the problem of overlapping
		vector<int> newPos = find_new_pos_after_move((*a).getPos(), distance, dir, false);

		(*a).changeDirection(dir);
		(*a).changePos(newPos);
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
		//should not accur
	}
	else {
		Agent::Direction dir_after_move = find_new_dir_after_move((*a).getPos(), (*chosenEnemy).getPos());
		int distance = find_distance_to_move(a, chosenEnemy);
		//Have not consider the problem of overlapping
		vector<int> newPos = find_new_pos_after_move((*a).getPos(), distance, dir_after_move, false);
		
		(*a).changeDirection(dir_after_move);
		(*a).changePos(newPos);
	}
}

/*
 Move to it, and then deliver damage
 Right now, move to it means overlapping *****************
*/
void Battle::charge_newly_chosen_enemy(Agent * a, Agent * chosenEnemy)
{
	Agent::Direction dir_after_move = find_new_dir_after_move((*a).getPos(), { (*chosenEnemy).getPos()[0] - 20, (*chosenEnemy).getPos()[1] - 20 });

	(*a).changeDirection(dir_after_move); //move there
	(*a).changePos({ (*chosenEnemy).getPos()[0] - 20, (*chosenEnemy).getPos()[1] - 20 });

	attack_enemy(a, chosenEnemy); //deliver damage

	int special_bonus_received = 0;
	if ((*chosenEnemy).is_standing_on_high_ground(cr)) {
		special_bonus_received += SPECIAL_BONUS_TO_ATTACK;
	}
	if ((*chosenEnemy).is_higher_than_enemy(a, cr)) {
		special_bonus_received += SPECIAL_BONUS_TO_ATTACK;
	}

	// charge will cause damage to itself
	if (special_bonus_received > 0) (*a).decrease(special_bonus_received);
	else (*a).decrease(5);
}

/*
enemy_size -= damage
*/
void Battle::attack_enemy(Agent * a, Agent * enemy)
{
	//attack also means being attacked
	(*a).setCurrentEnemyIndex((*enemy).getIndex());
	(*enemy).setCurrentEnemyIndex((*a).getIndex());
	(*a).is_being_attacked = true;
	(*enemy).is_being_attacked = true;

	int special_bonus = 0;
	if ((*a).is_standing_on_high_ground(cr)) {
		special_bonus += SPECIAL_BONUS_TO_ATTACK * 2;
	}
	if ((*a).is_higher_than_enemy(enemy, cr)) {
		special_bonus += SPECIAL_BONUS_TO_ATTACK;
	}
	if ((*enemy).is_higher_than_enemy(a, cr)) special_bonus -= SPECIAL_BONUS_TO_ATTACK;

	special_bonus+= SPECIAL_BONUS_TO_ATTACK * has_special_bonus_against(a, enemy);
	int damage = (*enemy).attack_damage_delivered(special_bonus, (*enemy).getArmorDefence());
	
	(*enemy).decrease(damage);
}

void Battle::move_to_built_in_dir(Agent * a)
{
	Agent::Direction dir;
	int distance = rand()% (int)NEIGHBOR_RANGE + 100;

	if ((*a).getSide() == Bayezid) { 
		dir = Agent::Direction::SOUTH; 
	}
	else dir = Agent::Direction::NORTH;

	(*a).changeDirection(dir);
	(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, dir, false));
}

/* Pre-requirement: this agent decided to retreat.
If it is a Otoman agent, retreat to north.
If it is a bayezid agent, retreat to south
*/
void Battle::withdraw_to_built_in_dir(Agent * a)
{
	Agent::Direction dir;
	int distance = 200;

	if ((*a).getSide() == Bayezid) dir = Agent::Direction::NORTH;
	else dir = Agent::Direction::SOUTH;

	(*a).changeDirection(dir);
	vector<int> newPos = find_new_pos_after_move((*a).getPos(), distance, dir, true);

	//out of battlefield
	if (newPos[0] == -1) {
		(*a).in_battlefield = false;

		if ((*a).getSide() == Bayezid) { --Ottoman_alive_in_battle; ++Ottoman_left_battle; }
		else { --Tamerlane_alive_in_battle;  ++Tamerlane_left_battle; }
	}
	(*a).changePos(newPos);
}

void Battle::shoot_the_enemy(Agent * a, Agent * enemy)
{
	//printf("-----Agent %d is shooting Agent %d ", (*a).getIndex(), (*enemy).getIndex());
	(*enemy).decrease((*a).missile_damage_delivered((*enemy).getArmorDefence()));
}

/*
return the enemy chosen by a random advantage type.
*/
Agent * Battle::chooseTheWeakestEnemy(Agent * a)
{
	if ((*a).getEnemies().size() == 0) return nullptr; //should not happen. it is handled by FSM already
	else {
		int random = rand() % 4;
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
				//roll the dice again [0, 1]
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
	if ((*a).getEnemies().empty()) return nullptr; //should not happen, handled by FSM

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
	vector<Agent *> enemies_engaged;

	for (Agent * enemy : (*a).getEnemies()) {
		if (has_special_bonus_against(a, enemy) != 0) {
			enemies_with_special_bonus.push_back(enemy);
		}
		if ((*enemy).getAgentState() == BROKEN || (*enemy).getAgentState() == RETREAT) {
			enemies_running_away.push_back(enemy);
		}

		if ((*enemy).getAgentState() == ENGAGED) {
			enemies_engaged.push_back(enemy);
		}
	}

	if (enemies_with_special_bonus.empty() && enemies_running_away.empty() && enemies_engaged.empty()) return nullptr;
	else {
		int random = rand() % 3;
		//choose the enemy who is running for its life
		if (random == 0 && !enemies_running_away.empty()) {
			random = rand() % (enemies_running_away.size());
			return enemies_running_away[random];
		}	
		else if (random == 1 && !enemies_engaged.empty()) {
			random = rand() % (enemies_engaged.size());
			return enemies_engaged[random];

		}
		// choose the enemy with special bonus
		else if (random == 2 && !enemies_with_special_bonus.empty()) {
			random = rand() % (enemies_with_special_bonus.size());
			return enemies_with_special_bonus[random];
		}
		else {
			if (!enemies_running_away.empty()) {
				random = rand() % (enemies_running_away.size());
				return enemies_running_away[random];
			}
			else if (!enemies_engaged.empty()){
				random = rand() % (enemies_engaged.size());
				return enemies_engaged[random];
			}
			else {
				random = rand() % (enemies_with_special_bonus.size());
				return enemies_with_special_bonus[random];
			}
		}
	}
	}
	

/*If there is enemy in neighbor, return the first one found
Else return nullptr*/
Agent * Battle::find_enemy_in_neighbor(Agent * a)
{
	vector<Agent *> enemy;
	for (Agent *n : (*a).getNeighbor()) {
		if ((*a).getSide() != (*n).getSide()) {
			enemy.push_back(n);
		}
	}
	if (enemy.empty())return nullptr;
	else {
		int random = rand() % (enemy.size());
		return enemy[random];
	}
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
	if (A.empty() || B.empty()) {}//printf("EMPTY VECTOR**************\n");
	else return sqrt(pow(B[0] - A[0], 2) + pow(B[1] - A[1], 2));
}

/*
IF This agent's state is BROKEN,
	all abilities to attack, defend and shoot are disabled; 
	this agent will move in the opposite direction from the closest enemy.
*/
void Battle::running_for_life(Agent * a)
{
	Agent::Direction direction_from;
	Agent * closestEnemy = find_the_closest_enemy(a);
	int distance = (int)NEIGHBOR_RANGE;

	if (closestEnemy == nullptr) {
		if ((*a).getSide() == Bayezid) direction_from = Agent::Direction::NORTH;
		else direction_from = Agent::Direction::SOUTH;
	}
	else {
		Agent::Direction direction_toward_enemy = find_new_dir_after_move((*a).getPos(), (*closestEnemy).getPos());
	
		direction_from = (*a).getOppositeDir(direction_toward_enemy);
	}

	(*a).changeDirection(direction_from);
	(*a).disableAttackAbility();

	vector<int> newPos = find_new_pos_after_move((*a).getPos(), distance, direction_from, true);
	//out of battlefield
	if (newPos[0] == -1) {
		(*a).in_battlefield = false;
		
		if ((*a).getSide() == Bayezid) { --Ottoman_alive_in_battle; ++Ottoman_left_battle; }
		else {--Tamerlane_alive_in_battle;  ++Tamerlane_left_battle;}
	}
	(*a).changePos(newPos);
}

/*
Return the direction at which this agent should face after moving toward the enemy
*/
Agent::Direction Battle::find_new_dir_after_move(std::vector<int> pos, std::vector<int> enemy_pos)
{
	if (enemy_pos[0] <= pos[0] + DIRECTION_STANDARD && enemy_pos[0] >= pos[0] - DIRECTION_STANDARD) {

		if (enemy_pos[1] <= pos[1]) return Agent::Direction::NORTH;
		else return Agent::Direction::SOUTH;
	}
	else if (enemy_pos[1] <= pos[1] + DIRECTION_STANDARD && enemy_pos[1] >= pos[1] - DIRECTION_STANDARD) {
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
std::vector<int> Battle::find_new_pos_after_move(std::vector<int> pos, int distance, Agent::Direction dir, bool out_field_waiver)
{
	vector<int> newPos;
	newPos.push_back(pos[0]);
	newPos.push_back(pos[1]);

	if (dir == Agent::Direction::EAST) newPos[0] += distance;

	else if (dir == Agent::Direction::WEST) newPos[0] -= distance;
	
	else if (dir == Agent::Direction::NORTH) newPos[1] -= distance;

	else if (dir == Agent::Direction::SOUTH) newPos[1] += distance;

	else {
		int x_dis = sqrt(pow(distance, 2) / 2), y_dis = sqrt(pow(distance, 2) / 2);

		if (dir == Agent::Direction::NW) { newPos[0] -= x_dis; newPos[1] -= y_dis; }
		else if (dir == Agent::Direction::SE) { newPos[0] += x_dis; newPos[1] += y_dis; }
		else if (dir == Agent::Direction::NE) { newPos[0] += x_dis; newPos[1] -= y_dis; }
		else { newPos[0] -= x_dis; newPos[1] += y_dis; }
	}

	//If WITHDRAW OR RUN_FOR_LIFE
	if (out_field_waiver) {
		if (newPos[0] > BATTLEFIELD_X_MAX || newPos[0] < 0 || newPos[1] > BATTLEFIELD_Y_MAX || newPos[1] < 0) {
			return { -1, -1 };
		}
		else {
			return newPos;
		}
	}
	else {
		if (newPos[0] > BATTLEFIELD_X_MAX || newPos[0] < 0 || newPos[1] > BATTLEFIELD_Y_MAX || newPos[1] < 0) {
			return pos;
		}
		else {
			return newPos;
		}
	}

	
}



void Battle::move_to_default_enemy_direciton(Agent * a)
{
	int distance = rand() % ((int)NEIGHBOR_RANGE + 1) + 100;
	//Ottomans are supposed to move south
	if ((*a).getSide() == Bayezid) {
		(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, Agent::Direction::SOUTH, false));
	}
	//Tamerlane are supposed to move north
	else {
		(*a).changePos(find_new_pos_after_move((*a).getPos(), distance, Agent::Direction::NORTH, false));
	}
}

bool Battle::more_than_70percent_in_flight(int side)
{
	return 
		((side == Bayezid && (double)(Ottoman_broken + Ottoman_retreat + Ottoman_dead) / (double) Ottoman_size >= 0.7)
		|| (side == Tamerlane && (double)(Tamerlane_broken + Tamerlane_retreat + Tamerlane_dead) / (double) Tamerlane_size >= 0.7));
}

bool Battle::no_one_is_able_to_fight(int side)
{
	if (side == Bayezid) {
	/*	printf("Ottoman size is %d, Ottoman dead is %d, Ottoman_left is %d, Ottoman broken is %d\n", Ottoman_size
			, Ottoman_dead, Ottoman_left_battle, Ottoman_broken);*/
		return (Ottoman_size - Ottoman_dead - Ottoman_left_battle - Ottoman_broken) == 0;
	}
	else {
		return (Tamerlane_size - Tamerlane_dead - Tamerlane_left_battle - Tamerlane_broken ) == 0;
	}
}

/*
If offensive = 0, Ottoman stay idle, and Tamerlane move.
If offensive = 1, Ottoman move, Tamerlane stay.
If offensive = 2, both move
*/
Battle::Actions Battle::idle_or_move(Agent * a, int offensive)
{
	if (offensive == 0) {
		if ((*a).getSide() == Bayezid && (*a).getName() != Agent::Name::SERB_CAL && (*a).getName() != Agent::Name::RUMELIAN
			&& (*a).getName() != Agent::Name::SERB_INF && (*a).getName() != Agent::Name::ANATOLIAN
			&& (*a).getName() != Agent::Name::TARTAR && (*a).getName() != Agent::Name::KAPIKULU)
		{
			return Battle::Actions::IDLE;
		}
		else { return Battle::Actions::MOVE; }
	}
	else if (offensive == 2) {
		if ((*a).getSide() == Tamerlane) { return Battle::Actions::IDLE; }
		else { return Battle::Actions::MOVE; }
	}
	else {
		return Battle::Actions::MOVE;
	}
}

/*
Must make sure it does not move beyond battlefield
the distance [100, neighbor range]
*/
int Battle::find_distance_to_move(Agent * a, Agent * chosenEnemy)
{
	int max_to_move, distance;
	if (chosenEnemy == nullptr) max_to_move = -1;
	else max_to_move = (int)distance_between_two_points((*a).getPos(), (*chosenEnemy).getPos());

	distance = rand() % (200 + 1) + 100; //So it will be easier to check whether overlap 
	
	// If the distance between this agent and its enemy is smaller than the distance decide to move
	if (max_to_move != -1 && distance > max_to_move) { //should not happen
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
