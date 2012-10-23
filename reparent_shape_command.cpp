#include "reparent_shape_command.h"

namespace cppgef
{

ReparentShapeCommand::ReparentShapeCommand(shared_ptr< Diagram > diagram, shared_ptr< ShapeBase > shape, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy) :
	diagram_( diagram ),
	shape_( shape ),
	old_parent_( old_parent ),
	new_parent_( new_parent ),
	dx_( dx ),
	dy_( dy )
{
	index_ = old_parent_->getChildIndex (shape_);
}

ReparentShapeCommand::~ReparentShapeCommand()
{
}

shared_ptr<Diagram> ReparentShapeCommand::getDiagram()
{
	return diagram_;
}

bool ReparentShapeCommand::allowUndo() const
{
	return true;
}

bool ReparentShapeCommand::execute()
{
	old_parent_->eraseChild (shape_);
	new_parent_->addChild (shape_);
	
	shape_->getBounds().getLocation().moveRel (dx_, dy_);
	
	return true;
}

bool ReparentShapeCommand::unexecute()
{
	new_parent_->eraseChild (shape_);
	old_parent_->insertChild (shape_, index_);
	
	shape_->getBounds().getLocation().moveRel (-dx_, -dy_);
	
	return true;
}

}

