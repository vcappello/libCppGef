/*
 * direct_relation.h
 *
 *  Created on: 05/ott/2012
 *      Author: vincenzo
 */

#ifndef DIRECT_RELATION_H_
#define DIRECT_RELATION_H_

#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <sigc++/sigc++.h>

#include <end_point.h>

using boost::shared_ptr;

namespace cppgef
{

class DirectRelation: public cppgef::ModelElementBase
{
public:
	DirectRelation();
	virtual ~DirectRelation();

	const shared_ptr<EndPoint>& GetEndPoint() const;
	void SetEndPoint(const shared_ptr<EndPoint>& end_point);

	const shared_ptr<EndPoint>& GetStartPoint() const;
	void SetStartPoint(const shared_ptr<EndPoint>& start_point);

	signal_property_changed_t signalStartPointChanged();
	signal_property_changed_t signalEndPointChanged();
	signal_property_changed_t signalStartPointPositionChanged();
	signal_property_changed_t signalEndPointPositionChanged();

protected:
	shared_ptr< EndPoint > start_point_;
	shared_ptr< EndPoint > end_point_;

	// Signal instances
	signal_property_changed_t signal_start_point_changed_;
	signal_property_changed_t signal_end_point_changed_;
	signal_property_changed_t signal_start_point_position_changed_;
	signal_property_changed_t signal_end_point_position_changed_;

protected:

	void onStartPointPositionChanged();
	void onEndPointPositionChanged();

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

} /* namespace cppgef */
#endif /* DIRECT_RELATION_H_ */
