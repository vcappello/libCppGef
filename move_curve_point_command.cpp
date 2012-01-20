#include "move_curve_point_command.h"

namespace cppgef
{

MoveCurvePointCommand::MoveCurvePointCommand(shared_ptr< Diagram > diagram, shared_ptr< CurveLine > curve, IMovePointPolicy::PointSequence sequence, int dx, int dy) :
	diagram_( diagram ),
	curve_( curve ),
	sequence_( sequence ),
	dx_( dx ),
	dy_( dy )
{
}

MoveCurvePointCommand::~MoveCurvePointCommand()
{
}

shared_ptr<Diagram> MoveCurvePointCommand::getDiagram()
{
	return diagram_;
}

bool MoveCurvePointCommand::allowUndo() const
{
	return true;
}

bool MoveCurvePointCommand::execute()
{
	switch (sequence_)
	{
		case IMovePointPolicy::CurveStartPoint:
			curve_->getStartPoint().moveRel (dx_, dy_);
			curve_->getStartControlPoint().moveRel (dx_, dy_);
			break;
		case IMovePointPolicy::CurveEndPoint:
			curve_->getEndPoint().moveRel (dx_, dy_);
			curve_->getEndControlPoint().moveRel (dx_, dy_);
			break;
		case IMovePointPolicy::CurveStartControlPoint:
			curve_->getStartControlPoint().moveRel (dx_, dy_);
			break;
		case IMovePointPolicy::CurveEndControlPoint:
			curve_->getEndControlPoint().moveRel (dx_, dy_);
			break;
	}
	
	return true;
}

bool MoveCurvePointCommand::unexecute()
{
	switch (sequence_)
	{
		case IMovePointPolicy::CurveStartPoint:
			curve_->getStartPoint().moveRel (-dx_, -dy_);
			curve_->getStartControlPoint().moveRel (-dx_, -dy_);
			break;
		case IMovePointPolicy::CurveEndPoint:
			curve_->getEndPoint().moveRel (-dx_, -dy_);
			curve_->getEndControlPoint().moveRel (-dx_, -dy_);
			break;
		case IMovePointPolicy::CurveStartControlPoint:
			curve_->getStartControlPoint().moveRel (-dx_, -dy_);
			break;
		case IMovePointPolicy::CurveEndControlPoint:
			curve_->getEndControlPoint().moveRel (-dx_, -dy_);
			break;
	}
	
	return true;
}

}

