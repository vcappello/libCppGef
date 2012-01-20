#ifndef SIZE_H
#define SIZE_H

#include <boost/serialization/access.hpp>
#include <sigc++/sigc++.h>

namespace cppgef
{

/**
 * @class Size
 * @author Vincenzo Cappello
 * @date 06/11/2010
 * @file size.h
 * 
 * @brief The size of a rectangle in width and height
 */
class Size
{
public:
	typedef sigc::signal< void > signal_property_changed_t;
	
public:
	/**
	 * @brief Construct an empty size
	 */
	Size();
	
	/**
	 * @brief Construct a size using width and height
	 * @param width size width
	 * @param height size height
	 */
	Size(int width, int height);
	
	Size(const Size& rhs);
	
	virtual ~Size();

	/**
	 * @brief Size width
	 */
	int getWidth() const;
	
	/**
	 * @brief Size width
	 * @param width new value
	 */
	void setWidth(int width);

	/**
	 * @brief Size height
	 */
	int getHeight() const;
	
	/**
	 * @brief Size height
	 * @param height new value
	 */
	void setHeight(int height);

	/**
	 * @brief Determines whether a Size is empty, both width and height are zero
	 * @return true if the size is empty
	 */
	bool isEmpty() const;
	
	void set(int w, int h);
	
	void resizeRel(int dx, int dy);
	
	signal_property_changed_t signalWidthChanged();
	signal_property_changed_t signalHeightChanged();
	
	Size& operator= (const Size& rhs);
	bool operator== (const Size& rhs) const;
	
protected:
	int width_;
	int height_;

	signal_property_changed_t signal_width_changed_;
	signal_property_changed_t signal_height_changed_;
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // SIZE_H
