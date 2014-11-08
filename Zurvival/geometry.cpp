#include "geometry.h"

Point::Point(double x, double y) {
	this->x = x;
	this->y = y;
}

Point::Point(const Point &p) {
	this->x = p.x;
	this->y = p.y;
}

Point::Point() {

}

Point::~Point() {

}
Point Point::operator=(const Point& p) {
	return Point(p.x, p.y);
}
Point Point::operator+(const Point& p)
{
	Point ret(p.x,p.y);
	ret.x += this->x;
	ret.y += this->y;
	return ret;
}

Point Point::operator-(const Point& p)
{
	Point ret(p.x, p.y);
	ret.x -= this->x;
	ret.y -= this->y;
	return ret;
}

double Point::operator^(const Point& p)
{
	return this->x * p.y - this->y * p.x;
	/*int cross = AB[0] * AC[1] - AB[1] * AC[0];*/
}

double Point::operator*(const Point& p)
{
	return this->x * p.x + this->y * p.y;
	/*int dot = AB[0] * BC[0] + AB[1] * BC[1];*/
}

double Point::distance(const Point& p) {
	double dx = this->x - p.x;
	double dy = this->y - p.y;
	return sqrt(dx*dx + dy*dy);
}

double Point::angleTo(const Point& p) {
	double dx = p.x - this->x;
	double dy = p.y - this->y;
	return atan2(dy, dx);
}

Segment::Segment(Point p0, Point p1) {
	this->p0.x = p0.x;
	this->p1.x = p1.x;
	this->p0.y = p0.y;
	this->p1.y = p1.y;
}

Segment::~Segment() {

}

Circle::Circle(Point c, double r) {
	this->c.x = c.x;
	this->c.y = c.y;
	this->r = r;
}

Circle::~Circle() {

}

double linePointDist(Point A, Point B, Point C, bool isSegment){
	double dist = ((B - A) ^ (C - A)) / sqrt((B - A)*(B - A));
	if (isSegment){
		int dot1 = (C - B)*(B - A);
		if (dot1 > 0)return sqrt((B - C)*(B - C));
		int dot2 = (C - A)*(A - B);
		if (dot2 > 0)return sqrt((A - C)*(A - C));
	}
	return abs(dist);
}

bool collide(Segment s, Circle c) {
	double dist = linePointDist(s.p0, s.p1, c.c,true);
	return dist <= c.r;
}