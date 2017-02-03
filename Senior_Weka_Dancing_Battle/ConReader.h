
#ifndef _CONREADER_H
#define _CONREADER_H

#include <vector>

class ConReader{
private:
	std::vector <double> altitude;
	int getIndex(int x, int y);
	

public:
	void fancyPrint();
	int loadAltitude();
	double getHeight(std::vector<int>);

};


#define SAMLE_POINT_DISTANCE 500
#define WE_WIDTH 20
#define NS_LENGTH 25

#endif
