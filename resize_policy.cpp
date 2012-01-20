#include "resize_policy.h"

namespace cppgef
{

AbstractResizePolicy::AbstractResizePolicy()
{
}

AbstractResizePolicy::~AbstractResizePolicy()
{
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::create(ResizeDirection resize_direction, const Rectangle& rectangle)
{
	switch (resize_direction)
	{
		case RD_TOP_LEFT:
			return createTopLeft(rectangle);
			break;
		case RD_TOP:
			return createTop(rectangle);
			break;
		case RD_TOP_RIGHT:
			return createTopRight(rectangle);
			break;
		case RD_RIGHT:
			return createRight(rectangle);
			break;
		case RD_BOTTOM_RIGHT:
			return createBottomRight(rectangle);
			break;
		case RD_BOTTOM:
			return createBottom(rectangle);
			break;
		case RD_BOTTOM_LEFT:
			return createBottomLeft(rectangle);
			break;
		case RD_LEFT:
			return createLeft(rectangle);
			break;
	}
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createTopLeft(const Rectangle& rectangle)
{
	if (rectangle.getSize().getWidth() < 0)
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y2Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y1Policy() );
		}
	}
	else
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y2Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y1Policy() );
		}		
	}
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createTop(const Rectangle& rectangle)
{
	if (rectangle.getSize().getHeight() < 0)
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeY2Policy() );
	}
	else
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeY1Policy() );
	}	
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createTopRight(const Rectangle& rectangle)
{
	if (rectangle.getSize().getWidth() < 0)
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y2Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y1Policy() );
		}
	}
	else
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y2Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y1Policy() );
		}		
	}	
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createRight(const Rectangle& rectangle)
{
	if (rectangle.getSize().getWidth() < 0)
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeX1Policy() );
	}
	else
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeX2Policy() );
	}
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createBottomRight(const Rectangle& rectangle)
{
	if (rectangle.getSize().getWidth() < 0)
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y1Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y2Policy() );
		}
	}
	else
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y1Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y2Policy() );
		}		
	}		
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createBottom(const Rectangle& rectangle)
{
	if (rectangle.getSize().getHeight() < 0)
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeY1Policy() );
	}
	else
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeY2Policy() );
	}		
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createBottomLeft(const Rectangle& rectangle)
{
	if (rectangle.getSize().getWidth() < 0)
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y1Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX2Y2Policy() );
		}
	}
	else
	{
		if (rectangle.getSize().getHeight() < 0)
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y1Policy() );
		}
		else
		{
			return shared_ptr< AbstractResizePolicy >( new ResizeX1Y2Policy() );
		}		
	}		
}

shared_ptr< AbstractResizePolicy > AbstractResizePolicy::createLeft(const Rectangle& rectangle)
{
	if (rectangle.getSize().getWidth() < 0)
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeX2Policy() );
	}
	else
	{
		return shared_ptr< AbstractResizePolicy >( new ResizeX1Policy() );
	}	
}

////////////////////////////////////////////////////////////////////////////////

void ResizeX1Y1Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getLocation().moveRel (dx, dy);
	rectangle.getSize().resizeRel (-dx, -dy);
}

void ResizeY1Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getLocation().moveRel (0, dy);
	rectangle.getSize().resizeRel (0, -dy);
}

void ResizeX2Y1Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getLocation().moveRel (0, dy);
	rectangle.getSize().resizeRel (dx, -dy);
}

void ResizeX2Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getSize().resizeRel (dx, 0);
}

void ResizeX2Y2Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getSize().resizeRel (dx, dy);
}

void ResizeY2Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getSize().resizeRel (0, dy);
}

void ResizeX1Y2Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getLocation().moveRel (dx, 0);
	rectangle.getSize().resizeRel (-dx, dy);	
}

void ResizeX1Policy::resize(Rectangle& rectangle, int dx, int dy)
{
	rectangle.getLocation().moveRel (dx, 0);
	rectangle.getSize().resizeRel (-dx, 0);	
}

}

