#include "move_frame_command.h"

namespace cppgef
{

MoveFrameCommand::MoveFrameCommand(shared_ptr< Diagram > diagram, const shared_ptr< Frame > frame, int dx, int dy) :
	diagram_( diagram ),
	frame_( frame ),
	dx_( dx ),
	dy_( dy )
{
}

MoveFrameCommand::~MoveFrameCommand()
{
}

shared_ptr<Diagram> MoveFrameCommand::getDiagram()
{
	return diagram_;
}

bool MoveFrameCommand::allowUndo() const
{
	return true;
}

bool MoveFrameCommand::execute()
{
	frame_->getBounds().getLocation().moveRel (dx_, dy_);
	
	return true;
}

bool MoveFrameCommand::unexecute()
{
	frame_->getBounds().getLocation().moveRel (-dx_, -dy_);
	
	return true;
}

}

