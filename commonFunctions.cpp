#include"commonFunctions.h"

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

bool LineLineIntersect(double* pXY1, //Line 1 start
    double* pXY2, //Line 1 end
    double* pXY3, //Line 2 start
    double* pXY4, //Line 2 end
    double*pIntersectionPt) //Output 
{
    double detL1 = Det(pXY1[0], pXY1[1], pXY2[0], pXY2[1]);
    double detL2 = Det(pXY3[0], pXY3[1], pXY4[0], pXY4[1]);
    double x1mx2 = pXY1[0] - pXY2[0];
    double x3mx4 = pXY3[0] - pXY4[0];
    double y1my2 = pXY1[1] - pXY2[1];
    double y3my4 = pXY3[1] - pXY4[1];

    double xnom = Det(detL1, x1mx2, detL2, x3mx4);
    double ynom = Det(detL1, y1my2, detL2, y3my4);
    double denom = Det(x1mx2, y1my2, x3mx4, y3my4);
    if (denom == 0.0)//Lines don't seem to cross
    {
        pIntersectionPt[0] = NAN;
        pIntersectionPt[1] = NAN;
        return false;
    }

    pIntersectionPt[0] = xnom / denom;
    pIntersectionPt[1] = ynom / denom;

    return true; //All OK
}