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
	freeBST(root);
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
	printf("\n The tree height is %d\n",getTreeHeight(root));
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

int RangeSearch::getTreeHeight(node * n)
{
	int left, right;
	if (n == NULL) return 0;
	else {
		left = getTreeHeight(n->left);
		right = getTreeHeight(n->right);

		return 1 + std::max(left, right);
	}
}

void RangeSearch::freeBST(node * t)
{
	if (t != NULL) {
		if (t->left != NULL) freeBST(t->left);
		if (t->right != NULL) freeBST(t->right);	
		
		if (t->ag != NULL) t->ag = NULL;
		if (t->parent != NULL) t->parent = NULL;

		t = NULL;
		delete t;
		
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

			int distance = sqrt(pow(n->x - x, 2) + pow(n->y - y, 2));
			if (distance != 0 && distance <= range) {
				if (which_search == neighbor_search) (*agent).add_neighbor(n->ag);
				else{
					(*agent).add_enemies(n->ag);
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

			int distance = sqrt(pow(n->x - x, 2) + pow(n->y - y, 2));
			if (distance != 0 && distance <= range) {

				if (which_search == neighbor_search) (*agent).add_neighbor(n->ag);
				else  {
					(*agent).add_enemies(n->ag);
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

