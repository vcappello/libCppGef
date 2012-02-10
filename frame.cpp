#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "frame.h"

#include <glibmm/i18n.h>

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(cppgef::Frame)

#include <ustring_serialization.h>

template 
void cppgef::Frame::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::Frame::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

template
void cppgef::Frame::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::Frame::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

Frame::Frame() :
	title_( _("Frame") ),
	line_color_( Settings::getInstance()->getLineColor() ),
	line_width_( Settings::getInstance()->getLineWidth() ),
	line_dash_style_( Settings::getInstance()->getLineDashStyle() ),
	line_dash_style_offset_( Settings::getInstance()->getLineDashStyleOffset() ),	
	fill_color_( Settings::getInstance()->getFillColor() )
{
	bounds_.signalRectangleChanged().connect (
		sigc::mem_fun(this, &Frame::onBoundsRectangleChanged));
}

Frame::~Frame()
{
}

Rectangle& Frame::getBounds()
{
	return bounds_;
}

void Frame::setBounds(const Rectangle& value)
{
	bounds_ = value;
}

void Frame::setTitle(const Glib::ustring& value)
{
	if (value != title_)
	{
		title_ = value;
		
		signal_title_changed_.emit();
	}
}

Glib::ustring Frame::getTitle() const
{
	return title_;
}

cppgef::Color Frame::getLineColor() const
{
	return line_color_;
}

void Frame::setLineColor(const cppgef::Color& value)
{
	if (line_color_ != value)
	{
		line_color_ = value;
		signal_line_color_changed_.emit();
	}
}

double Frame::getLineWidth() const
{
	return line_width_;
}

void Frame::setLineWidth(double value)
{
	if (float_not_equ(line_width_, value))
	{
		line_width_ = value;
		signal_line_width_changed_.emit();
	}
}

std::vector< double > Frame::getLineDashStyle() const
{
	return line_dash_style_;
}

void Frame::setLineDashStyle(const std::vector< double >& value)
{
	if (line_dash_style_ != value)

	{
		line_dash_style_ = value;
		signal_line_dash_style_changed_.emit();
	}
}

double Frame::getLineDashStyleOffset() const
{
	return line_dash_style_offset_;
}

void Frame::setLineDashStyleOffset(double value)
{
	if (float_not_equ(line_dash_style_offset_, value))
	{
		line_dash_style_offset_ = value;
		signal_line_dash_style_offset_changed_.emit();
	}
}
	
cppgef::Color Frame::getFillColor() const
{
	return fill_color_;
}

void Frame::setFillColor(const cppgef::Color& value)
{
	if (fill_color_ != value)
	{
		fill_color_ = value;
		signal_fill_color_changed_.emit();
	}
}

Glib::ustring Frame::getText() const
{
	return getTitle();
}

void Frame::setText(const Glib::ustring& value)
{
	setTitle (value);
}	

Frame::signal_property_changed_t Frame::signalBoundsChanged()
{
	return signal_bounds_changed_;
}

Frame::signal_property_changed_t Frame::signalTitleChanged()
{
	return signal_title_changed_;
}

Frame::signal_property_changed_t Frame::signalLineColorChanged()
{
	return signal_line_color_changed_;
}	

Frame::signal_property_changed_t Frame::signalLineWidthChanged()
{
	return signal_line_width_changed_;
}

Frame::signal_property_changed_t Frame::signalLineDashStyleChanged()
{
	return signal_line_dash_style_changed_;
}

Frame::signal_property_changed_t Frame::signalLineDashStyleOffsetChanged()
{
	return signal_line_dash_style_offset_changed_;
}

Frame::signal_property_changed_t Frame::signalFillColorChanged()
{
	return signal_fill_color_changed_;
}	

void Frame::onBoundsRectangleChanged()
{
	signal_bounds_changed_.emit();
}

template< class Archive >
void Frame::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ComplexModelElementBase);
	
	ar & BOOST_SERIALIZATION_NVP(bounds_);
	ar & BOOST_SERIALIZATION_NVP(title_);
	ar & BOOST_SERIALIZATION_NVP(line_color_);
	ar & BOOST_SERIALIZATION_NVP(line_width_);
	ar & BOOST_SERIALIZATION_NVP(line_dash_style_);
	ar & BOOST_SERIALIZATION_NVP(line_dash_style_offset_);
	ar & BOOST_SERIALIZATION_NVP(fill_color_);
}

}

