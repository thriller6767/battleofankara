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
void kdtree_n2_compare();
void test_random();
double dis_btw(int, int, int, int);

int main() {
	
	battle.populate(0, 0);

	test_kdtree();
	kdtree_n2_compare();



	battle.deleteAllAgent();
	return 0;
	
}

void test_kdtree() {

	//put nodes
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

		(*a).print_enemies();
	}

	//single search
	//kdtree.findAgent_within_range(battle.ivm.AgentList[144], (*battle.ivm.AgentList[144]).getSightRange(), enemies_in_sight_search);

	printf("neighbors:");
	for (Agent * n : (*battle.ivm.AgentList[144]).getNeighbor()) printf("%d, ", (*n).getIndex());
	printf("\nenemies: ");
	for (Agent * n : (*battle.ivm.AgentList[144]).getEnemies()) printf("%d, ", (*n).getIndex());
	kdtree.~RangeSearch();

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


