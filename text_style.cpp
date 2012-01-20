#include "text_style.h"

#include "float_cmp.h"
#include <gtkmm.h>

namespace cppgef
{

TextStyle::TextStyle() :
	alpha_( 1.0 ),
	font_desc_(  Gtk::Settings::get_default()->property_gtk_font_name() )
{
}

TextStyle::~TextStyle()
{
}

double TextStyle::getAlpha() const
{
	return alpha_;
}

void TextStyle::setAlpha(double value)
{
	this->alpha_ = value;
}

Gdk::Color TextStyle::getColor() const
{
	return color_;
}

void TextStyle::setColor(const Gdk::Color& value)
{
	this->color_ = value;
}

Pango::FontDescription TextStyle::getFontDesc() const
{
	return font_desc_;
}

void TextStyle::setFontDesc(const Pango::FontDescription& font_desc)
{
	font_desc_ = font_desc;
}

void TextStyle::apply(Cairo::RefPtr<Cairo::Context> context)
{
	if (float_equ (alpha_, 1.0))
	{
		context->set_source_rgb (color_.get_red_p(), 
			color_.get_green_p(),
			color_.get_blue_p());		
	}
	else
	{
		context->set_source_rgba (color_.get_red_p(), 
			color_.get_green_p(),
			color_.get_blue_p(),
			alpha_);		
	}	
}

}

