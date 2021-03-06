#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include <cstddef>
#include "Agent.h"

using namespace std;


string Agent::printName()
{
	switch (name) {
		case Agent::Name::JANISSARY: return "Janissary";
		case Agent::Name::KAPIKULU: return "Kapikulu";
		case Agent::Name::AZAPS: return "Azaps inf";
		case Agent::Name::ANATOLIAN: return "Anatolian";
		case Agent::Name::RUMELIAN: return "Rumelian";
		case Agent::Name::TARTAR: return "Tartar	";
		case Agent::Name::SERB_INF: return "Serbian inf";
		case Agent::Name::SERB_CAL: return "Serbian Cal";
		case Agent::Name::OTTO_RESERVES: return "OttomanReser";
		case Agent::Name::TAMER_HORSE_ARCHER: return "Tamerlane Horse-Archer";
		case Agent::Name::TAMER_GAURDS: return "Tamerlane Gaurds";
		case Agent::Name::TAMERLANE: return "Tamerlane	";
		case Agent::Name::TAMER_RESERVES: return "Tamerlane Reserves";
		default: printf("SHIIIIIIIIT "); return "War Elephant";
	}
}

string Agent::printDir()
{
	switch (dir) {
	case Agent::Direction::NORTH: return "N";
	case Agent::Direction::SOUTH: return "S";
	case Agent::Direction::EAST: return "E"; 
	case Agent::Direction::WEST: return "W"; 
	case Agent::Direction::NW: return "North West"; 
	case Agent::Direction::NE: return "North East"; 
	case Agent::Direction::SW: return "South West"; 
	default: printf("Shit "); return "South East"; 
	}
}

string Agent::printCate()
{
	switch (type) {
	case Agent::Category::HEAVY_CAL: return "Heavy Cavalry"; 
	case Agent::Category::HEAVY_INF: return "Heavy Infantry"; 
	case Agent::Category::LIGHT_CAL: return "Light Cavalry"; 
	case Agent::Category::LIGHT_INF: return "Light Infantry"; 
	default: printf("SHIT TTTTTTT ");  return "War Elephant";
	}
}

Agent::Agent(Name n, Category t, Direction d, int sid, int siz, int mor, int fat, double sr, int ad, int md, double mr, double acc, int armor, int index, vector<int> p, int f, double ratio, double wi, double de, int currentEnemyIndex) :
	name(n), type(t), dir(d), side(sid), size(siz), initial_size(siz), morale(mor), initial_morale(mor), fatigue(fat), sight_range(sr), attack_damage(ad), initial_ad(ad),
	missile_damage(md), initial_md(md), missile_range(mr), accuracy(acc), armor_defence(armor), initial_armor(armor), agent_Index(index), pos(p), initial_pos(p),agent_state(f), missile_range_ratio(ratio),
	width(wi), depth(de), current_enemy_index_this_agent_is_attacking(currentEnemyIndex){}



Agent::Name Agent::getName()
{
	return name;
}
int Agent::getSide()
{
	return side;
}

int Agent::getSize()
{
	return size;
}

int Agent::getInitialSize()
{
	return initial_size;
}

int Agent::getMorale()
{
	return morale;
}

int Agent::getFatigue()
{
	return fatigue;
}

std::vector<int> Agent::getPos()
{
	return pos;
}

Agent::Direction Agent::getDirection()
{
	return dir;
}

Agent::Category Agent::getCategory()
{
	return type;
}

double Agent::getSightRange()
{
	return sight_range;
}

double Agent::getAccuracy()
{
	return accuracy;
}

double Agent::getMissileRange()
{
	return missile_range;
}

double Agent::getArmorDefence()
{
	return (((double)size/(double)initial_size) * armor_defence);
}

int Agent::getFrontLineSize()
{
	return front_line_size;
}

