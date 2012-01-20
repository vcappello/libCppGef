#include "rounded_rectangle_figure.h"

namespace cppgef
{

RoundedRectangleFigure::RoundedRectangleFigure()
{
}

RoundedRectangleFigure::~RoundedRectangleFigure()
{
}

void RoundedRectangleFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	double x = bounds_.getLocation().getX();
	double y = bounds_.getLocation().getY();
	double width = bounds_.getSize().getWidth();
	double height = bounds_.getSize().getHeight();
	
	double aspect = 1.0;     // aspect ratio 
	double corner_radius = 5; //height / 10.0;   // and corner curvature radius 

	double radius = corner_radius / aspect;
	double degrees = M_PI / 180.0;

	context->begin_new_path();
	
	context->arc (x + width - radius, y + radius, radius, -90 * degrees, 0 * degrees);
	context->arc (x + width - radius, y + height - radius, radius, 0 * degrees, 90 * degrees);
	context->arc (x + radius, y + height - radius, radius, 90 * degrees, 180 * degrees);
	context->arc (x + radius, y + radius, radius, 180 * degrees, 270 * degrees);
	
	context->close_path();
	
	fill_style_->apply (context);
	stroke_style_->apply (context);
	
	paintChildren (context);	
}

void RoundedRectangleFigure::paintWithAlpha(double alpha)
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

