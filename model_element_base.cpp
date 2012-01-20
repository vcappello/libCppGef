#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include <boost/serialization/nvp.hpp>

#include "model_element_base.h"

template 
void cppgef::ModelElementBase::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::ModelElementBase::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

namespace cppgef
{

ModelElementBase::ModelElementBase()
{
}

ModelElementBase::~ModelElementBase()
{
}	

template< class Archive >
void ModelElementBase::serialize(Archive & ar, const unsigned int version)
{
}

}


