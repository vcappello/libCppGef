#include "move_shape_command.h"

namespace cppgef
{

MoveShapeCommand::MoveShapeCommand(shared_ptr< Diagram > diagram, const shared_ptr< ShapeBase > shape, int dx, int dy) :
	diagram_( diagram ),
	shape_( shape ),
	dx_( dx ),
	dy_( dy )
{
}

MoveShapeCommand::~MoveShapeCommand()
{
}

shared_ptr<Diagram> MoveShapeCommand::getDiagram()
{
	return diagram_;
}

bool MoveShapeCommand::allowUndo() const
{
	return true;
}

bool MoveShapeCommand::execute()
{
	shape_->getBounds().getLocation().moveRel (dx_, dy_);
	
	return true;
}

bool MoveShapeCommand::unexecute()
{
	shape_->getBounds().getLocation().moveRel (-dx_, -dy_);
	
	return true;
}

}

