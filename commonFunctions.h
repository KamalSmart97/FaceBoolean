#pragma once
#include <cmath>
#include <list> 

double distanceBetweenPoints(double* pt1, double* pt2);


inline double Det(double a, double b, double c, double d);

bool LineLineIntersect(double* pXY1, //Line 1 start
    double* pXY2, //Line 1 end
    double* pXY3, //Line 2 start
    double* pXY4, //Line 2 end
    double* pIntersectionPt); //Output 



