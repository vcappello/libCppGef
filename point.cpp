#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <boost/archive/xml_oarchive.hpp>

#include "point.h"

#include <rectangle.h>

#include <boost/serialization/nvp.hpp>

#include <boost/serialization/export.hpp>
BOOST_CLASS_EXPORT(cppgef::Point)

template 
void cppgef::Point::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::Point::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 

template
void cppgef::Point::serialize(
    boost::archive::xml_oarchive &ar,
    const unsigned int /* file_version */
);

template
void cppgef::Point::serialize(
    boost::archive::xml_iarchive &ar,
    const unsigned int /* file_version */
);

namespace cppgef
{

Point::Point() :
	x_( 0 ),
	y_( 0 )
{
}

Point::Point(const int& x, const int& y) :
	x_( x ),
	y_( y )
{
}

Point::Point(const Point& rhs) :
	x_( rhs.x_ ),
	y_( rhs.y_ )
{
}

Point::~Point()
{
}

int Point::getX() const
{
	return x_;
}

void Point::setX(int x)
{
	if (x != x_)
	{
		x_ = x;
		
		signal_x_changed_.emit();
	}
}

int Point::getY() const
{
	return y_;
}

void Point::setY(int y)
{
	if (y != y_)
	{
		y_ = y;
		
		signal_y_changed_.emit();
	}
}

void Point::moveRel(int dx, int dy)
{
	setX (x_ + dx);
	setY (y_ + dy);
}

void Point::moveRel(const Size& size)
{
	moveRel (size.getWidth(), size.getHeight());
}

void Point::set(int x, int y)
{
	setX (x);
	setY (y);	
}

Rectangle Point::surround(int dx, int dy)
{
	return Rectangle (x_ - dx,
			y_ - dy,
			dx * 2,
			dy * 2);	
}

Point::signal_property_changed_t Point::signalXChanged()
{
	return signal_x_changed_;
}

Point::signal_property_changed_t Point::signalYChanged()
{
	return signal_y_changed_;
}

Point& Point::operator= (const Point& rhs)
{
	setX (rhs.x_);
	setY (rhs.y_);
	
	return *this;
}

bool Point::operator== (const Point& rhs) const
{
	return (x_ == rhs.x_) && (y_ == rhs.y_);
}

bool Point::operator!= (const Point& rhs) const
{
	return (x_ != rhs.x_) || (y_ != rhs.y_);
}

template< class Archive >
void Point::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(x_);
	ar & BOOST_SERIALIZATION_NVP(y_);
}

}


