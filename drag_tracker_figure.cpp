#include "drag_tracker_figure.h"

namespace cppgef
{

DragTrackerFigure::DragTrackerFigure()
{
	stroke_style_->setLineWidth (1.0);

	std::vector< double > no_dash;
	stroke_style_->setLineDashStyle (no_dash);
	stroke_style_->setLineDashStyleOffset (0);
}

DragTrackerFigure::~DragTrackerFigure()
{
}

void DragTrackerFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	double x = bounds_.getLocation().getX();
	double y = bounds_.getLocation().getY();
	double width = bounds_.getSize().getWidth();
	double height = bounds_.getSize().getHeight();
	
	// Adjust position by 0.5 for draw single pixel
	// sharp lines
	context->user_to_device (x, y);
	x += 0.5;
	y += 0.5;
	context->device_to_user (x, y);
	
	context->begin_new_path();
	context->rectangle (x, y, width, height);
		
	fill_style_->apply (context);
	stroke_style_->apply (context);
	
	paintChildren (context);
}

void DragTrackerFigure::paintWithAlpha(double alpha)
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

