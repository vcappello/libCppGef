/*
 * end_point.cpp
 *
 *  Created on: 28/set/2012
 *      Author: vincenzo
 */

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "end_point.h"

#include <boost/serialization/export.hpp>
#include <ustring_serialization.h>

template
void cppgef::EndPoint::serialize(
    boost::archive::text_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::EndPoint::serialize(
    boost::archive::text_iarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::EndPoint::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::EndPoint::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

const char* cppgef::EndPoint::DIRECTION_START = "START";
const char* cppgef::EndPoint::DIRECTION_END = "END";

namespace cppgef
{

EndPoint::EndPoint()
{
}

EndPoint::~EndPoint()
{
}

EndPoint::direction_t EndPoint::GetDirection() const
{
	return direction_;
}

void EndPoint::SetDirection(const direction_t& direction)
{
	direction_ = direction;
	signal_direction_changed_.emit();
}

shared_ptr<Point> EndPoint::GetRefPoint()
{
	return ref_point_;
}

void EndPoint::SetRefPoint(const shared_ptr<Point>& ref_point)
{
	ref_point_ = ref_point;

	ref_point_->signalXChanged().connect (
			sigc::mem_fun( this, &EndPoint::onPositionChanged ));

	ref_point_->signalYChanged().connect (
			sigc::mem_fun( this, &EndPoint::onPositionChanged ));
}

EndPoint::signal_property_changed_t EndPoint::signalRefPointChanged()
{
	return signal_ref_point_changed_;
}

EndPoint::signal_property_changed_t EndPoint::signalDirectionChanged()
{
	return signal_direction_changed_;
}

EndPoint::signal_property_changed_t EndPoint::signalPositionChanged()
{
	return signal_position_changed_;
}

void EndPoint::onPositionChanged()
{
	signal_position_changed_.emit();
}

template< class Archive >
void EndPoint::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(ref_point_);
	ar & BOOST_SERIALIZATION_NVP(direction_);
}

} /* namespace cppgef */
