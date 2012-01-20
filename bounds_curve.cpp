#include "bounds_curve.h"

#include <curve_line.h>
#include <curve_figure.h>

namespace cppgef
{

BoundsCurve::BoundsCurve()
{
}

BoundsCurve::~BoundsCurve()
{
}

void BoundsCurve::setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds)
{
	shared_ptr< CurveLine > curve_element = dynamic_pointer_cast< CurveLine >(element);
	
	if (curve_element)
	{
		Point start_pt;
		Point end_pt;
		Point start_ctrl_pt;
		Point end_ctrl_pt;
		
		computePoints (bounds, start_pt, end_pt, start_ctrl_pt, end_ctrl_pt);
		
		curve_element->setStartPoint (start_pt);
		curve_element->setEndPoint (end_pt);
		curve_element->setStartControlPoint (start_ctrl_pt);
		curve_element->setEndControlPoint (end_ctrl_pt);
	}	
}

void BoundsCurve::setBounds(shared_ptr<Figure> figure, const Rectangle& bounds)
{
	shared_ptr< CurveFigure > curve_figure = dynamic_pointer_cast< CurveFigure >(figure);
	
	if (curve_figure)
	{
		Point start_pt;
		Point end_pt;
		Point start_ctrl_pt;
		Point end_ctrl_pt;
		
		computePoints (bounds, start_pt, end_pt, start_ctrl_pt, end_ctrl_pt);
		
		curve_figure->setStartPoint (start_pt);
		curve_figure->setEndPoint (end_pt);
		curve_figure->setStartControlPoint (start_ctrl_pt);
		curve_figure->setEndControlPoint (end_ctrl_pt);
	}		
}

void BoundsCurve::computePoints(const Rectangle& bounds, Point& start_point, Point& end_point, Point& start_control_point, Point& end_control_point)
{
	start_point = bounds.getLocation();

	end_point = start_point;
	end_point.moveRel (bounds.getSize());

	int dx = 0;
	if (bounds.getSize().getWidth() != 0)
		dx = bounds.getSize().getWidth() / 2;

	int dy = 0;
	if (bounds.getSize().getHeight() != 0)
		dy = bounds.getSize().getHeight() / 2;

	start_control_point = start_point;
	start_control_point.moveRel (0, dy);

	end_control_point = end_point;
	end_control_point.moveRel (-dx, 0);	
}

}

