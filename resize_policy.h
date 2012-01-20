#ifndef RESIZEPOLICY_H
#define RESIZEPOLICY_H

#include <boost/shared_ptr.hpp>

#include <rectangle.h>

using boost::shared_ptr;

namespace cppgef
{

class AbstractResizePolicy
{
public:
	enum ResizeDirection
	{
		RD_TOP_LEFT,
		RD_TOP,
		RD_TOP_RIGHT,
		RD_RIGHT,
		RD_BOTTOM_RIGHT,
		RD_BOTTOM,
		RD_BOTTOM_LEFT,
		RD_LEFT
	};

public:
	AbstractResizePolicy();
	virtual ~AbstractResizePolicy();
	
	virtual void resize(Rectangle& rectangle, int dx, int dy) = 0;
	
	static shared_ptr< AbstractResizePolicy > create(ResizeDirection resize_direction, const Rectangle& rectangle);
	
	static shared_ptr< AbstractResizePolicy > createTopLeft(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createTop(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createTopRight(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createRight(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createBottomRight(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createBottom(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createBottomLeft(const Rectangle& rectangle);
	static shared_ptr< AbstractResizePolicy > createLeft(const Rectangle& rectangle);
};

class ResizeX1Y1Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeY1Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeX2Y1Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeX2Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeX2Y2Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeY2Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeX1Y2Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

class ResizeX1Policy : public AbstractResizePolicy
{
	void resize(Rectangle& rectangle, int dx, int dy);
};

}

#endif // RESIZEPOLICY_H
