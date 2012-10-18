#ifndef MODELELEMENT_H
#define MODELELEMENT_H

#include <boost/serialization/access.hpp>
#include <boost/shared_ptr.hpp>

#include <sigc++/sigc++.h>
#include <vector>

#include <point.h>

using boost::shared_ptr;
using std::vector;

namespace cppgef
{

	
/**
 * @class ModelElement
 * @author vincenzo cappello
 * @date 05/12/2010
 * @file model_element.h
 * 
 * @brief Base class for model element
 */
class ModelElementBase
{
public:
	typedef sigc::signal< void > signal_property_changed_t;
	typedef vector< shared_ptr< Point > > anchor_points_container_t;
	typedef anchor_points_container_t::iterator anchor_points_iterator_t;

public:
	ModelElementBase();
	virtual ~ModelElementBase();

	void addAnchorPoint(shared_ptr< Point > anchor_point);
	void removeAnchorPoint(shared_ptr< Point > anchor_point);

	signal_property_changed_t signalAnchorPointsChanged();

protected:
	vector< shared_ptr< Point > > anchor_points_;
	
	signal_property_changed_t signal_anchor_points_changed_;

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // MODELELEMENT_H
