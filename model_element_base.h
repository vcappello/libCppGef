#ifndef MODELELEMENT_H
#define MODELELEMENT_H

#include <boost/serialization/access.hpp>
#include <boost/shared_ptr.hpp>

#include <sigc++/sigc++.h>

using boost::shared_ptr;

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

public:
	ModelElementBase();
	virtual ~ModelElementBase();
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // MODELELEMENT_H
