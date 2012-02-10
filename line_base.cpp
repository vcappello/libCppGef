#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>

#include "line_base.h"

#include <boost/serialization/export.hpp>

#include <ustring_serialization.h>

template 
void cppgef::LineBase::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::LineBase::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

template
void cppgef::LineBase::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::LineBase::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

LineBase::LineBase() :
	line_color_( Settings::getInstance()->getLineColor() ),
	line_width_( Settings::getInstance()->getLineWidth() ),
	line_dash_style_( Settings::getInstance()->getLineDashStyle() ),
	line_dash_style_offset_( Settings::getInstance()->getLineDashStyleOffset() )
{
	start_point_.signalXChanged().connect (
		sigc::mem_fun( this, &LineBase::onStartPointChanged ));
		
	start_point_.signalYChanged().connect (
		sigc::mem_fun( this, &LineBase::onStartPointChanged ));

	end_point_.signalXChanged().connect (
		sigc::mem_fun( this, &LineBase::onEndPointChanged ));

	end_point_.signalYChanged().connect (
		sigc::mem_fun( this, &LineBase::onEndPointChanged ));
}

LineBase::~LineBase()
{
}

Point& LineBase::getStartPoint()
{
	return start_point_;
}

void LineBase::setStartPoint(const Point& value)
{
	start_point_ = value;
}

Point& LineBase::getEndPoint()
{
	return end_point_;
}

void LineBase::setEndPoint(const Point& value)
{
	end_point_ = value;
}

cppgef::Color LineBase::getLineColor() const
{
	return line_color_;
}

void LineBase::setLineColor(const cppgef::Color& value)
{
	if (line_color_ != value)
	{
		line_color_ = value;
		signal_line_color_changed_.emit();
	}
}

double LineBase::getLineWidth() const
{
	return line_width_;
}

void LineBase::setLineWidth(double value)
{
	if (float_not_equ(line_width_, value))
	{
		line_width_ = value;
		signal_line_width_changed_.emit();
	}
}

std::vector< double > LineBase::getLineDashStyle() const
{
	return line_dash_style_;
}

void LineBase::setLineDashStyle(const std::vector< double >& value)
{
	if (line_dash_style_ != value)
	{
		line_dash_style_ = value;
		signal_line_dash_style_changed_.emit();
	}
}

double LineBase::getLineDashStyleOffset() const
{
	return line_dash_style_offset_;
}

void LineBase::setLineDashStyleOffset(double value)
{
	if (float_not_equ(line_dash_style_offset_, value))
	{
		line_dash_style_offset_ = value;
		signal_line_dash_style_offset_changed_.emit();
	}
}
	
LineBase::signal_property_changed_t LineBase::signalStartPointChanged()
{
	return signal_start_point_changed_;
}

LineBase::signal_property_changed_t LineBase::signalEndPointChanged()
{
	return signal_end_point_changed_;
}
	
LineBase::signal_property_changed_t LineBase::signalLineColorChanged()
{
	return signal_line_color_changed_;
}	

LineBase::signal_property_changed_t LineBase::signalLineWidthChanged()
{
	return signal_line_width_changed_;
}

LineBase::signal_property_changed_t LineBase::signalLineDashStyleChanged()
{
	return signal_line_dash_style_changed_;
}

LineBase::signal_property_changed_t LineBase::signalLineDashStyleOffsetChanged()
{
	return signal_line_dash_style_offset_changed_;
}

void LineBase::onStartPointChanged()
{
	signal_start_point_changed_.emit();
}

void LineBase::onEndPointChanged()
{
	signal_end_point_changed_.emit();
}

template< class Archive >
void LineBase::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ModelElementBase);

	ar & BOOST_SERIALIZATION_NVP(start_point_);
	ar & BOOST_SERIALIZATION_NVP(end_point_);

	ar & BOOST_SERIALIZATION_NVP(line_color_);
	ar & BOOST_SERIALIZATION_NVP(line_width_);
	ar & BOOST_SERIALIZATION_NVP(line_dash_style_);
	ar & BOOST_SERIALIZATION_NVP(line_dash_style_offset_);
}

}



