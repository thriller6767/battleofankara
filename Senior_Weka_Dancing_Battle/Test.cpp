#include <iostream>
#include <vector>
#include <functional>
#include "ConReader.h"
#include "Agent.h"
#include "Initial_val_mapper.h"
#include "Battle.h"
#include "RangeSearch.h"

using namespace std;

Battle battle;
RangeSearch kdtree;

void test_kdtree();
void map_kdtree();
void map_neighbor_and_enemy();
void kdtree_n2_compare();
void test_random();
double dis_btw(int, int, int, int);

void test_choose_and_execute_action();

void test_put_range_tree_in_battle();
void test_range_search_in_battle();
void test_while();

void test_one_battle();

void test_attack_damage();

int main() {

	//battle.initiate_battle(0, 0, 1);
	//test_attack_damage();
	//battle.ivm.fancyPrint();
	//printf("Ottoman last is %d\n", battle.ivm.Ottoman_last_index);
	//printf("morale is %d\n", (*battle.ivm.AgentList[0]).getMorale());
	//test_kdtree();
	//kdtree_n2_compare();

	//test_choose_and_execute_action();

	//battle.delete_searchTree();

	//battle.deleteAllAgent();

	test_one_battle();

	//printf(" 0.1 * 56 is %d\n", (int) (0.1 * 56));
	//test_while();

	return 0;
	
}

void test_kdtree() {

	//put nodes
	map_kdtree();

	for (Agent *a : battle.ivm.AgentList) {
		kdtree.put(a, (*a).getPos().at(0), (*a).getPos().at(1));
	}
	//print the tree
	//kdtree.printTree();

	//print tree height
	//kdtree.printHeight();

	//range search for neighbors
	for (Agent *a : battle.ivm.AgentList) {
		kdtree.findAgent_within_range(a, NEIGHBOR_RANGE, neighbor_search);

		//(*a).print_neighbors();
	}

	//range search for enemies
	for (Agent *a : battle.ivm.AgentList) {
		int sightrange = (*a).getSightRange();
		kdtree.findAgent_within_range(a, sightrange, enemies_in_sight_search);

		//(*a).print_enemies();
	}

	//single search
	//kdtree.findAgent_within_range(battle.ivm.AgentList[144], (*battle.ivm.AgentList[144]).getSightRange(), enemies_in_sight_search);

	//printf("neighbors:");
	//for (Agent * n : (*battle.ivm.AgentList[144]).getNeighbor()) printf("%d, ", (*n).getIndex());
	//printf("\nenemies: ");
	//for (Agent * n : (*battle.ivm.AgentList[144]).getEnemies()) printf("%d, ", (*n).getIndex());
	//kdtree.~RangeSearch();

}

void map_kdtree()
{
	for (Agent *a : battle.ivm.AgentList) {
		kdtree.put(a, (*a).getPos().at(0), (*a).getPos().at(1));
	}
}

void map_neighbor_and_enemy()
{
	//range search for neighbors
	for (Agent *a : battle.ivm.AgentList) {
		kdtree.findAgent_within_range(a, NEIGHBOR_RANGE, neighbor_search);

		//(*a).print_neighbors();
	}

	//range search for enemies
	for (Agent *a : battle.ivm.AgentList) {
		int sightrange = (*a).getSightRange();
		kdtree.findAgent_within_range(a, sightrange, enemies_in_sight_search);

		//(*a).print_enemies();
	}
}

void kdtree_n2_compare()
{
	printf("\n--------------------------------n2 compare---------------\n");
	int target_index = 144;
	double dis;
	int x = (*battle.ivm.AgentList[target_index]).getPos()[0];
	int y = (*battle.ivm.AgentList[target_index]).getPos()[1];

	(*battle.ivm.AgentList[target_index]).clear_enemies();
	(*battle.ivm.AgentList[target_index]).clear_neighbor();

	int x2, y2;
	for (Agent *a : battle.ivm.AgentList) {
		x2 = (*a).getPos()[0];
		y2 = (*a).getPos()[1];
		dis = sqrt(pow(x - x2, 2) + pow(y - y2, 2));

		if (dis != 0 && dis <= (*battle.ivm.AgentList[target_index]).getSightRange() && (*a).getSide() != (*battle.ivm.AgentList[target_index]).getSide()) {
			(*battle.ivm.AgentList[target_index]).add_enemies(a);
		}
		if (dis != 0 && dis <= NEIGHBOR_RANGE) {
			(*battle.ivm.AgentList[target_index]).add_neighbor(a);
		}
	}

	//dis = dis_btw(x, (*battle.ivm.AgentList[6]).getPos()[0], y, (*battle.ivm.AgentList[6]).getPos()[1]);
	//printf("the side of agent is %d\n", (*battle.ivm.AgentList[target_index]).getSide());
	//printf("sight range of this agent is %f\n", (*battle.ivm.AgentList[target_index]).getSightRange());
	//printf("distance btw this agent and #6 is %f\n", dis);
	(*battle.ivm.AgentList[target_index]).print_enemies();
	(*battle.ivm.AgentList[target_index]).print_neighbors();
}

