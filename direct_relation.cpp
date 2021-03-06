/*
 * direct_relation.cpp
 *
 *  Created on: 05/ott/2012
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

#include "direct_relation.h"

#include <boost/serialization/export.hpp>

template
void cppgef::DirectRelation::serialize(
    boost::archive::text_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::DirectRelation::serialize(
    boost::archive::text_iarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::DirectRelation::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::DirectRelation::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

DirectRelation::DirectRelation()
{
}

DirectRelation::~DirectRelation()
{
}

const shared_ptr<EndPoint>& DirectRelation::GetEndPoint() const
{
	return end_point_;
}

void DirectRelation::SetEndPoint(const shared_ptr<EndPoint>& end_point)
{
	end_point_ = end_point;
	line_->setEndPoint (*end_point->GetRefPoint());

	signal_end_point_changed_.emit();

	end_point_->signalPositionChanged().connect (
			sigc::mem_fun( this, &DirectRelation::onEndPointPositionChanged ));
}

const shared_ptr<EndPoint>& DirectRelation::GetStartPoint() const
{
	return start_point_;
}

void DirectRelation::SetStartPoint(const shared_ptr<EndPoint>& start_point)
{
	start_point_ = start_point;
	line_->setStartPoint (*start_point->GetRefPoint());

	signal_start_point_changed_.emit();

	end_point_->signalPositionChanged().connect (
			sigc::mem_fun( this, &DirectRelation::onStartPointPositionChanged ));
}

const shared_ptr<LineBase>& DirectRelation::GetLine() const
{
	return line_;
}

void DirectRelation::SetLine(const shared_ptr<LineBase>& line)
{
	line_ = line;

	line_->signalLineColorChanged().connect(
			sigc::mem_fun( this, &DirectRelation::onLineColorChanged ));

	line_->signalLineWidthChanged().connect(
			sigc::mem_fun( this, &DirectRelation::onLineWidthChanged ));

	line_->signalLineDashStyleChanged().connect(
			sigc::mem_fun( this, &DirectRelation::onLineDashStyleChanged ));

	line_->signalLineDashStyleOffsetChanged().connect(
			sigc::mem_fun( this, &DirectRelation::onLineDashStyleOffsetChanged ));
}

DirectRelation::signal_property_changed_t DirectRelation::signalStartPointChanged()
{
	return signal_start_point_changed_;
}

DirectRelation::signal_property_changed_t DirectRelation::signalEndPointChanged()
{
	return signal_end_point_changed_;
}


DirectRelation::signal_property_changed_t DirectRelation::signalStartPointPositionChanged()
{
	return signal_start_point_position_changed_;
}

DirectRelation::signal_property_changed_t DirectRelation::signalEndPointPositionChanged()
{
	return signal_end_point_position_changed_;
}

DirectRelation::signal_property_changed_t DirectRelation::signalLineColorChanged()
{
	return signal_line_color_changed_;
}

DirectRelation::signal_property_changed_t DirectRelation::signalLineWidthChanged()
{
	return signal_line_width_changed_;
}

DirectRelation::signal_property_changed_t DirectRelation::signalLineDashStyleChanged()
{
	return signal_line_dash_style_changed_;
}

DirectRelation::signal_property_changed_t DirectRelation::signalLineDashStyleOffsetChanged()
{
	return signal_line_dash_style_offset_changed_;
}

void DirectRelation::onEndPointPositionChanged()
{
	signal_end_point_position_changed_.emit();
}

void DirectRelation::onStartPointPositionChanged()
{
	signal_start_point_position_changed_.emit();
}

void DirectRelation::onLineColorChanged()
{
	signal_line_color_changed_.emit();
}

void DirectRelation::onLineWidthChanged()
{
	signal_line_width_changed_.emit();
}

void DirectRelation::onLineDashStyleChanged()
{
	signal_line_dash_style_changed_.emit();
}

void DirectRelation::onLineDashStyleOffsetChanged()
{
	signal_line_dash_style_offset_changed_.emit();
}

template< class Archive >
void DirectRelation::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ModelElementBase);

	ar & BOOST_SERIALIZATION_NVP(start_point_);
	ar & BOOST_SERIALIZATION_NVP(end_point_);
	ar & BOOST_SERIALIZATION_NVP(line_);
}

} /* namespace cppgef */
