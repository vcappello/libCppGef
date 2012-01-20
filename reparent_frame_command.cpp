#include "reparent_frame_command.h"

namespace cppgef
{

ReparentFrameCommand::ReparentFrameCommand(shared_ptr< Diagram > diagram, shared_ptr< Frame > frame, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy) :
	diagram_( diagram ),
	frame_( frame ),
	old_parent_( old_parent ),
	new_parent_( new_parent ),
	dx_( dx ),
	dy_( dy )
{
}

ReparentFrameCommand::~ReparentFrameCommand()
{
}

shared_ptr<Diagram> ReparentFrameCommand::getDiagram()
{
	return diagram_;
}

bool ReparentFrameCommand::allowUndo() const
{
	return true;
}

bool ReparentFrameCommand::execute()
{
	old_parent_->eraseChild (frame_);
	new_parent_->addChild (frame_);
	
	frame_->getBounds().getLocation().moveRel (dx_, dy_);
	
	return true;	
}

bool ReparentFrameCommand::unexecute()
{
	new_parent_->eraseChild (frame_);
	old_parent_->addChild (frame_);
	
	frame_->getBounds().getLocation().moveRel (-dx_, -dy_);

	return true;	
}

}

