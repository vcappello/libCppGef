#include "rectangle.h"

#include <boost/serialization/nvp.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

template 
void cppgef::Rectangle::serialize( 
    boost::archive::text_oarchive &ar, 
    const unsigned int /* file_version */ 
); 

template 
void cppgef::Rectangle::serialize( 
    boost::archive::text_iarchive &ar, 
    const unsigned int /* file_version */ 
); 


namespace cppgef
{

Rectangle::Rectangle()
{
	initialize();
}

Rectangle::Rectangle(const int& x, const int& y, const int& width, const int& height) :
	location_( x, y ),
	size_( width, height )
{
	initialize();
}

Rectangle::Rectangle(const Point& location, const Size& size) :
 	location_( location ),
	size_( size )
{
	initialize();
}

Rectangle::Rectangle(const Point& start, const Point& end) :
	location_( start ),
	size_( end.getX() >= start.getX() ? end.getX() - start.getX() + 1 : end.getX() - start.getX() - 1,
			end.getY() >= start.getY() ? end.getY() - start.getY() + 1 : end.getY() - start.getY() - 1 )
{
	initialize();
}

Rectangle::Rectangle(const Rectangle& rhs) :
 	location_( rhs.location_ ),
	size_( rhs.size_ )
{
	initialize();
}

Rectangle::~Rectangle()
{
}

void Rectangle::initialize()
{
	location_x_changed_connection_ = location_.signalXChanged().connect (
		sigc::mem_fun(this, &Rectangle::onLocationXChanged));
		
	location_y_changed_connection_ = location_.signalYChanged().connect (
		sigc::mem_fun(this, &Rectangle::onLocationYChanged));

	size_width_changed_connection_ = size_.signalWidthChanged().connect (
		sigc::mem_fun(this, &Rectangle::onSizeWidthChanged));
		
	size_height_changed_connection_= size_.signalHeightChanged().connect (
		sigc::mem_fun(this, &Rectangle::onSizeHeightChanged));	
}

Point& Rectangle::getLocation()
{
	return location_;
}

Point Rectangle::getLocation() const
{
	return location_;
}

void Rectangle::setLocation(const Point& location)
{
	// Block location slots for avoid to emit two signal_rectangle_changed_
	// when both X and Y change
	location_x_changed_connection_.block();
	location_y_changed_connection_.block();
	
	location_ = location;
	
	// Unblock signal
	location_x_changed_connection_.unblock();
	location_y_changed_connection_.unblock();
	
	// Emit the signal_rectangle_changed_ once
	signal_rectangle_changed_.emit();
}

Size& Rectangle::getSize()
{
	return size_;
}

Size Rectangle::getSize() const
{
	return size_;
}

void Rectangle::setSize(const Size& size)
{
	// Block size slots for avoid to emit two signal_rectangle_changed_
	// when both Width and Height change
	size_height_changed_connection_.block();
	size_width_changed_connection_.block();
	
	size_ = size;
	
	// Unblock signal
	size_height_changed_connection_.unblock();
	size_width_changed_connection_.unblock();
	
	// Emit the signal_rectangle_changed_ once
	signal_rectangle_changed_.emit();
}

int Rectangle::getLeft() const
{
	if (size_.getWidth() < 0)
		return location_.getX() + size_.getWidth() + 1;
		
	return location_.getX();
}

int Rectangle::getTop() const
{
	if (size_.getHeight() < 0)
		return location_.getY() + size_.getHeight() + 1;
		
	return location_.getY();
}

int Rectangle::getRight() const
{
	if (size_.getWidth() < 0)
		return location_.getX();
		
	return location_.getX() + size_.getWidth() - 1;	
}

int Rectangle::getBottom() const
{
	if (size_.getHeight() < 0)
		return location_.getY();
		
	return location_.getY() + size_.getHeight() - 1;	
}

int Rectangle::getX1() const
{
	return location_.getX();
}

int Rectangle::getY1() const
{
	return location_.getY();
}

int Rectangle::getX2() const
{
	return size_.getWidth() >= 0 ? location_.getX() + size_.getWidth() - 1 : location_.getX() + size_.getWidth() + 1;
}

int Rectangle::getY2() const
{
	return size_.getHeight() >= 0 ? location_.getY() + size_.getHeight() - 1 : location_.getY() + size_.getHeight() + 1;
}

bool Rectangle::contains(const Point& point) const
{
	return (getLeft() <= point.getX()) &&
		(getTop() <= point.getY()) &&
		(getRight() >= point.getX()) &&
		(getBottom() >= point.getY());
}

bool Rectangle::contains(const Rectangle& rectangle) const
{
	return (getLeft() <= rectangle.getLeft()) &&
		(getTop() <= rectangle.getTop()) &&
		(getRight() >= rectangle.getRight()) &&
		(getBottom() >= rectangle.getBottom());
}

Rectangle Rectangle::inflate(int dx, int dy)
{
	Rectangle inflated(
		Point( getLeft() - dx, getTop() - dy ),
		Point( getRight() + dx, getBottom() + dy ));
		
	return inflated;
}

Rectangle Rectangle::deflate(int dx, int dy)
{
	Rectangle deflated(
		Point( getLeft() + dx, getTop() + dy ),
		Point( getRight() - dx, getBottom() - dy ));
		
	return deflated;	
}

Rectangle& Rectangle::operator= (const Rectangle& rhs)
{
	// Block location slots for avoid to emit four signal_rectangle_changed_
	// when X, Y, Width and Height change
	location_x_changed_connection_.block();
	location_y_changed_connection_.block();
	size_height_changed_connection_.block();
	size_width_changed_connection_.block();
	
	location_ = rhs.location_;
	size_ = rhs.size_;

	location_x_changed_connection_.unblock();
	location_y_changed_connection_.unblock();
	size_height_changed_connection_.unblock();
	size_width_changed_connection_.unblock();

	// Emit the signal_rectangle_changed_ once
	signal_rectangle_changed_.emit();
	
	return *this;
}

bool Rectangle::operator== (const Rectangle& rhs) const
{
	return (location_ == rhs.location_) && (size_ == rhs.size_);
}

Rectangle::signal_property_changed_t Rectangle::signalRectangleChanged()
{
	return signal_rectangle_changed_;
}

void Rectangle::onLocationXChanged()
{
	signal_rectangle_changed_.emit();
}

void Rectangle::onLocationYChanged()
{
	signal_rectangle_changed_.emit();
}

void Rectangle::onSizeWidthChanged()
{
	signal_rectangle_changed_.emit();
}

void Rectangle::onSizeHeightChanged()
{
	signal_rectangle_changed_.emit();
}
	
template< class Archive >
void Rectangle::serialize(Archive & ar, const unsigned int version)
{
	ar & BOOST_SERIALIZATION_NVP(location_);
	ar & BOOST_SERIALIZATION_NVP(size_);
}

std::ostream& operator<<(std::ostream& os, const Rectangle& rhs)
{
	os << rhs.location_.getX() << ", ";
	os << rhs.location_.getY() << ", ";
	os << rhs.size_.getWidth() << ", ";
	os << rhs.size_.getHeight();
	
	return os;
}

}

