#include "resize_frame_command.h"

namespace cppgef
{

ResizeFrameCommand::ResizeFrameCommand(shared_ptr< Diagram > diagram, shared_ptr< Frame > frame, AbstractResizePolicy::ResizeDirection resize_direction, int dx, int dy) :
	diagram_( diagram ),
	frame_( frame ),
	resize_direction_( resize_direction ),
	dx_( dx ),
	dy_( dy )
{
	resize_policy_ = AbstractResizePolicy::create (resize_direction_, frame_->getBounds());
}

ResizeFrameCommand::~ResizeFrameCommand()
{
}

shared_ptr<Diagram> ResizeFrameCommand::getDiagram()
{
	return diagram_;	
}

bool ResizeFrameCommand::allowUndo() const
{
	return true;
}

bool ResizeFrameCommand::execute()
{
	resize_policy_->resize (frame_->getBounds(), dx_, dy_);
	
	return true;
}

bool ResizeFrameCommand::unexecute()
{
	resize_policy_->resize (frame_->getBounds(), -dx_, -dy_);
	
	return true;
}

}

