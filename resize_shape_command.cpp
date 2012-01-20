#include "resize_shape_command.h"

namespace cppgef
{

ResizeShapeCommand::ResizeShapeCommand(shared_ptr< Diagram > diagram, shared_ptr< ShapeBase > shape, AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy) :
	diagram_( diagram ),
	shape_( shape ),
	resize_direction_( resize_direction ),
	dx_( dx ),
	dy_( dy )
{
	resize_policy_ = AbstractResizePolicy::create (resize_direction_, shape_->getBounds());
}

ResizeShapeCommand::~ResizeShapeCommand()
{
}

shared_ptr<Diagram> ResizeShapeCommand::getDiagram()
{
	return diagram_;	
}

bool ResizeShapeCommand::allowUndo() const
{
	return true;
}

bool ResizeShapeCommand::execute()
{
	resize_policy_->resize (shape_->getBounds(), dx_, dy_);
	
	return true;
}

bool ResizeShapeCommand::unexecute()
{
	resize_policy_->resize (shape_->getBounds(), -dx_, -dy_);
	
	return true;
}

}

