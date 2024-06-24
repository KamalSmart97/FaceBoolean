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

inline double Det(double a, double b, double c, double d)
{
    return a * d - b * c;
}

bool LineLineIntersect(double x1, double y1, //Line 1 start
    double x2, double y2, //Line 1 end
    double x3, double y3, //Line 2 start
    double x4, double y4, //Line 2 end
    double& ixOut, double& iyOut) //Output 
{

    double detL1 = Det(x1, y1, x2, y2);
    double detL2 = Det(x3, y3, x4, y4);
    double x1mx2 = x1 - x2;
    double x3mx4 = x3 - x4;
    double y1my2 = y1 - y2;
    double y3my4 = y3 - y4;

    double xnom = Det(detL1, x1mx2, detL2, x3mx4);
    double ynom = Det(detL1, y1my2, detL2, y3my4);
    double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
    if (denom == 0.0)//Lines don't seem to cross
    {
        ixOut = NAN;
        iyOut = NAN;
        return false;
    }

    ixOut = xnom / denom;
    iyOut = ynom / denom;

    return true; //All OK
}


