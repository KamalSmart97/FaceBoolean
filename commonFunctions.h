#pragma once
#include <cmath>
#include <list> 

double distanceBetweenPoints(double* pt1, double* pt2)
{
	double len = 0.0;

	len = (pt2[1] - pt1[1]) * (pt2[1] - pt1[1]) + (pt2[0] - pt1[0]) * (pt2[0] - pt1[0]);

	len = sqrt(len);

	return len;
}


