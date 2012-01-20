#include "move_curve_command.h"

namespace cppgef
{

MoveCurveCommand::MoveCurveCommand(shared_ptr< Diagram > diagram, shared_ptr< CurveLine > curve, int dx, int dy) :
	diagram_( diagram ),
	curve_( curve ),
	dx_( dx ),
	dy_( dy )
{
}

MoveCurveCommand::~MoveCurveCommand()
{
}

shared_ptr<Diagram> MoveCurveCommand::getDiagram()
{
	return diagram_;
}

bool MoveCurveCommand::allowUndo() const
{
	return true;
}

bool MoveCurveCommand::execute()
{
	curve_->getStartPoint().moveRel (dx_, dy_);
	curve_->getEndPoint().moveRel (dx_, dy_);
	curve_->getStartControlPoint().moveRel (dx_, dy_);
	curve_->getEndControlPoint().moveRel (dx_, dy_);
	
	return true;
}

bool MoveCurveCommand::unexecute()
{
	curve_->getStartPoint().moveRel (-dx_, -dy_);
	curve_->getEndPoint().moveRel (-dx_, -dy_);
	curve_->getStartControlPoint().moveRel (-dx_, -dy_);
	curve_->getEndControlPoint().moveRel (-dx_, -dy_);
	
	return true;
}

}

