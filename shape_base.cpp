#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

#include "shape_base.h"

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(cppgef::ShapeBase)

#include <ustring_serialization.h>

template 
void cppgef::ShapeBase::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::ShapeBase::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

template
void cppgef::ShapeBase::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::ShapeBase::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

ShapeBase::ShapeBase() :
	line_color_( Settings::getInstance()->getLineColor() ),
	line_width_( Settings::getInstance()->getLineWidth() ),
	line_dash_style_( Settings::getInstance()->getLineDashStyle() ),
	line_dash_style_offset_( Settings::getInstance()->getLineDashStyleOffset() ),
	fill_color_( Settings::getInstance()->getFillColor() )
{
	bounds_.signalRectangleChanged().connect (
		sigc::mem_fun(this, &ShapeBase::onBoundsRectangleChanged));
}

ShapeBase::~ShapeBase()
{
}

Rectangle& ShapeBase::getBounds()
{
	return bounds_;
}

void ShapeBase::setBounds(const Rectangle& value)
{
	bounds_ = value;
}

cppgef::Color ShapeBase::getLineColor() const
{
	return line_color_;
}

void ShapeBase::setLineColor(const cppgef::Color& value)
{
	if (line_color_ != value)
	{
		line_color_ = value;
		signal_line_color_changed_.emit();
	}
}

double ShapeBase::getLineWidth() const
{
	return line_width_;
}

void ShapeBase::setLineWidth(double value)
{
	if (float_not_equ(line_width_, value))
	{
		line_width_ = value;
		signal_line_width_changed_.emit();
	}
}

std::vector< double > ShapeBase::getLineDashStyle() const
{
	return line_dash_style_;
}

void ShapeBase::setLineDashStyle(const std::vector< double >& value)
{
	if (line_dash_style_ != value)
	{
		line_dash_style_ = value;
		signal_line_dash_style_changed_.emit();
	}
}

double ShapeBase::getLineDashStyleOffset() const
{
	return line_dash_style_offset_;
}

void ShapeBase::setLineDashStyleOffset(double value)
{
	if (float_not_equ(line_dash_style_offset_, value))
	{
		line_dash_style_offset_ = value;
		signal_line_dash_style_offset_changed_.emit();
	}
}
	
cppgef::Color ShapeBase::getFillColor() const
{
	return fill_color_;
}

void ShapeBase::setFillColor(const cppgef::Color& value)
{
	if (fill_color_ != value)
	{
		fill_color_ = value;
		signal_fill_color_changed_.emit();
	}
}

ShapeBase::signal_property_changed_t ShapeBase::signalBoundsChanged()
{
	return signal_bounds_changed_;
}

ShapeBase::signal_property_changed_t ShapeBase::signalLineColorChanged()
{
	return signal_line_color_changed_;
}	

ShapeBase::signal_property_changed_t ShapeBase::signalLineWidthChanged()
{
	return signal_line_width_changed_;
}

ShapeBase::signal_property_changed_t ShapeBase::signalLineDashStyleChanged()
{
	return signal_line_dash_style_changed_;
}

ShapeBase::signal_property_changed_t ShapeBase::signalLineDashStyleOffsetChanged()
{
	return signal_line_dash_style_offset_changed_;
}

ShapeBase::signal_property_changed_t ShapeBase::signalFillColorChanged()
{
	return signal_fill_color_changed_;
}	

void ShapeBase::onBoundsRectangleChanged()
{
	signal_bounds_changed_.emit();
}

template< class Archive >
void ShapeBase::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ModelElementBase);
	
	ar & BOOST_SERIALIZATION_NVP(bounds_);
	ar & BOOST_SERIALIZATION_NVP(line_color_);
	ar & BOOST_SERIALIZATION_NVP(line_width_);
	ar & BOOST_SERIALIZATION_NVP(line_dash_style_);
	ar & BOOST_SERIALIZATION_NVP(line_dash_style_offset_);
	ar & BOOST_SERIALIZATION_NVP(fill_color_);
}

}
