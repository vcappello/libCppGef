#include "group_figure.h"

#include <boost/foreach.hpp>

namespace cppgef
{

GroupFigure::GroupFigure()
{
}

GroupFigure::~GroupFigure()
{
}

shared_ptr< RectFigureBase > GroupFigure::getBackgroundFigure()
{
	return background_figure_;
}

void GroupFigure::setBackgroundFigure(shared_ptr< RectFigureBase > value)
{
	background_figure_ = value;
}

void GroupFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	if (background_figure_)
		background_figure_->paint (context);
	
	double x = bounds_.getLocation().getX();
	double y = bounds_.getLocation().getY();
	double width = bounds_.getSize().getWidth();
	double height = bounds_.getSize().getHeight();
	
	context->save();
	
#ifdef GTKMM_3
	context->begin_new_path();
	context->rectangle (x, y, width, height);
	context->clip();
#endif
	
	context->begin_new_path();
	
	paintChildren (context);
	
	context->restore();
}

void GroupFigure::paintWithAlpha(double alpha)
{
	if (background_figure_)
		background_figure_->paintWithAlpha (alpha);
	
	BOOST_FOREACH( shared_ptr< Figure > child, children_)
	{
		child->paintWithAlpha (alpha);
	}	
}

void GroupFigure::onBoundsRectangleChanged()
{
	if (background_figure_)	
		background_figure_->setBounds (bounds_);
	
	RectFigureBase::onBoundsRectangleChanged();
}

}

