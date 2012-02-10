#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "size.h"

#include <boost/serialization/nvp.hpp>

template 
void cppgef::Size::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::Size::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

template
void cppgef::Size::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::Size::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

Size::Size() :
	width_( 0 ),
	height_( 0 )
{
}

Size::Size(int width, int height) :
	width_( width ),
	height_( height )
{
	
}

Size::Size(const Size& rhs) :
	width_( rhs.width_ ),
	height_( rhs.height_ )
{
	
}

Size::~Size()
{
}

int Size::getWidth() const
{
	return width_;
}

void Size::setWidth(int width)
{
	if (width != width_)
	{
		width_ = width;
	
		signal_width_changed_.emit();
	}
}

int Size::getHeight() const
{
	return height_;
}

void Size::setHeight(int height)
{
	if (height != height_)
	{
		height_ = height;
	
		signal_height_changed_.emit();
	}
}

bool Size::isEmpty() const
{
	return width_ == 0 && height_ == 0;
}

void Size::set(int w, int h)
{
	setWidth (w);
	setHeight (h);	
}

void Size::resizeRel(int dx, int dy)
{
	setWidth (width_ + dx);
	setHeight (height_ + dy);
}

Size::signal_property_changed_t Size::signalWidthChanged()
{
	return signal_width_changed_;
}

Size::signal_property_changed_t Size::signalHeightChanged()
{
	return signal_height_changed_;
}

Size& Size::operator= (const Size& rhs)
{
	setWidth (rhs.width_);
	setHeight (rhs.height_);
	
	return *this;
}

bool Size::operator== (const Size& rhs) const
{
	return (width_ == rhs.width_) && (height_ == rhs.height_);
}

template< class Archive >
void Size::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(width_);
	ar & BOOST_SERIALIZATION_NVP(height_);
}

}


