#include <iostream>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <queue>
#include <math.h>
#include <functional>

#include "RangeSearch.h"

using namespace std;

RangeSearch::~RangeSearch()
{
	delete root;
	root = NULL;
	size = 0;
	//freeBST(root);
}

/* ----------------------Insert-------------------*/
void RangeSearch::put(Agent *a, int key_x, int key_y)
{
	root = putBoundaries(a, root, key_x, key_y, NULL, RED);
}

void RangeSearch::findAgent_within_range(Agent * a, double range, int which_search)
{
	rangeSearch(root, a, range, which_search);
}

node * RangeSearch::putBoundaries(Agent *a, node * n, int key_x, int key_y, node * parent, int parentColor)
{
	//printf("------------------\n");
	if (n == NULL) {
		node * toReturn = new node(key_x, key_y);
		toReturn->color = -parentColor;
		toReturn->parent = parent;
		toReturn->ag = a;
		++size;
		return toReturn;
	}
	else {
		// odd level, compare x.	
		if (n->color == BLACK ) { 
			
			//printf(" x= %d, n->x = %d \n", key_x, n->x);
			if (key_x <= n->x) {
				
				//printf("add to left\n");
				n->left = putBoundaries(a, n->left, key_x, key_y, n, n->color);
			}
			else {
				//printf("add to right\n");
				n->right = putBoundaries(a, n->right, key_x, key_y, n, n->color);
			}
		}
		else {            
			// even level, compare y
			//printf(" y= %d, n->y = %d \n", key_y, n->y);
			if (key_y <= n->y) {

				//printf("add to left\n");
				n->left = putBoundaries(a, n->left, key_x, key_y, n, n->color);
			}
			else {
				//printf("add to right\n");
				n->right = putBoundaries(a, n->right, key_x, key_y, n, n->color);
			}
		}
		return n;
	}
}

void RangeSearch::printTree()
{
	postOrder(root, 0);
}

void RangeSearch::printHeight()
{
	printf("\n The tree height is %d\n",getTreeHeight(root, 0 , 0));
}

void RangeSearch::printSize()
{
	printf("size of tree is %d\n", size);
}

void RangeSearch::postOrder(node * n, int indent)
{
	if (n != NULL) {
		if (n->left != NULL) postOrder(n->left, indent + 4);
		if (n->right != NULL)postOrder(n->right, indent + 4);

		if (indent != 0) {
			cout << setw(indent) << ' ';
		}
		//printf("%d: %d\n", n->y, n->value);
		printf("(%d, %d)\n", n->x, n->y);
	}

}

int RangeSearch::getTreeHeight(node * n, int h, int hMax)
{
	int left, right;
	if (n != NULL) {
		if (h > hMax) hMax = h;

		if (n->left != NULL) hMax = getTreeHeight(n->left, h + 1, hMax);

		if (n->right != NULL) hMax = getTreeHeight(n->right, h + 1, hMax);
	}
	return hMax;
}

void RangeSearch::freeBST(node * t)
{
	if (t != NULL) {
		freeBST(t->left);
		freeBST(t->right);		
		delete t;

		if (t->left != NULL) t->left = NULL;
		if (t->right != NULL) t->right = NULL;
		t = NULL;
	}
}

