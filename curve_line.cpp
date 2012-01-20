#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>

#include "curve_line.h"

#include <boost/serialization/export.hpp>

// TODO: export here LineBase since BOOST_CLASS_EXPORT
// does not work on linux for this class
BOOST_CLASS_EXPORT(cppgef::LineBase)
BOOST_CLASS_EXPORT(cppgef::CurveLine)

namespace cppgef
{

CurveLine::CurveLine() :
	LineBase()
{
	start_control_point_.signalXChanged().connect (
		sigc::mem_fun( this, &CurveLine::onStartControlPointChanged ));
		
	start_control_point_.signalYChanged().connect (
		sigc::mem_fun( this, &CurveLine::onStartControlPointChanged ));

	end_control_point_.signalXChanged().connect (
		sigc::mem_fun( this, &CurveLine::onEndControlPointChanged ));

	end_control_point_.signalYChanged().connect (
		sigc::mem_fun( this, &CurveLine::onEndControlPointChanged ));
}

CurveLine::~CurveLine()
{
}

Point& CurveLine::getStartControlPoint()
{
	return start_control_point_;
}

void CurveLine::setStartControlPoint(const Point& value)
{
	start_control_point_ = value;
}

Point& CurveLine::getEndControlPoint()
{
	return end_control_point_;
}

void CurveLine::setEndControlPoint(const Point& value)
{
	end_control_point_ = value;
}

CurveLine::signal_property_changed_t CurveLine::signalStartControlPointChanged()
{
	return signal_start_control_point_changed_;
}

CurveLine::signal_property_changed_t CurveLine::signalEndControlPointChanged()
{
	return signal_end_control_point_changed_;
}

void CurveLine::onStartControlPointChanged()
{
	signal_start_control_point_changed_.emit();
}

void CurveLine::onEndControlPointChanged()
{
	signal_end_control_point_changed_.emit();
}
	
template< class Archive >
void CurveLine::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(LineBase);
	
	ar & BOOST_SERIALIZATION_NVP(start_control_point_);
	ar & BOOST_SERIALIZATION_NVP(end_control_point_);
}

}

