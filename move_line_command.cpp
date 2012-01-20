#include "move_line_command.h"

namespace cppgef
{

MoveLineCommand::MoveLineCommand(shared_ptr< Diagram > diagram, shared_ptr< LineBase > simple_line, int dx, int dy) :
	diagram_( diagram ),
	simple_line_( simple_line ),
	dx_( dx ),
	dy_( dy )
{
}

MoveLineCommand::~MoveLineCommand()
{
}

shared_ptr<Diagram> MoveLineCommand::getDiagram()
{
	return diagram_;
}

bool MoveLineCommand::allowUndo() const
{
	return true;
}

bool MoveLineCommand::execute()
{
	simple_line_->getStartPoint().moveRel (dx_, dy_);
	simple_line_->getEndPoint().moveRel (dx_, dy_);
	
	return true;
}

bool MoveLineCommand::unexecute()
{
	simple_line_->getStartPoint().moveRel (-dx_, -dy_);
	simple_line_->getEndPoint().moveRel (-dx_, -dy_);
	
	return true;
}

}

