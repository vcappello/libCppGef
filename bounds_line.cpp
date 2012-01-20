#include "bounds_line.h"

#include <line_base.h>
#include <line_figure.h>

namespace cppgef
{

BoundsLine::BoundsLine()
{
}

BoundsLine::~BoundsLine()
{
}

void BoundsLine::setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds)
{
	shared_ptr< LineBase > line_element = dynamic_pointer_cast< LineBase >(element);
	
	if (line_element)
	{
		Point pt(bounds.getLocation());
		
		line_element->setStartPoint (pt);
		
		pt.moveRel (bounds.getSize());
		
		line_element->setEndPoint (pt);
	}
}

void BoundsLine::setBounds(shared_ptr< Figure > figure, const Rectangle& bounds)
{
	shared_ptr< LineFigure > line_figure = dynamic_pointer_cast< LineFigure >(figure);
	
	if (line_figure)
	{
		Point pt(bounds.getLocation());
		
		line_figure->setStartPoint (pt);
		
		pt.moveRel (bounds.getSize());
		
		line_figure->setEndPoint (pt);
	}	
}

}