int Agent::getAgentState()
{
	return agent_state;
}
double Agent::getWidth()
{
	return width;
}
double Agent::getDepth()
{
	return depth;
}
std::vector<Agent*> Agent::getNeighbor()
{
	return neighbors;
}
std::vector<Agent*> Agent::getEnemies()
{
	return enemies;
}
std::vector<Agent*> Agent::getEnemies_in_missile()
{
	return enemies_in_missile;
}
int Agent::getIndex()
{
	return agent_Index;
}
double Agent::getHeight(ConReader cr)
{
	return cr.getHeight(pos);
}

int Agent::getCurrentEnemyIndex()
{
	return current_enemy_index_this_agent_is_attacking;
}

bool Agent::getBetrayBit()
{
	return betray;
}

int Agent::getInitialMorale()
{
	return initial_morale;
}

std::vector<int> Agent::getInitialPos()
{
	return initial_pos;
}

int Agent::getInitialAd()
{
	return initial_ad;
}

Agent::Direction Agent::getOppositeDir(Agent::Direction d)
{
	Agent::Direction direction_from;
	switch (d) {
	case Agent::Direction::NORTH: direction_from = Agent::Direction::SOUTH; break;
	case Agent::Direction::SOUTH: direction_from = Agent::Direction::NORTH; break;
	case Agent::Direction::WEST: direction_from = Agent::Direction::EAST; break;
	case Agent::Direction::EAST: direction_from = Agent::Direction::WEST; break;
	case Agent::Direction::NE: direction_from = Agent::Direction::SW; break;
	case Agent::Direction::SW: direction_from = Agent::Direction::NE; break;
	case Agent::Direction::NW: direction_from = Agent::Direction::SE; break;
	default: direction_from = Agent::Direction::NW; break;
	}
	return direction_from;
}

Agent::Direction Agent::getDirectionTowardCenter(Agent::Direction d)
{
	Agent::Direction center;
	switch (d) {
	case Agent::Direction::NORTH: center = Agent::Direction::WEST; break;
	case Agent::Direction::SOUTH: center = Agent::Direction::EAST; break;
	case Agent::Direction::WEST: center = Agent::Direction::SOUTH; break;
	case Agent::Direction::EAST: center = Agent::Direction::NORTH; break;
	case Agent::Direction::NE: center = Agent::Direction::NW; break;
	case Agent::Direction::SW: center = Agent::Direction::SE; break;
	case Agent::Direction::NW: center = Agent::Direction::NE; break;
	default: center = Agent::Direction::SW; break;
	}
	return center;
}

void Agent::setSize(int s)
{
	size = s;
	initial_size = s;
}

void Agent::setMorale(int m)
{
	morale = m;
	initial_morale = m;
}

void Agent::setFatigue(int f)
{
	fatigue = f;
}

void Agent::setAttackDamage(int d)
{
	attack_damage = d;
}

void Agent::setBetrayBit(bool b)
{
	betray = b;
}

void Agent::setSide(int s)
{
	side = s;
}

void Agent::setSightRange(double r)
{
	sight_range = r;
}

void Agent::setMissleRange(double r)
{
	missile_range = r;
}

void Agent::setArmor()
{
	armor_defence = initial_armor;
}

void Agent::setMD()
{
	missile_damage = initial_md;
}

/*
Enemy_index = -1 if this agent is not currently attacking anyone.
*/
void Agent::setCurrentEnemyIndex(int enemy_index)
{
	current_enemy_index_this_agent_is_attacking = enemy_index;
}

void Agent::setFrontLineSize()
{
	switch (name) {
	case Agent::Name::AZAPS:front_line_size = sqrt(size * LIGHT_INF_WIDTH_DEPTH_RATIO); break;
	case Agent::Name::TAMER_HORSE_ARCHER: front_line_size = sqrt(size * TAMER_HORSE_ARCHER_RATIO); break;
	case Agent::Name::ELEPHANT: front_line_size = size; break;
	case Agent::Name::TAMER_RESERVES: front_line_size = sqrt(size * TAMER_RESERVE_RATIO); break;
	default: front_line_size = sqrt(size * DEFAULT_WIDTH_DEPTH_RATIO); break;
	}

}

