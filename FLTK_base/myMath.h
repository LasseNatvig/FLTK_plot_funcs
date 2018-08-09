#pragma once
struct xyPair { double x, y; };
vector<xyPair> calcPoly(int from, int to, int steps, int aCoeff, int bCoeff, int cCoeff);
double findMax_absolute_Yvalue(vector<xyPair> values);