#include "bounds_shape.h"

#include <shape_base.h>
#include <rect_figure_base.h>

namespace cppgef
{

BoundsShape::BoundsShape()
{
}

BoundsShape::~BoundsShape()
{
}

void BoundsShape::setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds)
{
	shared_ptr< ShapeBase > shape = dynamic_pointer_cast< ShapeBase >(element);
	
	if (shape)
		shape->setBounds (bounds);
}

void BoundsShape::setBounds(shared_ptr< Figure > figure, const Rectangle& bounds)
{
	shared_ptr< RectFigureBase > rect_figure = dynamic_pointer_cast< RectFigureBase >(figure);
	
	if (rect_figure)
		rect_figure->setBounds (bounds);
}

}

