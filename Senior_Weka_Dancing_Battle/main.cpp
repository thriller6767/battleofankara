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
int size_increase;
int rounds ;
void run(int);

int main() {

	march_from_constantinople = 0;
	is_ottoman_offensive = 0;
	is_water_poisoned = 0;
	any_betrayal = false;
	size_increase = 0;
	rounds = 150;
	
	

	run(rounds);
}

void run(int rounds)
{
	battle1.cr.loadAltitude();
	

	ofstream RESULTFILE("data/results.csv", ios::app);
	if (RESULTFILE) {
		RESULTFILE << "Constantinople, Offensive, Poisoned, Betrayal, Size Increase, End Rounds, Given Rounds, Result, O_Casualty,T_Casualty, Trend,\n";
		
		int i = 0, k = 0;
		while (k <= 1){
			while (is_water_poisoned <= 1) {
				while (march_from_constantinople <= 1) {
					while (is_ottoman_offensive <= 2){
						while (size_increase <= 6) {

							int x = 0;
							while (x < 20) {

								RESULTFILE << march_from_constantinople << "," << is_ottoman_offensive << " ," << is_water_poisoned << "," << any_betrayal << "," << size_increase << ",";

								battle1.first_time_populate(is_water_poisoned, march_from_constantinople, size_increase, any_betrayal);

								battle1.simple_result_of_one_battle(RESULTFILE, i,
									is_ottoman_offensive,
									any_betrayal,
									march_from_constantinople,
									is_water_poisoned,
									size_increase,
									250);

								///battle1.ivm.remapping(is_water_poisoned, march_from_constantinople, any_betrayal, battle1.cr);
								battle1.ivm.deleteAllAgent();

								x++;
								i++;
							}
							size_increase++;
						}
						is_ottoman_offensive++;
					}
					march_from_constantinople++;
				}
				is_water_poisoned++;
			}
			any_betrayal = true;
		}
	}
	else {
		printf("Cannot open result.csv to write*********************\n");
	}

	battle1.deleteAllAgent();
}