void RangeSearch::rangeSearch(node * n, Agent * agent, double range, int which_search)
{	
	int x, y;	
	if (n != NULL ) {
		x = (*agent).getPos().at(0);
		y = (*agent).getPos().at(1);

		// odd level, compare x
		if (n->color == BLACK) {
			double distance = sqrt(pow(n->x - x, 2) + pow(n->y - y, 2));

			if (distance != 0 && distance <= range) {
				if (n->ag != nullptr) {

					if (which_search == neighbor_search 
						&& (*n->ag).is_alive 
						&& (*n->ag).in_battlefield 
						&& (*n->ag).getAgentState() != DEAD
						) {
						(*agent).add_neighbor(n->ag);
					}
					else if (which_search == enemies_in_sight_search && (*agent).getSide() != (*n->ag).getSide()
						&& is_agent_facing_enemy(agent, n->ag) && (*n->ag).is_alive && (*n->ag).in_battlefield
						&& (*n->ag).getAgentState() != DEAD
						) {
						(*agent).add_enemies(n->ag);
					}
					else if (which_search == enemies_in_shooting_search && (*agent).getSide() != (*n->ag).getSide()
						&& is_agent_facing_enemy(agent, n->ag) && (*n->ag).is_alive && (*n->ag).in_battlefield
						&& (*n->ag).getAgentState() != DEAD
						) {
						(*agent).add_enemies_to_shoot(n->ag);
					}
				}
				else {
					printf("\nnullptr---------------\n");
				}
			}
			// if agent is within the left boundary of n
			if (n->x >= x + range) {
				rangeSearch(n->left, agent, range,  which_search);
			}
			// if agent is within the right boundary of n
			else if (n->x < x - range) {
				rangeSearch(n->right, agent, range, which_search);
			}
			//if agent intersect with boundary n, recursively traverse both left and right.
			else if (n->x < x + range && n->x >= x - range) {
			
				rangeSearch(n->left, agent, range, which_search);
				rangeSearch(n->right, agent, range, which_search);
			}
			//else prune.

		}
		// Even level, compare y
		else{
			double distance = sqrt(pow(n->x - x, 2) + pow(n->y - y, 2));
			if (distance != 0 && distance <= range) {
				if (n->ag != nullptr) {

					if (which_search == neighbor_search && (*n->ag).is_alive && (*n->ag).in_battlefield 
						&& (*n->ag).getAgentState() != DEAD
						) {
						(*agent).add_neighbor(n->ag);
					}
					else if (which_search == enemies_in_sight_search && (*agent).getSide() != (*n->ag).getSide()
						&& is_agent_facing_enemy(agent, n->ag) && (*n->ag).is_alive && (*n->ag).in_battlefield
						&& (*n->ag).getAgentState() != DEAD
						) {
						(*agent).add_enemies(n->ag);
					}
					else if (which_search == enemies_in_shooting_search && (*agent).getSide() != (*n->ag).getSide()
						&& is_agent_facing_enemy(agent, n->ag) && (*n->ag).is_alive && (*n->ag).in_battlefield
						&& (*n->ag).getAgentState() != DEAD
						) {
						(*agent).add_enemies_to_shoot(n->ag);
					}
				}
				else {
					printf("\nnullptr!------------\n");
				}
			}
			// if agent is within the left boundary of n
			if (n->y >= y + range) {
				rangeSearch(n->left, agent, range, which_search);
			}
			// if agent is within the right boundary of n
			else if (n->y < y - range) {
				rangeSearch(n->right, agent, range, which_search);
			}
			//if agent intersect with boundary n, recursively traverse both left and right.
			else if (n->y < y + range && n->y >= y - range) {
				rangeSearch(n->left, agent, range, which_search);
				rangeSearch(n->right, agent, range, which_search);
			}

		}

	}
}

bool RangeSearch::is_agent_facing_enemy(Agent * agent, Agent * enemy)
{
	vector<int> agent_Pos = (*agent).getPos();
	vector<int> enemy_Pos = (*enemy).getPos();
	Agent::Direction dir = (*agent).getDirection();

	switch (dir) {
	case Agent::Direction::NORTH:

		// means that A can face B. An agent cannot see an enemy at its back
		return (enemy_Pos[1] <= agent_Pos[1]);

	case Agent::Direction::SOUTH:
		return (enemy_Pos[1] >= agent_Pos[1]);

	case Agent::Direction::EAST:
		return (enemy_Pos[0] >= agent_Pos[0]);

	case Agent::Direction::WEST:
		return (enemy_Pos[0] <= agent_Pos[0]);

	case Agent::Direction::NW:
		return (enemy_Pos[1] <= -enemy_Pos[0] + agent_Pos[0] + agent_Pos[1]);

	case Agent::Direction::NE:
		return (enemy_Pos[1] <= enemy_Pos[0]);

	case Agent::Direction::SW:
		return (enemy_Pos[1] >= enemy_Pos[0]);

	case Agent::Direction::SE:
		return (enemy_Pos[1] >= -enemy_Pos[0] + agent_Pos[0] + agent_Pos[1]);
	}

}

