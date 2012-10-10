/*
 * end_point.h
 *
 *  Created on: 28/set/2012
 *      Author: vincenzo
 */

#ifndef END_POINT_H_
#define END_POINT_H_

#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <sigc++/sigc++.h>

#include <model_element_base.h>

#include <string>

#include <point.h>

using boost::shared_ptr;
using std::string;

namespace cppgef
{

class EndPoint: public cppgef::ModelElementBase
{
public:
	typedef string direction_t;

	static const char* DIRECTION_START;
	static const char* DIRECTION_END;

public:
	EndPoint();
	virtual ~EndPoint();

	const direction_t& GetDirection() const;
	void SetDirection(const direction_t& direction);

	const shared_ptr<Point>& GetRefPoint() const;
	void SetRefPoint(const shared_ptr<Point>& ref_point);

	signal_property_changed_t signalRefPointChanged();
	signal_property_changed_t signalDirectionChanged();
	signal_property_changed_t signalPositionChanged();

protected:
	shared_ptr< Point > ref_point_;
	direction_t direction_;

	// Signal instances
	signal_property_changed_t signal_ref_point_changed_;
	signal_property_changed_t signal_direction_changed_;
	signal_property_changed_t signal_position_changed_;

protected:
	void onPositionChanged();

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

} /* namespace cppgef */
#endif /* END_POINT_H_ */
