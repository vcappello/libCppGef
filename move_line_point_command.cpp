#include "move_line_point_command.h"

namespace cppgef
{

MoveLinePointCommand::MoveLinePointCommand(shared_ptr< Diagram > diagram, shared_ptr< LineBase > simple_line, IMovePointPolicy::PointSequence sequence, int dx, int dy) :
	diagram_( diagram ),
	simple_line_( simple_line ),
	sequence_( sequence ),
	dx_( dx ),
	dy_( dy )
{
}

MoveLinePointCommand::~MoveLinePointCommand()
{
}

shared_ptr<Diagram> MoveLinePointCommand::getDiagram()
{
	return diagram_;
}

bool MoveLinePointCommand::allowUndo() const
{
	return true;
}

bool MoveLinePointCommand::execute()
{
	if (sequence_ == IMovePointPolicy::StartPoint)
		simple_line_->getStartPoint().moveRel (dx_, dy_);
	else if (sequence_ == IMovePointPolicy::EndPoint)
		simple_line_->getEndPoint().moveRel (dx_, dy_);
		
	return true;
}

bool MoveLinePointCommand::unexecute()
{
	if (sequence_ == IMovePointPolicy::StartPoint)
		simple_line_->getStartPoint().moveRel (-dx_, -dy_);
	else if (sequence_ == IMovePointPolicy::EndPoint)
		simple_line_->getEndPoint().moveRel (-dx_, -dy_);
		
	return true;
}

}

