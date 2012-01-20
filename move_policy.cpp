#include "move_policy.h"

#include <line_figure.h>
#include <curve_figure.h>

namespace cppgef
{

//////////////////////////////////////////////////////////////////////////////////////

void MoveStartPointPolicy::moveTo(shared_ptr< Figure > figure, int dx, int dy)
{
	shared_ptr< LineFigure > line_figure = dynamic_pointer_cast< LineFigure >(figure);
	
	if (line_figure)
	{
		Point pt = line_figure->getStartPoint();
		pt.moveRel (dx, dy);
		line_figure->setStartPoint (pt);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void MoveEndPointPolicy::moveTo(shared_ptr< Figure > figure, int dx, int dy)
{
	shared_ptr< LineFigure > line_figure = dynamic_pointer_cast< LineFigure >(figure);
	
	if (line_figure)
	{
		Point pt = line_figure->getEndPoint();
		pt.moveRel (dx, dy);
		line_figure->setEndPoint (pt);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void MoveCurveStartPointPolicy::moveTo(shared_ptr< Figure > figure, int dx, int dy)
{
	shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >(figure);
	
	if (curve_figure)
	{
		curve_figure->getStartPoint().moveRel (dx, dy);
		curve_figure->getStartControlPoint().moveRel (dx, dy);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void MoveCurveEndPointPolicy::moveTo(shared_ptr< Figure > figure, int dx, int dy)
{
	shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >(figure);
	
	if (curve_figure)
	{
		curve_figure->getEndPoint().moveRel (dx, dy);
		curve_figure->getEndControlPoint().moveRel (dx, dy);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void MoveCurveStartControlPointPolicy::moveTo(shared_ptr< Figure > figure, int dx, int dy)
{
	shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >(figure);
	
	if (curve_figure)
	{
		Point pt = curve_figure->getStartControlPoint();
		pt.moveRel (dx, dy);
		curve_figure->setStartControlPoint (pt);
	}
}

//////////////////////////////////////////////////////////////////////////////////////

void MoveCurveEndControlPointPolicy::moveTo(shared_ptr< Figure > figure, int dx, int dy)
{
	shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >(figure);
	
	if (curve_figure)
	{
		Point pt = curve_figure->getEndControlPoint();
		pt.moveRel (dx, dy);
		curve_figure->setEndControlPoint (pt);
	}
}

}

