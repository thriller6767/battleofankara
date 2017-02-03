#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>
#include "ConReader.h"

using namespace std;

int ConReader::getIndex(int x, int y) { return x + WE_WIDTH * y; }

void ConReader::fancyPrint()
{
	for  (int i =0 ; i< altitude.size(); ++i){
		if (i % 20 == 0) {
			printf("\n  ROW %d:", i / 20 + 1);	
		}
		printf("%f |", altitude[i]);
	}
}

int ConReader::loadAltitude() {
	ifstream inFile("battlefield.txt");
	if (!inFile) {
		cerr << "File battlefield.txt not fould." << endl;
		return -1;
	}

	string line;
	double alt;
	getline(inFile, line);

	while (getline(inFile, line)) {
		if (line.empty()) continue;
		istringstream iss(line);

		while (iss >> alt)
			altitude.push_back(alt);
	}
}

double ConReader::getHeight(vector<int> pos)
{
	int nsPoint = (int)(pos[1] / SAMLE_POINT_DISTANCE + 0.5);
	int wePoint = (int)(pos[0] / SAMLE_POINT_DISTANCE + 0.5);

	return altitude[getIndex(wePoint, nsPoint)];
}





