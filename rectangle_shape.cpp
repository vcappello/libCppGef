#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/base_object.hpp>
#include <boost/serialization/extended_type_info.hpp>
#include <boost/serialization/nvp.hpp>

#include "rectangle_shape.h"

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(cppgef::RectangleShape)

template 
void cppgef::RectangleShape::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::RectangleShape::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

namespace cppgef
{

RectangleShape::RectangleShape() :
	ShapeBase()
{
}

RectangleShape::~RectangleShape()
{
}

template< class Archive >
void RectangleShape::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ShapeBase);
}

}

