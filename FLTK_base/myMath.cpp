#pragma once
#include "std_lib_facilities.h"
#include "myMath.h"
#include <assert.h>

vector<xyPair> calcPoly(int from, int to, int steps, int aCoeff, int bCoeff, int cCoeff) {
	assert(to > from);
	vector<xyPair> values;
	for (int i = 0; i <= steps; i++) {
		double x = from + (i*((to - from) / steps));   // Sjekk at siste blir med, teste med få steg ...  // antar trenger cast
		xyPair p;
		p.x = x;
		p.y = (aCoeff * x * x) + (bCoeff * x) + cCoeff;
		values.push_back(p);
	}
	return values;
}
double findMax_absolute_Yvalue(vector<xyPair> values) {
	if (values.size() == 0) {
		cout << " error";
		return -1;
	}
	else {
		double min = values[0].y;
		double max = values[0].y;
		for (unsigned int i = 1; i < values.size(); i++) {
			if (min > values[i].y) min = values[1].y;
			if (max < values[i].y) max = values[i].y;
		}
		if (min < 0) min = -min;   // Take absolute value
		if (min > max) 	return min;
		else return max;
	}
}