#include "fill_style.h"

#include "float_cmp.h"

namespace cppgef
{

FillStyle::FillStyle() :
	color_( Settings::getInstance()->getFillColor() )
{
}

FillStyle::~FillStyle()
{
}

Color& FillStyle::getColor()
{
	return color_;
}

void FillStyle::setColor(const Color& value)
{
	this->color_ = value;
}

void FillStyle::apply(Cairo::RefPtr<Cairo::Context> context)
{
	color_.useInCairo (context);
	
	context->fill_preserve();	
}

}

