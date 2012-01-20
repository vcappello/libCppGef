#include "ellipse_figure.h"

#include <cmath>

namespace cppgef
{

EllipseFigure::EllipseFigure()
{
}

EllipseFigure::~EllipseFigure()
{
}

void EllipseFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	double x = bounds_.getLocation().getX();
	double y = bounds_.getLocation().getY();
	double width = bounds_.getSize().getWidth();
	double height = bounds_.getSize().getHeight();
	
	double rx = width == 0 ? 1 : width / 2.0;
	double ry = height == 0 ? 1 : height / 2.0;

	context->save();
	
	context->begin_new_path();
	context->translate (x + rx, y + ry);
	context->scale (rx, ry);
	context->arc (0.0, 0.0, 1.0, 0.0, 2 * M_PI);
	//context->close_path();
	
	context->restore();

	fill_style_->apply (context);
	stroke_style_->apply (context);
	
	paintChildren (context);
}

void EllipseFigure::paintWithAlpha(double alpha)
{
	double new_stroke_alpha = 0;
	if (alpha < stroke_style_->getColor().getA()) 
		new_stroke_alpha = stroke_style_->getColor().getA() - alpha;
	
	stroke_style_->getColor().setA (new_stroke_alpha);

	double new_fill_alpha = 0;
	if (alpha < fill_style_->getColor().getA()) 
		new_fill_alpha = fill_style_->getColor().getA() - alpha;
	
	fill_style_->getColor().setA (new_fill_alpha);
}

}

