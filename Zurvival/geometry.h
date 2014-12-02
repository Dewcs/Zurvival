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

class V3 {
public:
	float x, y, z;
	V3(float x, float y, float z);
	float dist(const V3& other) {
		return sqrt((x - other.x)*(x - other.x) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z));
	}
	float angleTo(const V3& other) {
		float scalar = dotProduct(other);
		float bot = sqrt(x*x + y*y + z*z)*sqrt(other.x*other.x + other.y*other.y + other.z*other.z);
		return acos(scalar / bot);  
	}
	float dotProduct(const V3 &other) {
		return x*other.x + y*other.y + z*other.z;
	}
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