/*
morale will only influenced by four factors: casualty_rate, fatigue, neighbor and height.
*/
void Agent::updateMorale(ConReader cr)
{
	double remaining_ratio = (double)size / (double)initial_size;
	double morale_d = remaining_ratio * (double)initial_morale;
	morale = (int)morale_d;
	if (agent_Index == 100){
		//printf("size is %d, now morale is %d, state is %s, fatigue is %d, current enemy is %d\n", size, morale, printState().c_str(), fatigue, current_enemy_index_this_agent_is_attacking);
	}
	//influenced by neighbor
	if (is_neighbor_dead()) morale -= 2;
	if (is_neighbor_broken()) morale -= 3;
	if (side == Bayezid && does_neighbor_betray()) morale -= 5;
	if (is_surrounded()) morale -= 2;

	//influenced by height
	if (is_standing_on_high_ground(cr)) morale += 10;

	//influenced by fatigue*************************************************
	if (fatigue >= 60 && fatigue < 100) morale -= 1;
	else if (fatigue >= 100) morale -= 2;

}

/*
Fatigure will increase iff the state is ENGAGED or it is moving.****
In order not to further influence morale, fatigue will not increase if state is RETREAT/BROKEN/FIGHT_TO_DEATH.
*/
void Agent::changeFatigue(int val_to_increase)
{
	fatigue += val_to_increase;
}

void Agent::decrease(int damage)
{
	size -= damage;
}


void Agent::updateShootingRange(ConReader cr)
{
	if (missile_range != 0) missile_range = find_shootingRange(cr);
}

void Agent::updateSightRange(ConReader cr)
{
	sight_range = find_sightRange(cr);
	//printf("sightrange is %f", sight_range);
}

void Agent::changeAgentState(int status)
{
	agent_state = status;
}

void Agent::changeSide()
{
	side = -side;
	betray = true;
}

void Agent::changePos(std::vector<int> position)
{
	pos[0] = position[0];
	pos[1] = position[1];
}

void Agent::changeDirection(Agent::Direction newDir)
{
	dir = newDir;
}

/*
If it is broken, all the ability to attack/defend will disappear.
*/
void Agent::disableAttackAbility()
{
	missile_range = 0;
	missile_damage = 0;
	attack_damage = 0;
	armor_defence = 0;
}

/*
If it is fighting to death, all abilities will increase.
*/
void Agent::strengthenAbilities()
{
	missile_damage += 25 ;
	attack_damage += 25;
	armor_defence += 25;
}

void Agent::strengthenMorale()
{
	morale += 20;
}

void Agent::weakenMorale()
{
	morale -= 2;
}

void Agent::increaseAttackDamage(double rate)
{
	double attack = (double)attack_damage* rate;
	attack_damage = (int) attack ;
}

int Agent::attack_damage_delivered(int special_bonus, double enemy_defend)
{
	double attack;
	//if (fatigue >= 300) {
	//	attack = ((double)attack_damage - enemy_defend - 0.1*(double)fatigue + (double)special_bonus);
	//}
	//else if (fatigue >= 200 && fatigue < 300) {
	//	attack = ((double)attack_damage - enemy_defend - 0.2*(double)fatigue + (double)special_bonus);
	//}
	//else if (fatigue >= 100 && fatigue < 200){
	//	attack = ((double)attack_damage - enemy_defend - 0.3*(double)fatigue + (double)special_bonus);
	//}
	//else {
	//	attack = ((double)attack_damage - enemy_defend - 0.5*(double)fatigue + (double)special_bonus);
	//}

	attack = ((double)attack_damage - enemy_defend + (double)special_bonus- 0.1*(double)fatigue);
	if (attack <= 1.0) return 1;
	else {
		double deliver = ((double)size / (double)initial_size) * attack ;
		if (deliver <= 1.0) return 1;
		else return (int)deliver;
	}
}

int Agent::missile_damage_delivered(double enemy_defend)
{
	double missile = (double)missile_damage * (double)(accuracy) -(double)fatigue - enemy_defend;
	if (missile <= 0) return 1;
	else {
		double deliver = ((double)size / (double)initial_size) * missile;
		return (int)deliver;
	}
}

