#ifndef ICOMPLEXMODELELEMENT_H
#define ICOMPLEXMODELELEMENT_H

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/serialization/access.hpp>
#include <sigc++/sigc++.h>

#include <model_element_base.h>
#include <error.h>

using boost::shared_ptr;
using std::vector;

namespace cppgef
{

class ComplexModelElementBase : public ModelElementBase
{
public:
	typedef shared_ptr< ModelElementBase > item_t;
	typedef vector< item_t > children_t;
	typedef children_t::iterator iterator;
	typedef children_t::const_iterator const_iterator;
	typedef children_t::size_type size_t;
	
	typedef sigc::signal< void, shared_ptr< ModelElementBase > > signal_children_property_changed_t;
	
public:
	ComplexModelElementBase();
	virtual ~ComplexModelElementBase();

	virtual void addChild(shared_ptr< ModelElementBase > element);

	virtual void insertChild(shared_ptr< ModelElementBase > element, size_t pos);

	virtual void eraseChild(shared_ptr< ModelElementBase > element);

	virtual int getChildIndex(shared_ptr< ModelElementBase > element);

	virtual iterator getChildrenBegin();
	virtual const_iterator getChildrenBegin() const;
	
	virtual iterator getChildrenEnd();
	virtual const_iterator getChildrenEnd() const;

	virtual size_t getChildrenSize() const;

	signal_children_property_changed_t signalChildAdded();
	signal_children_property_changed_t signalChildErased();
	
protected:
	children_t children_;
	
protected:
	signal_children_property_changed_t signal_child_added_;
	signal_children_property_changed_t signal_child_erased_;

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // ICOMPLEXMODELELEMENT_H
