#pragma once
#include <math.h>
#include "Functions.h"

class Point {
	public:
		double x, y;
		Point();
		Point(double x, double y);
		Point(const Point &p);
		~Point();
		Point operator=(const Point& p);
		Point operator+(const Point& p);
		Point operator-(const Point& p);
		double operator*(const Point& p);
		double operator^(const Point& p);
		double distance(const Point& p);
		double angleTo(const Point& p);
};

class Segment {
public:
	Point p0, p1;
	Segment(Point p0,Point p1);
	~Segment();
};

class Circle {
public:
	Point c;
	double r;
	Circle(Point c, double r);
	~Circle();
};

double linePointDist(Point A, Point B, Point C, bool isSegment);

bool collide(Segment s, Circle c);