void Agent::add_neighbor(Agent * neighbor)
{
	neighbors.push_back(neighbor);
}

void Agent::clear_neighbor()
{
	if (neighbors.size() != 0) neighbors.clear();
}

void Agent::add_enemies(Agent * enemy)
{
	enemies.push_back(enemy);
}

void Agent::clear_enemies()
{
	if (enemies.size() != 0) enemies.clear();
}

void Agent::add_enemies_to_shoot(Agent * e)
{
	enemies_in_missile.push_back(e);
}

void Agent::clear_enemies_to_shoot()
{
	if (enemies_in_missile.size() != 0) enemies_in_missile.clear();
}
bool Agent::is_neighbor_dead()
{
	for (Agent * a : neighbors) {
		if ((*a).getAgentState() == DEAD && (*a).getSide() == side) { return true; }
	}
	return false;
}
/*
Returns true if a neighbor of this side is broken.
*/
bool Agent::is_neighbor_broken()
{
	for (Agent * a : neighbors) {
		if ((*a).getAgentState() == BROKEN && (*a).getSide() == side) { return true; }
	}
	return false;
}
/*
Returns true if a neighbor of your side betray.
*/
bool Agent::does_neighbor_betray()
{
	for (Agent * a : neighbors) {
		if ((*a).getBetrayBit()) { return true; }
	}
	return false;
}

bool Agent::is_higher_than_enemy(Agent * e, ConReader cr)
{
	return (cr.getHeight(pos) > cr.getHeight((*e).getPos()));
}

bool Agent::is_surrounded()
{
	if (neighbors.size() <= 3) return false;
	for (Agent * a : neighbors) {
		if ((*a).getSide() == side) { return false; }
	}
	return true;
}

bool Agent::is_standing_on_high_ground(ConReader cr)
{
	double height = cr.getHeight(pos);
	double height_500_in_front = getPos_in_front(1, cr);

	// standing on a higher place
	if (height > height_500_in_front) return true;
	else return false;
		
}

bool Agent::is_betrayable_unit()
{
	return (name == Agent::Name::TARTAR || name == Agent::Name::ANATOLIAN);
}

bool Agent::is_able_to_fight_to_death()
{
	return 
		(name == Agent::Name::JANISSARY || name == Agent::Name::KAPIKULU || name == Agent::Name::SERB_CAL
		|| name == Agent::Name::SERB_INF || name == Agent::Name::TAMERLANE || name == Agent::Name::TAMER_GAURDS);
}

bool Agent::is_size_below_20_percent()
{
	double result = ((double)size) / ((double)initial_size);
	return result <= 0.2;
}

bool Agent::is_morale_below_zero()
{
	return (morale <= 0);
}

bool Agent::is_morale_below_20()
{
	return (morale < 20);
}

bool Agent::is_size_below_50_percent()
{
	double result = ((double)size) / ((double)initial_size);
	return result< 0.5;
}

bool Agent::not_able_to_shoot()
{
	return (name == Agent::Name::SERB_CAL || name == Agent::Name::SERB_INF);
}

bool Agent::is_size_below_1_percent()
{
	double result = (double)size / (double)initial_size;
	return (result <= 0.01);
}


double Agent::find_sightRange(ConReader cr)
{
	double height = cr.getHeight(pos);
	return getPlace_of_max_sight_range(height, cr);	
}

double Agent::find_shootingRange(ConReader cr)
{
	double height = cr.getHeight(pos);
	double height_in_front = getPos_in_front(1, cr); // only get the height of 500m in front.

	double horizontal_dis;
	double shooting_height;
	if (height > height_in_front) { //math for quadratic formula of projectile motion

		horizontal_dis = pow(SHOOTING_SPEED, 2.0) * sin(2 * SHOOTING_DEGREE) / GRAVITATIONAL_ACCELERATION;
		shooting_height = ELEVATION_OF_SHOOT + height - height_in_front;

		//printf("Standing on a higher ground, horizontal_dis is %f, shooting height is %f, missile ratio is %f\n", horizontal_dis, shooting_height, missile_range_ratio);

		return missile_range_ratio * (horizontal_dis + shooting_height * (1.0 / tan(SHOOTING_DEGREE)));
	}
	else if (height == height_in_front) {

		//printf("same altitude");
		return missile_range;
	}
	else {
		// if height in front is higher than the height of this agent
		int height_half_in_front = getPos_in_front(0.5, cr);

		if (height < height_half_in_front) return missile_range * 0.5; // if 250m in the front has higher alt
		else return missile_range * 0.8;
	}

}

