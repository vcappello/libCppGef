#include "line_figure.h"

#include <geometry.h>

namespace cppgef
{

LineFigure::LineFigure()
{
	stroke_style_ = shared_ptr< StrokeStyle >(new StrokeStyle());
	
	start_point_.signalXChanged().connect (
		sigc::mem_fun( this, &LineFigure::onStartPointChanged ));

	start_point_.signalYChanged().connect (
		sigc::mem_fun( this, &LineFigure::onStartPointChanged ));

	end_point_.signalXChanged().connect (
		sigc::mem_fun( this, &LineFigure::onEndPointChanged ));

	end_point_.signalYChanged().connect (
		sigc::mem_fun( this, &LineFigure::onEndPointChanged ));
}

LineFigure::~LineFigure()
{
}

Point& LineFigure::getStartPoint()
{
	return start_point_;
}

void LineFigure::setStartPoint(const Point& value)
{
	start_point_ = value;
}

Point& LineFigure::getEndPoint()
{
	return end_point_;
}

void LineFigure::setEndPoint(const Point& value)
{
	end_point_ = value;
}

shared_ptr< StrokeStyle > LineFigure::getStrokeStyle()
{
	return stroke_style_;
}

shared_ptr< IVisualStyle > LineFigure::getVisualStyle(const Glib::ustring& id)
{
	if (id == "stroke-style")
		return stroke_style_;
		
	return shared_ptr< IVisualStyle >();
}

Rectangle LineFigure::getOutlineRect() const
{
	return Rectangle( start_point_, end_point_ );
}

void LineFigure::fitRect(const Rectangle& rect)
{
	start_point_ = rect.getLocation();
	end_point_ = Point( rect.getX2(), rect.getY2() );
}

bool LineFigure::isPointIn(const Point& pt)
{
	if (pointToLineDistance (getStartPoint(), 
		getEndPoint(),
		pt) < stroke_style_->getLineWidth() / 2.0 + 0.9)
		{
			return true;
		}
		
	return false;	
}

bool LineFigure::isBoundsOut(const Rectangle& rect)
{
	if (rect.contains (getOutlineRect()))
	{
		return true;
	}
	
	return false;		
}
	
void LineFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	context->begin_new_path();
	context->move_to (start_point_.getX(), start_point_.getY());
	context->line_to (end_point_.getX(), end_point_.getY());
	
	stroke_style_->apply (context);
	
	paintChildren (context);
}

void LineFigure::paintWithAlpha(double alpha)
{
	double new_stroke_alpha = 0;
	if (alpha < stroke_style_->getColor().getA()) 
		new_stroke_alpha = stroke_style_->getColor().getA() - alpha;
	
	stroke_style_->getColor().setA (new_stroke_alpha);
}

void  LineFigure::onStartPointChanged()
{
	if (layout_manager_)
		layout_manager_->applyConstraints (getOutlineRect());

}

void  LineFigure::onEndPointChanged()
{
	if (layout_manager_)
		layout_manager_->applyConstraints (getOutlineRect());
}


}

