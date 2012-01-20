#include "bounds_frame.h"

#include <frame.h>
#include <group_figure.h>

namespace cppgef
{

BoundsFrame::BoundsFrame()
{
}

BoundsFrame::~BoundsFrame()
{
}

void BoundsFrame::setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds)
{
	shared_ptr< Frame > frame = dynamic_pointer_cast< Frame >(element);
	
	if (frame)
		frame->setBounds (bounds);
}

void BoundsFrame::setBounds(shared_ptr< Figure > figure, const Rectangle& bounds)
{
	shared_ptr< GroupFigure > group_figure = dynamic_pointer_cast< GroupFigure >(figure);
	
	if (group_figure)
		group_figure->setBounds (bounds);	
}

}

