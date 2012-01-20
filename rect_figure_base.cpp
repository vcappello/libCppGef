/*
 * RectFigureBase.cpp
 *
 *  Created on: 03/mar/2011
 *      Author: vincenzo
 */

#include "rect_figure_base.h"

namespace cppgef
{

RectFigureBase::RectFigureBase()
{
	stroke_style_ = shared_ptr< StrokeStyle >(new StrokeStyle());
	fill_style_ = shared_ptr< FillStyle >(new FillStyle());
	
	bounds_.signalRectangleChanged().connect (
		sigc::mem_fun(this, &RectFigureBase::onBoundsRectangleChanged));
}

RectFigureBase::~RectFigureBase()
{
}

Rectangle& RectFigureBase::getBounds()
{
	return bounds_;
}

void RectFigureBase::setBounds(const Rectangle& value)
{
	bounds_ = value;
}

Rectangle RectFigureBase::getOutlineRect() const
{
	return bounds_;
}

shared_ptr< StrokeStyle > RectFigureBase::getStrokeStyle()
{
	return stroke_style_;
}

shared_ptr< FillStyle > RectFigureBase::getFillStyle()
{
	return fill_style_;
}

shared_ptr< IVisualStyle > RectFigureBase::getVisualStyle(const Glib::ustring& id)
{
	if (id == "stroke-style")
		return stroke_style_;
	else if (id == "fill-style")
		return fill_style_;
		
	return shared_ptr< IVisualStyle >();
}

void RectFigureBase::fitRect(const Rectangle& rect)
{
	bounds_ = rect;
}

bool RectFigureBase::isPointIn(const Point& pt)
{
	int i = stroke_style_->getLineWidth() / 2.0 + 0.9;

	if (getOutlineRect().inflate(i, i).contains (pt))
	{
		return true;
	}	
	
	return false;
}

bool RectFigureBase::isBoundsOut(const Rectangle& rect)
{
	int i = stroke_style_->getLineWidth() / 2.0 + 0.9;
	
	return rect.contains (getOutlineRect().inflate(i, i));
}
	
void RectFigureBase::onBoundsRectangleChanged()
{
	if (layout_manager_)
		layout_manager_->applyConstraints (bounds_);
}

}
