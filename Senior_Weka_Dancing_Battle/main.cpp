#include <iostream>
#include <iomanip> 
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <functional>
#include <string>
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
void write_mean_to_each_file();

void singleRun();

int main() {

	march_from_constantinople = 0;
	is_ottoman_offensive = 2;
	is_water_poisoned = 0;
	any_betrayal = false;
	size_increase = 0;
	rounds = 150;
	
	
	//write_mean_to_each_file();
	//run(rounds);
	singleRun();
}

void singleRun() {

	battle1.cr.loadAltitude();

	march_from_constantinople = 1;
	is_ottoman_offensive = 0;
	is_water_poisoned = 0;
	any_betrayal = true;
	size_increase = 0;

	ofstream RESULTFILE("data/results.csv", ios::app);
	if (RESULTFILE) {
		RESULTFILE << "Constantinople, Offensive, Poisoned, Betrayal, Size Increase, End Rounds, Given Rounds, Result, O_Casualty,T_Casualty, Trend\n";

		int i =4444;
		RESULTFILE << march_from_constantinople << "," << is_ottoman_offensive << " ," << is_water_poisoned << "," << any_betrayal << "," << size_increase << ",";

		battle1.first_time_populate(is_water_poisoned, march_from_constantinople, size_increase, any_betrayal);

		battle1.simple_result_of_one_battle(RESULTFILE, i,
			is_ottoman_offensive,
			any_betrayal,
			march_from_constantinople,
			is_water_poisoned,
			size_increase,
			300);

		///battle1.ivm.remapping(is_water_poisoned, march_from_constantinople, any_betrayal, battle1.cr);
		battle1.ivm.deleteAllAgent();
	}
}

void run(int rounds)
{
	battle1.cr.loadAltitude();
	

	ofstream RESULTFILE("data/results.csv", ios::app);
	if (RESULTFILE) {
		RESULTFILE << "Constantinople, Offensive, Poisoned, Betrayal, Size Increase, End Rounds, Given Rounds, Result, O_Casualty,T_Casualty\n";
		
		int i = 0;
		for (int k = 0; k <= 1; ++k) {
			for (is_water_poisoned = 0; is_water_poisoned <= 1; ++is_water_poisoned) {
				for (march_from_constantinople = 0; march_from_constantinople <= 1; ++march_from_constantinople) {
					for (is_ottoman_offensive = 0; is_ottoman_offensive <= 2; ++is_ottoman_offensive) {
						for (size_increase = 0; size_increase <= 5; ++size_increase) {

							for (int x = 0; x < 20; ++x) {

								RESULTFILE << march_from_constantinople << "," << is_ottoman_offensive << " ," << is_water_poisoned << "," << any_betrayal << "," << size_increase << ",";

								battle1.first_time_populate(is_water_poisoned, march_from_constantinople, size_increase, any_betrayal);

								battle1.simple_result_of_one_battle(RESULTFILE, i,
									is_ottoman_offensive,
									any_betrayal,
									march_from_constantinople,
									is_water_poisoned,
									size_increase,
									300);

								///battle1.ivm.remapping(is_water_poisoned, march_from_constantinople, any_betrayal, battle1.cr);
								battle1.ivm.deleteAllAgent();

								i++;
							}
						}
					}
				}
			}
			any_betrayal = true;
		}
	}
	else {
		printf("Cannot open result.csv to write*********************\n");
	}

	battle1.deleteAllAgent();
}

void write_mean_to_each_file()
{

	int index = 0, round = 0;
	
	vector<double> o, t;
	ifstream read;
	read.open(("data/casualty_per_round_" + std::to_string(index) + ".csv").c_str());
	ofstream write("data/compareRandomness.csv", ios::app);

	write << "File index, O_mean, O_STD, T_mean, T_STD\n";

	while (read && write) {

		printf("now index is %d\n", index);
		string line;
		double variance = 0.0, stdev = 0.0, mean = 0.0, sum = 0.0;

		getline(read, line);
		while (getline(read, line)) {
			if (line.empty()) continue;

			istringstream iss(line);

			double o_casualty, t_casualty; char ch;
			if (iss >> ch >> round >> ch >> o_casualty >> ch >> t_casualty >> ch) {

				o.push_back(o_casualty);
				t.push_back(t_casualty);

			}
		}
		for (int i = 0; i < o.size(); ++i) { sum += o[i]; }
		mean = sum / o.size();
		for (int i = 0; i < o.size(); ++i) { variance += pow(o[i] - mean, 2); }
		stdev = sqrt(variance / o.size());

		write << index << "," << mean << "," << stdev;

		variance = 0.0, stdev = 0.0, mean = 0.0, sum = 0.0;
		for (int i = 0; i < t.size(); ++i) { sum += t[i]; }
		mean = sum / t.size();
		for (int i = 0; i < t.size(); ++i) { variance += pow(t[i] - mean, 2); }
		stdev = sqrt(variance / t.size());

		write << "," << mean << "," << stdev << "\n";

		o.clear(); t.clear();

		++index;
		read.close();
		read.open(("data/casualty_per_round_" + std::to_string(index) + ".csv").c_str());
		if (!read) {
			printf("cannot open file: %s\n", ("data/casualty_per_round_" + std::to_string(index) + ".csv").c_str());
		}
	}
	read.close();
	write.close();
}
