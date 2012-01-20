#include "rectangle_figure.h"

namespace cppgef
{

RectangleFigure::RectangleFigure()
{
}

RectangleFigure::~RectangleFigure()
{
}

void RectangleFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	double x = bounds_.getLocation().getX();
	double y = bounds_.getLocation().getY();
	double width = bounds_.getSize().getWidth();
	double height = bounds_.getSize().getHeight();
	
	context->begin_new_path();
	context->rectangle (x, y, width, height);
	
	fill_style_->apply (context);
	stroke_style_->apply (context);
	
	paintChildren (context);
}

void RectangleFigure::paintWithAlpha(double alpha)
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

