#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include <boost/serialization/nvp.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/shared_ptr.hpp>

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

template
void cppgef::ModelElementBase::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::ModelElementBase::serialize(
    boost::archive::xml_iarchive &ar,
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

void ModelElementBase::addAnchorPoint(shared_ptr<Point> anchor_point)
{
	anchor_points_.push_back (anchor_point);
	signal_anchor_points_changed_.emit();
}

void ModelElementBase::removeAnchorPoint(shared_ptr<Point> anchor_point)
{
	anchor_points_iterator_t anchor_point_itor = std::find (anchor_points_.begin(), anchor_points_.end(),
			anchor_point);

	if (anchor_point_itor != anchor_points_.end())
	{
		anchor_points_.erase (anchor_point_itor);
		signal_anchor_points_changed_.emit();
	}
}

ModelElementBase::signal_property_changed_t ModelElementBase::signalAnchorPointsChanged()
{
	return signal_anchor_points_changed_;
}

template< class Archive >
void ModelElementBase::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(anchor_points_);
}

}