int Agent::getRightTriangleSide(int Hypertenus)
{
	return sqrt(pow(Hypertenus, 2) / 2);
}


/*
This method will increase 500m along the direction this agent faces.
*/
double Agent::getPos_in_front(int times, ConReader cr )
{
	vector<int> posInFront;
	switch (dir) {

	case Agent::Direction::NORTH: posInFront = { pos[0], pos[1] - SAMLE_POINT_DISTANCE * times }; break;
	case Agent::Direction::SOUTH: posInFront = { pos[0], pos[1] + SAMLE_POINT_DISTANCE * times }; break;
	case Agent::Direction::EAST: posInFront = { pos[0] + SAMLE_POINT_DISTANCE * times, pos[1] }; break;
	case Agent::Direction::WEST: posInFront = { pos[0] - SAMLE_POINT_DISTANCE * times, pos[1] }; break;
	case Agent::Direction::NE: posInFront = { pos[0] + getRightTriangleSide(SAMLE_POINT_DISTANCE * times), pos[1] - getRightTriangleSide(SAMLE_POINT_DISTANCE*times) };  break;
	case Agent::Direction::NW: posInFront = { pos[0] - getRightTriangleSide(SAMLE_POINT_DISTANCE * times), pos[1] - getRightTriangleSide(SAMLE_POINT_DISTANCE*times) };  break;
	case Agent::Direction::SE: posInFront = { pos[0] + getRightTriangleSide(SAMLE_POINT_DISTANCE * times), pos[1] + getRightTriangleSide(SAMLE_POINT_DISTANCE*times) };  break;
	case Agent::Direction::SW: posInFront = { pos[0] - getRightTriangleSide(SAMLE_POINT_DISTANCE * times), pos[1] - getRightTriangleSide(SAMLE_POINT_DISTANCE*times) };  break;
	}

	if (posInFront[0] <= 0) { posInFront[0] = 0; return -1.0; }
	if (posInFront[1] <= 0) {posInFront[1] = 0; return -1.0;}
	if (posInFront[0] >= SAMLE_POINT_DISTANCE * 19) { posInFront[0] = SAMLE_POINT_DISTANCE * 19; return -1.0; }
	if (posInFront[1] >= SAMLE_POINT_DISTANCE * 24) {posInFront[1] = SAMLE_POINT_DISTANCE * 24; return -1.0; }


	return cr.getHeight(posInFront);
}


double Agent::getPlace_of_max_sight_range(double height, ConReader cr) 
{
	int i = 1;

	double height_in_front = getPos_in_front(i, cr); // get the altitude of a place 500m forward
	while (height_in_front != -1.0    // not out of the battelfield
		&& (height_in_front <= height  || (height_in_front > height && height_in_front <= getPos_in_front(i + 1, cr)))
		)
		// Standing on a higher ground,
		// Or standing on a lower ground, but looking at a hill. the maximum sight range will be the the hilltop
	{
		++i;
		height_in_front = getPos_in_front(i, cr); 
	}
	return i*SAMLE_POINT_DISTANCE;
}



void Agent::print()
{	
	printf("%s	%s	%d	%d	   %d	    %s		%d	%d	%f	%f	%d	%d	(%d, %d)	%f	%d	%d\n", printName().c_str(),
		printCate().c_str(), size, morale, fatigue, printDir().c_str(), attack_damage, missile_damage,
		missile_range, accuracy, armor_defence, agent_Index, pos[0], pos[1], sight_range, (int)width, (int)depth);
}

