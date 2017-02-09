#pragma once

#ifndef _AGENT_H
#define _AGENT_H

#include "ConReader.h"
#include <vector>

#define MAX_SIGHT_RANGE 4000
#define BASIC_Shooting_Range 300
#define BASIC_SIGHT_RANGE 500
#define HEIGHT_OF_PERSON 1.7
#define ELEVATION_OF_SHOOT 1.5
#define SHOOTING_SPEED 47.80
#define GRAVITATIONAL_ACCELERATION 9.8
#define SHOOTING_DEGREE 45

#define Bayezid 999
#define Tamerlane -999

#define BROKEN 777
#define RETREAT 666
#define ENGAGED 555
#define FIGHT_TO_DEATH 444
#define READY 333
#define DEAD 222

#define LIGHT_INF_WIDTH_DEPTH_RATIO 10
#define DEFAULT_WIDTH_DEPTH_RATIO 5/2
#define TAMER_HORSE_ARCHER_RATIO 84/50
#define TAMER_RESERVE_RATIO 5/4

#define CAVALRY_WIDTH 3
#define CAVALRY_DEPTH 4
#define HEAVY_INF_WIDTH 1
#define HEAVY_INF_DEPTH 1.5
#define LIGHT_INF_WIDTH 2
#define LIGHT_INF_DEPTH 2
#define WAR_ELEPHANT_WIDTH 10
#define WAR_ELEPHANT_DEPTH 15

#define MINIMUM_GAPE 10

class Agent {
public:
	enum class Direction { NORTH, SOUTH, EAST, WEST, NE, NW, SE, SW };
	enum class Category { HEAVY_CAL, HEAVY_INF, LIGHT_CAL, LIGHT_INF, WAR_ELEPHANT };
	enum class Name { JANISSARY, KAPUKULU, AZAPS, ANATOLIAN, RUMELIAN, TARTAR, SERB_INF, SERB_CAL, 
		OTTO_RESERVES, TAMER_HORSE_ARCHER, TAMER_GAURDS, TAMERLANE,TAMER_RESERVES, ELEPHANT};

	class Builder;
	bool in_battlefield = true;
	bool is_alive = true;
	bool is_being_attacked = false;

	Agent::Name getName();
	int getSide();
	int getSize();
	int getInitialSize();
	int getMorale();
	int getFatigue();
	std::vector<int> getPos();
	Agent::Direction getDirection();
	Agent::Category getCategory();
	double getSightRange();
	double getAccuracy();
	double getMissileRange();
	double getArmorDefence();
	int getFrontLineSize();
	int getAgentState();
	double getWidth();
	double getDepth();
	std::vector<Agent *> getNeighbor();
	std::vector<Agent *> getEnemies();
	std::vector<Agent *> getEnemies_in_missile();
	int getIndex();
	double getHeight(ConReader); 
	int getCurrentEnemyIndex();
	bool getBetrayBit();//GETTERS
	/*-----------------------------------------*/

	void setCurrentEnemyIndex(int enemy_index);
	void setFrontLineSize();
	void updateMorale(ConReader cr);
	void updateShootingRange(ConReader cr);
	void updateSightRange(ConReader cr);

	void changeFatigue(int);
	void decrease(int);
	void changeAgentState(int status);
	void changeSide();
	void changePos(std::vector<int>);
	void changeDirection(Agent::Direction);
	void disableAttackAbility();
	void strengthenAbilities();
	void strengthenMorale();
	void weakenMorale();
	void increaseAttackDamage(double rate);
	
	int attack_damage_delivered(int special_bonus, double enemy_defend);
	int missile_damage_delivered(double enemy_defend);	

	void add_neighbor(Agent * neighbor);
	void add_enemies(Agent * enemy);
	void add_enemies_to_shoot(Agent * e);

	void clear_neighbor();	
	void clear_enemies();	
	void clear_enemies_to_shoot();

	bool is_neighbor_broken();
	bool does_neighbor_betray();
	bool is_surrounded();
	bool is_standing_on_high_ground(ConReader cr);
	bool is_betrayable_unit();
	bool is_able_to_fight_to_death();
	bool is_size_below_20_percent();
	bool is_morale_below_zero();
	bool is_morale_below_10();
	bool is_size_below_50_percent();
	bool not_able_to_shoot();

	double find_sightRange( ConReader cr);
	double find_shootingRange(ConReader cr);

	/*------------print--------------------------------*/
	void print();
	void print_neighbors();
	void print_enemies();
	std::string printName();

private:
	Name name;
	Category type;
	Direction dir;
	int side;
	std::vector<int> pos;
	int size;
	int initial_size;
	int initial_morale;
	int morale;
	int fatigue;
	int front_line_size;
	double width;
	double depth;
	double sight_range;
	int attack_damage;
	int missile_damage;
	double missile_range;
	double accuracy;
	int armor_defence;
	int agent_Index;
	int agent_state;
	double missile_range_ratio;
	std::vector<Agent *> neighbors;
	std::vector<Agent *> enemies;
	std::vector<Agent *> enemies_in_missile;
	int current_enemy_index_this_agent_is_attacking;
	bool betray;

	std::string printDir();
	std::string printCate();

	double getPlace_of_max_sight_range(double height, ConReader cr);
	double getPos_in_front(int times, ConReader cr);
	int getRightTriangleSide(int Hypertenus);

	Agent(Name n, Category t, Direction d, int sid, int siz, int mor, int fat, 
		double sr, int ad, int md, double mr, double acc, int armor, int index, 
		std::vector<int> pos, int fightState, double ratio, double width, 
		double depth, int currentEnemyIndex, bool betray);

};

class Agent:: Builder {
private:
	Agent::Name name;
	Agent::Category type;
	Agent::Direction dir;
	int side;
	std::vector<int> pos;
	int size;
	int initial_size;
	int morale;
	int fatigue;
	double sight_range;
	double width;
	double depth;
	int attack_damage;
	int missile_damage;
	double missile_range;
	double accuracy;
	int armor_defence;
	int agent_index;
	//int speed;
	int agent_state;
	double missile_range_ratio;
	int current_enemy_index_this_agent_is_attacking;
	bool betray; 
	int initial_morale;

public:
	Builder& initName(Agent::Name name);
	Builder& initCate(Agent::Category cate);
	Builder& initDir(Agent::Direction);
	Builder& initPos(std::vector<int> position);
	Builder& Side(int side);
	Builder& initSize(int size);
	Builder& initMorale(int morale);
	Builder& initFatigue(int fatigue);
	Builder& initSight(int sightRange);
	Builder& initAD(int AD);
	Builder& initMD(int MD);
	Builder& initMissile_Range(double MR);
	Builder& initAcc(double accuracy);
	Builder& initArmor(int armor);
	Builder& initAgentIndex(int index);
	//Builder& initSpeed(int speed);
	Builder& initAgentState(int fightState);
	Builder& initMissileRangeRatio(double ratio);
	Builder& initWidth(double width);
	Builder& initDepth(double depth);
	Builder& initCurrentEenemy(int index);
	Builder& initBetrayBoolean(bool b);
	Builder& initInitialMorale(int morale);

	Agent* build();

};

#endif // !_AGENT_H
