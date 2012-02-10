#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "complex_model_element_base.h"

#include <glibmm/i18n.h>

#include <boost/serialization/export.hpp>

BOOST_CLASS_EXPORT(cppgef::ComplexModelElementBase)

template 
void cppgef::ComplexModelElementBase::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::ComplexModelElementBase::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

template
void cppgef::ComplexModelElementBase::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::ComplexModelElementBase::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

ComplexModelElementBase::ComplexModelElementBase() 
{
}

ComplexModelElementBase::~ComplexModelElementBase() 
{
}

void ComplexModelElementBase::addChild(shared_ptr< ModelElementBase > element)
{
	children_.push_back (element);
	
	signal_child_added_.emit (element);
}

void ComplexModelElementBase::insertChild(shared_ptr< ModelElementBase > element, size_t pos)
{
	if (pos >= children_.size())
		children_.push_back (element);
	else
		children_.insert (children_.begin() + pos, element);

	signal_child_added_.emit (element);
}

void ComplexModelElementBase::eraseChild(shared_ptr< ModelElementBase > element)
{
	// TODO: use an unordered_map for fast seek?
	iterator child_itor = std::find (children_.begin(), children_.end(), element);
	
	if (child_itor != children_.end())
	{
		children_.erase (child_itor);
		signal_child_erased_.emit (element);		
	}
	else
	{
		throw Error(
			Glib::ustring(_("Can not erase element that is not added to the ComplexModelElementBase.")));
	}	
}

int ComplexModelElementBase::getChildIndex(shared_ptr< ModelElementBase > element)
{
	for (size_t i = 0; i < children_.size(); i++)
	{
		if (children_[i] == element)
			return i;
	}

	return -1;
}

ComplexModelElementBase::iterator ComplexModelElementBase::getChildrenBegin()
{
	return children_.begin();
}

ComplexModelElementBase::const_iterator ComplexModelElementBase::getChildrenBegin() const
{
	return children_.begin();
}

ComplexModelElementBase::iterator ComplexModelElementBase::getChildrenEnd()
{
	return children_.end();
}

ComplexModelElementBase::const_iterator ComplexModelElementBase::getChildrenEnd() const
{
	return children_.end();
}

ComplexModelElementBase::size_t ComplexModelElementBase::getChildrenSize() const
{
	return children_.size();
}

ComplexModelElementBase::signal_children_property_changed_t ComplexModelElementBase::signalChildAdded()
{
	return signal_child_added_;
}

ComplexModelElementBase::signal_children_property_changed_t ComplexModelElementBase::signalChildErased()
{
	return signal_child_erased_;
}

template< class Archive >
void ComplexModelElementBase::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ModelElementBase);
	
	ar & BOOST_SERIALIZATION_NVP(children_);
}

}

