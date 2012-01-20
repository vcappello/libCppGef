#include "settings.h"

namespace cppgef
{

Settings* Settings::instance_ = 0;
	
Settings::Settings() :
	line_color_( "black" ),
	line_width_( 1.0 ),
	line_dash_style_offset_( 0.0 ),
	fill_color_( "gold" ),
	text_foreground_color_( "black" ),
	text_background_color_( "yellow" ),
	text_has_background_color_( false )
{
}

Settings::~Settings()
{
}

Settings* Settings::getInstance()
{
	if(instance_ == 0)
	{
		instance_ = new Settings();
	}
	return instance_;
}

Color Settings::getLineColor() const
{
	return line_color_;
}

void Settings::setLineColor(const Color& value)
{
	line_color_ = value;
}

double Settings::getLineWidth() const
{
	return line_width_;
}

void Settings::setLineWidth(double value)
{
	line_width_ = value;
}

std::vector< double > Settings::getLineDashStyle() const
{
	return line_dash_style_;
}

void Settings::setLineDashStyle(const std::vector< double >& value)
{
	line_dash_style_ = value;
}

double Settings::getLineDashStyleOffset() const
{
	return line_dash_style_offset_;
}

void Settings::setLineDashStyleOffset(double value)
{
	line_dash_style_offset_ = value;
}

Color Settings::getFillColor() const
{
	return fill_color_;
}

void Settings::setFillColor(const Color& value)
{
	fill_color_ = value;
}

Color Settings::getTextForegroundColor() const
{
	return text_foreground_color_;
}

void Settings::setTextForegroundColor(const Color& value)
{
	text_foreground_color_ = value;
}

Color Settings::getTextBackgroundColor() const
{
	return text_background_color_;
}

void Settings::setTextBackgroundColor(const Color& value)
{
	text_background_color_ = value;
}

bool Settings::getTextHasBackgroundColor() const
{
	return text_has_background_color_;
}

void Settings::setTextHasBackgroundColor(bool value)
{
	text_has_background_color_ = value;
}

}