void test_random() {
	int random;
	random = rand() % 2;
	printf("binary random is %d\n", random);

	random = rand() % 6 + 1;
	printf("Random Number between 1 - 5 is %d\n", random);

	random = rand() % (11 + 1);
	printf("random number between 0 - 11 is %d \n", random);
}

double dis_btw(int x, int x2, int y , int y2)
{
	return sqrt(pow(x - x2, 2) + pow(y - y2, 2));
}

void test_choose_and_execute_action() 
{
	map_kdtree();
	map_neighbor_and_enemy();
	int offensive = 0;
	Agent* a = battle.ivm.AgentList[0];
	Agent* b = battle.ivm.AgentList[140];
	
	/*if a is retreating*/
	(*a).changeAgentState(RETREAT);

	printf("--------------test retreat------------------------------\n");
	printf("A's current Pos is (%d, %d)\n", (*a).getPos()[0], (*a).getPos()[1]);
	/*at the very first of battle, see what choice can be done for #0*/
	battle.choose_and_Execute_Action(a, 0, 0);
	printf("after make choice, Pos is (%d, %d)\n", (*a).getPos()[0], (*a).getPos()[1]);

	/*then if a is attacked by b when retreating*/
	printf("-------------a is attacked by bwhen retreating---------------\n");
	(*b).changeAgentState(ENGAGED);
	(*b).setCurrentEnemyIndex((*a).getIndex());
	(*b).is_being_attacked = true;
	(*a).is_being_attacked = true;
	(*a).setCurrentEnemyIndex((*b).getIndex());

	printf("what will b do?\n");
	printf("A current Size is %d, pos is (%d, %d), fatigue is %d\n", (*a).getSize(), (*a).getPos()[0], (*a).getPos()[1], (*a).getFatigue());
	printf("B current Size is %d, pos is (%d, %d), fatigue is %d\n", (*b).getSize(), (*b).getPos()[0], (*b).getPos()[1], (*b).getFatigue());
	
	battle.choose_and_Execute_Action(b, 0, 0);
	printf("\nafter b making choice:\nA Size is %d, pos is (%d, %d), fatigue is %d\n", (*a).getSize(), (*a).getPos()[0], (*a).getPos()[1], (*a).getFatigue());
	printf("B Size is %d, pos is (%d, %d), fatigue is %d\n", (*b).getSize(), (*b).getPos()[0], (*b).getPos()[1], (*b).getFatigue());
	
	battle.choose_and_Execute_Action(a, 0, 0);
	printf("\nafter a making choice:\na current Size is %d, pos is (%d, %d), fatigue is %d\n", (*a).getSize(), (*a).getPos()[0], (*a).getPos()[1], (*a).getFatigue());
	printf("B current Size is %d, pos is (%d, %d), fatigue is %d\n", (*b).getSize(), (*b).getPos()[0], (*b).getPos()[1], (*b).getFatigue());

	if ((*a).getAgentState() == ENGAGED) printf("A is now ENGAGED with %d\n", (*a).getCurrentEnemyIndex());
}

void test_put_range_tree_in_battle()
{
	battle.put_rangetree_boundaries();
}

void test_range_search_in_battle()
{
	battle.map_neighbor_and_enemies();
}

void test_while()
{
	int c = 1;
	battle.initiate_battle(0, 0, 1);
	
	while (c <= 2){
		battle.put_rangetree_boundaries();
		battle.map_neighbor_and_enemies();
		battle.delete_searchTree();
		++c;
	}
	battle.deleteAllAgent();

}

void test_one_battle()
{
	int march_from_constantinople = 0;
	int is_ottoman_offensive = 0;
	int is_water_poisoned = 0;
	int any_betrayal = 0;
	int size_increase_ratio = 1;
	int rounds = 50;

	int i = 2;
	while (i <= 4) {
		
		battle.simple_result_of_one_battle(i, is_ottoman_offensive, any_betrayal, march_from_constantinople, is_water_poisoned, size_increase_ratio, rounds * i);
		i++;
	}
}

void test_attack_damage()
{
	Agent * a = battle.ivm.AgentList[0];
	Agent * b = battle.ivm.AgentList[100];


	printf(" A -> B DAMAGE is %d\n", (*a).attack_damage_delivered(0, (*b).getArmorDefence()));
}

