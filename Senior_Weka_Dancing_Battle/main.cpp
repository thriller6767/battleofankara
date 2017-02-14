#include <iostream>
#include <vector>
#include <functional>
#include "ConReader.h"
#include "Agent.h"
#include "Initial_val_mapper.h"
#include "Battle.h"
#include "RangeSearch.h"


using namespace std;
Battle battle1;

int march_from_constantinople;
int is_ottoman_offensive;
int is_water_poisoned;
bool any_betrayal;
int size_increase_ratio;
int rounds ;
void run(int);

int main() {

	march_from_constantinople = 0;
	is_ottoman_offensive = 0;
	is_water_poisoned = 0;
	any_betrayal = false;
	size_increase_ratio = 0;
	rounds = 150;
	
	

	run(rounds);
}

void run(int rounds)
{
	battle1.cr.loadAltitude();
	ofstream RESULTFILE("results.csv", ios::app);
	battle1.first_time_populate(is_water_poisoned, march_from_constantinople, any_betrayal);

	if (RESULTFILE) {
		RESULTFILE << "Constantinople, Offensive, Poisoned, Betrayal, Size Increase, End Rounds, Given Rounds, Result, O_Casualty, O_Left_Rate, T_Casualty, T_left_Rate, Trend\n";
		int i = 3;
		while (i <= 6) {

			RESULTFILE << march_from_constantinople << "," << is_ottoman_offensive << " ," << is_water_poisoned << "," << any_betrayal << "," << size_increase_ratio << ",";

			battle1.simple_result_of_one_battle(RESULTFILE, i, is_ottoman_offensive, any_betrayal, march_from_constantinople, is_water_poisoned, size_increase_ratio, 170);
			i++;
		}
	}

	battle1.deleteAllAgent();
}
