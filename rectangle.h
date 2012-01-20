#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>
#include <sigc++/sigc++.h>

#include <point.h>
#include <size.h>

namespace cppgef
{

/**
 * @class Rectangle
 * @author vincenzo cappello
 * @date 06/12/2010
 * @file rectangle.h
 * 
 * @brief A rectangle in the 2D space
 */
class Rectangle
{
public:
	typedef sigc::signal< void > signal_property_changed_t;

public:
	Rectangle();
	Rectangle(const int& x, const int& y, const int& width, const int& height);
	Rectangle(const Point& location, const Size& size);
	Rectangle(const Point& start, const Point& end);
	Rectangle(const Rectangle& rhs);
	
	virtual ~Rectangle();

	/**
	 * @brief Return rectangle location, the upper-right corner
	 * 
	 * Use this member also for modify X or Y, in this case
	 * the signalRectangleChanged signal was emitted
	 * 
	 * 
	 * @return a reference to the rectangle location
	 */
	Point& getLocation();
	Point getLocation() const;
	
	/**
	 * @brief Set the new rectangle location
	 * 
	 * Emit the signalRectangleChanged signal
	 * 
	 * @param location the new location
	 */
	void setLocation(const Point& location);
	
	/**
	 * @brief Return rectangle size
	 * 
	 * Use this member also for modify Width or Height, in this case
	 * the signalRectangleChanged signal was emitted
	 * 
	 * @return a reference to the rectangle size
	 */
	Size& getSize();
	Size getSize() const;

	/**
	 * @brief Set the new rectangle size
	 * 
	 * Emit the signalRectangleChanged signal
	 * 
	 * @param location the new size
	 */
	void setSize(const Size& size);

	int getLeft() const;
	int getTop() const;
	int getRight() const;
	int getBottom() const;
	
	int getX1() const;
	int getY1() const;
	int getX2() const;
	int getY2() const;

	bool contains(const Point& point) const;
	bool contains(const Rectangle& rectangle) const;
	
	Rectangle inflate(int dx, int dy);
	Rectangle deflate(int dx, int dy);
	
	Rectangle& operator= (const Rectangle& rhs);
	bool operator== (const Rectangle& rhs) const;

	friend std::ostream& operator<<(std::ostream& os, const Rectangle& rhs);

	/** @name Signals */
	/*@{*/
	
	/**
	 * @brief Signal emitted when location or size change
	 * @return the signal
	 */
	signal_property_changed_t signalRectangleChanged();

	/*@}*/

protected:
	Point location_;
	Size size_;
	
	signal_property_changed_t signal_rectangle_changed_;
	
	sigc::connection location_x_changed_connection_;
	sigc::connection location_y_changed_connection_;
	sigc::connection size_width_changed_connection_;
	sigc::connection size_height_changed_connection_;
	
protected:
	void initialize();
	
	void onLocationXChanged();
	void onLocationYChanged();
	void onSizeWidthChanged();
	void onSizeHeightChanged();
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

std::ostream& operator<<(std::ostream& os, const Rectangle& rhs);

}

#endif // RECTANGLE_H
