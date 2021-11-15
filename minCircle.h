/*
 * minCircle.h
 *
 * Author: Noa Eitan 316222777, Coral Kuta 208649186
 */

#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <vector>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>
#include "anomaly_detection_util.h"

using namespace std;


// ------------ DO NOT CHANGE -----------

class Circle{
public:
	Point center;
	float radius;
	Circle(Point c,float r):center(c),radius(r){}
};
// ---------------------+-----------------

Circle findMinCircle(Point** points,size_t size);
bool is_inside(const Circle& c, const Point& p);
Point get_circle_center(float bx, float by, float cx, float cy);
Circle circle_from(const Point& A, const Point& B, const Point& C);
Circle circle_from(const Point& A, const Point& B);
bool is_valid_circle(const Circle& c, const vector<Point>& P);
Circle min_circle_trivial(vector<Point>& P);
Circle welzl(vector<Point>& to_process, vector<Point> boundary, int size);
float distance(Point a, Point b);

// you can add here additional methods

#endif /* MINCIRCLE_H_ */
