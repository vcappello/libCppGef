#ifndef GEOMETRY_H_
#define GEOMETRY_H_

#include <vector>

#include <point.h>
#include <rectangle.h>

using std::vector;

namespace cppgef
{

double pointToPointDistance(const Point& point1, const Point& point2);

double pointToLineDistance(const Point& line_start, const Point& line_end, const Point& point);

void createBezierPoints(vector< Point >& points, const Point& curve_start, const Point& start_control_point, const Point& end_control_point, const Point& curve_end);

Rectangle boundsPoints(const vector< Point >& points);

}

#endif // GEOMETRY_H_

