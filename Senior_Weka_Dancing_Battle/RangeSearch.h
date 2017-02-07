#pragma once

#include <vector>
#include <queue>
#include "Agent.h"

#define RED -999    // Even level is red
#define BLACK 999  // Odd level is black
#define neighbor_search 1000
#define enemies_in_sight_search 1001
#define enemies_in_shooting_search 1002
//#define POINT -1001 // Not a boundary

struct node {
	int x, y, color; 
	node * left;
	node * right;
	node * parent;

	Agent * ag;

	node(int key_x, int key_y) 
	{
		x = key_x;
		y = key_y;
		ag = NULL;
		left = NULL;
		right = NULL;
		parent = NULL;
	}

};

class RangeSearch {

public: 
	~RangeSearch();

	void put(Agent *a, int, int);
	void findAgent_within_range(Agent *a, double range, int which_search);
	void printTree();
	void printHeight();
	
private:
	node * root = NULL;

	node * putBoundaries(Agent *a, node * n, int key_x, int key_y, node * parent, int parentColor);

	void postOrder(node * n, int indent = 0);

	int getTreeHeight(node *n);

	void freeBST(node *);

	/*----------- Range Search-----------------*/
	
	void rangeSearch(node *n, Agent * agent, double range, int which_search);
	bool is_agent_facing_enemy(Agent *agent, Agent *enemy);
	
};