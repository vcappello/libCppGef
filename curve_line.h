#ifndef CURVELINE_H
#define CURVELINE_H

#include <line_base.h> // Base class: cppgef::LineBase

namespace cppgef
{

class CurveLine : public cppgef::LineBase
{
public:
	CurveLine();
	virtual ~CurveLine();
	
	Point& getStartControlPoint();
	void setStartControlPoint(const Point& value);
	
	Point& getEndControlPoint();
	void setEndControlPoint(const Point& value);
	
	signal_property_changed_t signalStartControlPointChanged();
	signal_property_changed_t signalEndControlPointChanged();
	
protected:
	Point start_control_point_;
	Point end_control_point_;	

	signal_property_changed_t signal_start_control_point_changed_;
	signal_property_changed_t signal_end_control_point_changed_;

protected:
	void onStartControlPointChanged();
	void onEndControlPointChanged();
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);	
};

}

#endif // CURVELINE_H
