#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>

#include "text_shape.h"

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(cppgef::TextShape)

#include <ustring_serialization.h>
#include <glibmm/i18n.h>

template 
void cppgef::TextShape::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::TextShape::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

namespace cppgef
{

TextShape::TextShape() :
	ShapeBase(),
	text_( _("Type some text here") )
{
}

TextShape::~TextShape()
{
}

Glib::ustring TextShape::getText() const
{
	return text_;
}

void TextShape::setText(const Glib::ustring& value)
{
	if (value != text_)
	{
		text_ = value;
		
		signal_text_changed_.emit();
	}
}

TextShape::signal_property_changed_t TextShape::signalTextChanged()
{
	return signal_text_changed_;
}

template< class Archive >
void TextShape::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ShapeBase);
	ar & BOOST_SERIALIZATION_NVP(text_);
}

}


