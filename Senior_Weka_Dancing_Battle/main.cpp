#include "battle.h"

/*Attributes*/
int march_from_constantinople; //1 means yes, will influence initial fatigue
int is_ottoman_deffensive; //1 means initially (Ottoman will remain idle, Tamerlane attack), otherwise reverse.
int is_water_poisoned;
int any_betrayal; //if 1, Ottoman Tartar and random 60%of Anatolian will be able to betray
double size_increase_ratio;
int rounds;

int battle();
int run_one_battle(int constantinople, int deffensive, int poison, int betrayal, double size_incease_ratio, int roun);

int battle() {
	march_from_constantinople = 0;
	is_ottoman_deffensive = 1;
	is_water_poisoned = 0;
	any_betrayal = 0;
	size_increase_ratio = 1;
	rounds = 300;

	/*
	hunter_game_before_battle = 0;
	did_not_get_paid = 0;*/

	return -1;
}

int run_one_battle(int constantinople, int deffensive, int poison, int betrayal, double size_incease_ratio, int rounds)
{
	return -1;
}
