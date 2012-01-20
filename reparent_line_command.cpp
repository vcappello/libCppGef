#include "reparent_line_command.h"

namespace cppgef
{

ReparentLineCommand::ReparentLineCommand(shared_ptr< Diagram > diagram, shared_ptr< LineBase > simple_line, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy) :
	diagram_( diagram ),
	simple_line_( simple_line ),
	old_parent_( old_parent ),
	new_parent_( new_parent ),
	dx_( dx ),
	dy_( dy )
{
}

ReparentLineCommand::~ReparentLineCommand()
{
}

shared_ptr<Diagram> ReparentLineCommand::getDiagram()
{
	return diagram_;
}

bool ReparentLineCommand::allowUndo() const
{
	return true;
}

bool ReparentLineCommand::execute()
{
	old_parent_->eraseChild (simple_line_);
	new_parent_->addChild (simple_line_);
	
	simple_line_->getStartPoint().moveRel (dx_, dy_);
	simple_line_->getEndPoint().moveRel (dx_, dy_);
	
	return true;	
}

bool ReparentLineCommand::unexecute()
{
	new_parent_->eraseChild (simple_line_);
	old_parent_->addChild (simple_line_);
	
	simple_line_->getStartPoint().moveRel (-dx_, -dy_);
	simple_line_->getEndPoint().moveRel (-dx_, -dy_);
	
	return true;	
}

}

