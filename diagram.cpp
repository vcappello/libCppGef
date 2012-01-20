#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

#include "diagram.h"

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(cppgef::Diagram)

#include <ustring_serialization.h>

#include <glibmm/i18n.h>

#include <algorithm>

template 
void cppgef::Diagram::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::Diagram::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

namespace cppgef
{

Diagram::Diagram()
{
}

Diagram::~Diagram()
{
}

void Diagram::setTitle(const Glib::ustring& value)
{
	if (value != title_)
	{
		title_ = value;
		
		signal_title_changed_.emit();
	}
}

Glib::ustring Diagram::getTitle() const
{
	return title_;
}

Diagram::signal_property_changed_t Diagram::signalTitleChanged()
{
	return signal_title_changed_;
}

template< class Archive >
void Diagram::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ComplexModelElementBase);
	
	ar & BOOST_SERIALIZATION_NVP(title_);
}

}


