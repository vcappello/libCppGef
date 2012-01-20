#include "stroke_style.h"

#include "float_cmp.h"

namespace cppgef
{

StrokeStyle::StrokeStyle() :
	color_( Settings::getInstance()->getLineColor() ),
	line_width_( 2.0 )
{
}

StrokeStyle::~StrokeStyle()
{
}

Color& StrokeStyle::getColor()
{
	return color_;
}

void StrokeStyle::setColor(const Color& value)
{
	this->color_ = value;
}

double StrokeStyle::getLineWidth() const
{
	return line_width_;
}

void StrokeStyle::setLineWidth(double value)
{
	this->line_width_ = value;
}

std::vector< double > StrokeStyle::getLineDashStyle() const
{
	return line_dash_style_;
}

void StrokeStyle::setLineDashStyle(const std::vector< double >& value)
{
	this->line_dash_style_ = value;
}

double StrokeStyle::getLineDashStyleOffset() const
{
	return line_dash_style_offset_;
}

void StrokeStyle::setLineDashStyleOffset(double value)
{
	this->line_dash_style_offset_ = value;
}

void StrokeStyle::apply(Cairo::RefPtr<Cairo::Context> context)
{
	color_.useInCairo (context);
	
	context->set_line_width (line_width_);
	
	context->set_dash (line_dash_style_, line_dash_style_offset_);
	
	context->stroke_preserve();
}

}