string Agent::printState()
{
	switch (agent_state) {
	case READY: return "READY";
	case ENGAGED: return "ENGAGED";
	case BROKEN: return "BROKEN";
	case RETREAT: return "RETREAT";
	case DEAD: return "DEAD";
	case FIGHT_TO_DEATH: return "FIGHT TO D";
	}
}

void Agent::print_neighbors()
{
	printf("Agent %d's neighbors: ", agent_Index);
	for (Agent * n : neighbors) { printf("%d, ", (*n).getIndex()); }
	printf("\n");
}

void Agent::print_enemies()
{
	printf("Agent %d's enemies: ", agent_Index);
	for (Agent * n : enemies) { printf("%d, ", (*n).getIndex()); }
	printf("\n");
}


/*
--------------------Builder-----------------------------------------
*/
Agent::Builder & Agent::Builder::initName(Agent::Name name)
{
	this->name = name;
	return *this;
}

Agent::Builder & Agent::Builder::initCate(Agent::Category cate)
{
	this->type = cate;
	return *this;
}

Agent::Builder & Agent::Builder::initDir(Agent::Direction dir)
{
	this->dir = dir;
	return *this;
}

Agent::Builder & Agent::Builder::initPos(std::vector<int> position)
{
	pos.push_back(position[0]);
	pos.push_back(position[1]);
	return *this;
}

Agent::Builder & Agent::Builder::Side(int side)
{
	this->side = side;
	return *this;
}

Agent::Builder & Agent::Builder::initSize(int size)
{
	this->size = size;
	this->initial_size = size;
	return *this;
}

Agent::Builder & Agent::Builder::initMorale(int morale)
{
	this->morale = morale;
	this->initial_morale = morale;
	return *this;
}

Agent::Builder & Agent::Builder::initFatigue(int fatigue)
{
	this->fatigue = fatigue;
	return *this;
}

Agent::Builder & Agent::Builder::initSight(int sightRange)
{
	this->sight_range = sightRange;
	return *this;
}

Agent::Builder & Agent::Builder::initAD(int AD)
{
	this->attack_damage = AD;
	return *this;
}

Agent::Builder & Agent::Builder::initMD(int MD)
{
	this->missile_damage = MD;
	return *this;
}

Agent::Builder & Agent::Builder::initMissile_Range(double MR)
{
	this->missile_range = MR * pow(SHOOTING_SPEED, 2.0) * sin(2 * SHOOTING_DEGREE) / GRAVITATIONAL_ACCELERATION;
	return *this;
}

Agent::Builder & Agent::Builder::initAcc(double accuracy)
{
	this->accuracy = accuracy;
	return *this;
}

Agent::Builder & Agent::Builder::initArmor(int armor)
{
	this->armor_defence = armor;
	return *this;
}

Agent::Builder & Agent::Builder::initAgentIndex(int index)
{
	this->agent_index = index;
	return *this;
}

Agent::Builder & Agent::Builder::initAgentState(int fightState)
{
	this->agent_state = fightState;
	return *this;
}

Agent::Builder & Agent::Builder::initMissileRangeRatio(double ratio)
{
	missile_range_ratio = ratio;
	return *this;
}

Agent::Builder & Agent::Builder::initWidth(double width)
{
	this->width = width;
	return *this;
}

Agent::Builder & Agent::Builder::initDepth(double depth)
{
	this->depth = depth;
	return *this;
}

Agent::Builder & Agent::Builder::initCurrentEenemy(int index)
{
	this->current_enemy_index_this_agent_is_attacking = index;
	return *this;
}

Agent:: Builder & Agent::Builder::initBetrayBoolean(bool b)
{
	this->betray = b;
	return *this;
}


Agent* Agent::Builder::build()
{
	return new Agent(this->name, this->type, this->dir, this->side, this->size, this->morale, this->fatigue, this->sight_range, 
		this->attack_damage, this->missile_damage, this->missile_range, this->accuracy, this->armor_defence, this->agent_index,
		this->pos, this->agent_state, this->missile_range_ratio, this->width, this->depth, this->current_enemy_index_this_agent_is_attacking);
}




