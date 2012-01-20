#ifndef POINT_H
#define POINT_H

#include <boost/serialization/access.hpp>
#include <sigc++/sigc++.h>

#include <size.h>

namespace cppgef
{

class Rectangle; // Forward declaration
	
/**
 * @class Point
 * @author Vincenzo Cappello
 * @date 06/11/2010
 * @file point.h
 * @brief A point in the 2D space
 */
class Point
{
public:
	typedef sigc::signal< void > signal_property_changed_t;
	
public:
	/**
	 * @brief Construct a point at 0, 0
	 */
	Point();

	/**
	 * @brief Construct a point at x, y
	 * @param x horizontal coordinate
	 * @param y vertical coordinate
	 */
	Point(const int& x, const int& y);
	
	Point(const Point& rhs);
	
	virtual ~Point();

	/**
	 * @brief Horizontal coordinate
	 */
	int getX() const;

	/**
	 * @brief Horizontal coordinate
	 * @param x new value
	 */
	void setX(int x);

	/**
	 * @brief Vertical coordinate
	 */
	int getY() const;


	/**
	 * @brief Vertical coordinate
	 * @param y new value
	 */
	void setY(int y);

	/**
	 * @brief Move the point by delta
	 * @param dx add to x
	 * @param dy add to y
	 */
	void moveRel(int dx, int dy);
	void moveRel(const Size& size);
	
	void set(int x, int y);
	
	Rectangle surround(int dx, int dy);
	
	signal_property_changed_t signalXChanged();
	signal_property_changed_t signalYChanged();
	
	Point& operator= (const Point& rhs);
	bool operator== (const Point& rhs) const;
	bool operator!= (const Point& rhs) const;
	
protected:
	int x_;
	int y_;

	signal_property_changed_t signal_x_changed_;
	signal_property_changed_t signal_y_changed_;
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // POINT_H
