#include "geometry.h"

#include <cmath>

const float CURVE_PRECISION = 100.0;

namespace cppgef
{

double pointToPointDistance(const Point& point1, const Point& point2)
{
	return sqrt((std::fabs(point1.getX() - point2.getX()) * std::fabs(point1.getX() - point2.getX())) +
			(std::fabs(point1.getY() - point2.getY()) * std::fabs(point1.getY() - point2.getY())));	
}

double pointToLineDistance(const Point& line_start, const Point& line_end, const Point& point)
{
	double A = point.getX() - line_start.getX();
	double B = point.getY() - line_start.getY();
	double C = line_end.getX() - line_start.getX();
	double D = line_end.getY() - line_start.getY();

	double dot = A * C + B * D;
	double len_sq = C * C + D * D;
	double param = dot / len_sq;

	Point intersect_point;

	if (param < 0)
	{
		intersect_point.set (line_start.getX(), line_start.getY());
	}
	else if (param > 1)
	{
		intersect_point.set (line_end.getX(), line_end.getY());
	}
	else
	{
		intersect_point.set (line_start.getX() + param * C, line_start.getY() + param * D);
	}

	return pointToPointDistance (point, intersect_point);	
}

void createBezierPoints(vector< Point >& points, const Point& curve_start, const Point& start_control_point, const Point& end_control_point, const Point& curve_end)
{
	points.clear();
	
	float step = 1.0 / CURVE_PRECISION;
	for (int i = 0; i <= CURVE_PRECISION; i++)
	{
		float t = i*step;
		float q1 = t*t*t*(-1)+t*t*3+t*(-3)+1;
		float q2 = t*t*t*3+t*t*(-6)+t*3;
		float q3 = t*t*t*(-3)+t*t*3;
		float q4 = t*t*t;
		
		float qx = q1 * curve_start.getX() +
			q2 * start_control_point.getX() +
			q3 * end_control_point.getX() +
			q4 * curve_end.getX();
			
		float qy = q1 * curve_start.getY() +
			q2 * start_control_point.getY() +
			q3 * end_control_point.getY() +
			q4 * curve_end.getY();

		points.push_back (Point( qx, qy ));
	}	
}

Rectangle boundsPoints(const vector< Point >& points)
{
	int xmin = points[0].getX();
	int ymin = points[0].getY();
	int xmax = xmin;
	int ymax = ymin;
	
	for (size_t i = 1; i < points.size(); i++)
	{
		if (points[i].getX() < xmin)
			xmin = points[i].getX();
			
		if (points[i].getY() < ymin)
			ymin = points[i].getY();
			
		if (points[i].getX() > xmax)
			xmax = points[i].getX();
			
		if (points[i].getY() > ymax)
			ymax = points[i].getY();
	}
	
	return Rectangle(Point( xmin, ymin ), Point( xmax, ymax));
}

}

