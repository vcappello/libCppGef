#include "curve_figure.h"

#include <geometry.h>

namespace cppgef
{

CurveFigure::CurveFigure() :
	points_validated_( false )
{
	stroke_style_ = shared_ptr< StrokeStyle >(new StrokeStyle());
	
	start_point_.signalXChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));
		
	start_point_.signalYChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));
		
	end_point_.signalXChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));
		
	end_point_.signalYChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));

	start_control_point_.signalXChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));
		
	start_control_point_.signalYChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));

	end_control_point_.signalXChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));
		
	end_control_point_.signalYChanged().connect (
		sigc::mem_fun(this, &CurveFigure::onPointChanged));
}

CurveFigure::~CurveFigure()
{
}

Point& CurveFigure::getStartPoint()
{
	return start_point_;
}

void CurveFigure::setStartPoint(const Point& value)
{
	start_point_ = value;
}

Point& CurveFigure::getEndPoint()
{
	return end_point_;
}

void CurveFigure::setEndPoint(const Point& value)
{
	end_point_ = value;
}

Point& CurveFigure::getStartControlPoint()
{
	return start_control_point_;
}

void CurveFigure::setStartControlPoint(const Point& value)
{
	start_control_point_ = value;
}

Point& CurveFigure::getEndControlPoint()
{
	return end_control_point_;
}

void CurveFigure::setEndControlPoint(const Point& value)
{
	end_control_point_ = value;
}

shared_ptr< StrokeStyle > CurveFigure::getStrokeStyle()
{
	return stroke_style_;
}

shared_ptr< IVisualStyle > CurveFigure::getVisualStyle(const Glib::ustring& id)
{
	if (id == "stroke-style")
		return stroke_style_;
		
	return shared_ptr< IVisualStyle >();
}

Rectangle CurveFigure::getOutlineRect() const
{
	if (!points_validated_)
	{
		createBezierPoints (points_, start_point_, start_control_point_, end_control_point_, end_point_);
		bounds_points_ = boundsPoints (points_);
		points_validated_ = true;
	}
	
	return bounds_points_;
}

void CurveFigure::fitRect(const Rectangle& rect)
{
	if (!points_validated_)
	{
		createBezierPoints (points_, start_point_, start_control_point_, end_control_point_, end_point_);
		bounds_points_ = boundsPoints (points_);
		points_validated_ = true;
	}
	
	double fx = (double)rect.getSize().getWidth() / bounds_points_.getSize().getWidth();
	double fy = (double)rect.getSize().getWidth() / bounds_points_.getSize().getWidth();
	
	Point prev_start_point_ = start_point_;
	Point prev_end_point_ = end_point_;
	
	start_point_ = relPointProportion(bounds_points_.getLocation(), start_point_, rect.getLocation(), fx, fy);
	end_point_ = relPointProportion(bounds_points_.getLocation(), end_point_, rect.getLocation(), fx, fy);
	start_control_point_ = relPointProportion(prev_start_point_, start_control_point_, start_point_, fx, fy);
	end_control_point_ = relPointProportion (prev_end_point_, end_control_point_, end_point_, fx, fy);
}

bool CurveFigure::isPointIn(const Point& pt)
{
	if (!points_validated_)
	{
		createBezierPoints (points_, start_point_, start_control_point_, end_control_point_, end_point_);
		bounds_points_ = boundsPoints (points_);
		points_validated_ = true;
	}
	
	if (bounds_points_.contains (pt))
	{
		float tollerance = stroke_style_->getLineWidth() / 2.0 + 0.9;
		
		for (size_t i = 1; i < points_.size(); i++)
		{
			if (pointToLineDistance (points_[i-1], points_[i], pt) <= tollerance)
			{
				return true;
			}
		}
	}
	
	return false;
}

bool CurveFigure::isBoundsOut(const Rectangle& rect)
{
	if (!points_validated_)
	{
		createBezierPoints (points_, start_point_, start_control_point_, end_control_point_, end_point_);
		bounds_points_ = boundsPoints (points_);
		points_validated_ = true;
	}
	
	return rect.contains (bounds_points_);
}

void CurveFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	context->begin_new_path();
	context->move_to (start_point_.getX(), start_point_.getY());
	context->curve_to (start_control_point_.getX(), start_control_point_.getY(),
		end_control_point_.getX(), end_control_point_.getY(),
		end_point_.getX(), end_point_.getY());
	
	stroke_style_->apply (context);
	
	paintChildren (context);	
}

void CurveFigure::paintWithAlpha(double alpha)
{
	double new_alpha = 0;
	if (alpha < stroke_style_->getColor().getA()) 
		new_alpha = stroke_style_->getColor().getA() - alpha;
	
	stroke_style_->getColor().setA (new_alpha);
}

void CurveFigure::onPointChanged()
{
	points_validated_ = false;
}

Point CurveFigure::relPointProportion(const Point& rel_point, const Point& source_point, const Point& new_rel_point, double fx, double fy)
{
	int sdx = source_point.getX() - rel_point.getX();
	int sdy = source_point.getY() - rel_point.getY();

	return Point ( new_rel_point.getX() + sdx * fx, new_rel_point.getY() + sdy * fy);
}

}



