#include "reparent_curve_command.h"

namespace cppgef
{

ReparentCurveCommand::ReparentCurveCommand(shared_ptr< Diagram > diagram, shared_ptr< CurveLine > curve, shared_ptr< ComplexModelElementBase > old_parent, shared_ptr< ComplexModelElementBase > new_parent, int dx, int dy) :
	diagram_( diagram ),
	curve_( curve ),
	old_parent_( old_parent ),
	new_parent_( new_parent ),
	dx_( dx ),
	dy_( dy )
{
}

ReparentCurveCommand::~ReparentCurveCommand()
{
}

shared_ptr<Diagram> ReparentCurveCommand::getDiagram()
{
	return diagram_;
}

bool ReparentCurveCommand::allowUndo() const
{
	return true;
}

bool ReparentCurveCommand::execute()
{
	old_parent_->eraseChild (curve_);
	new_parent_->addChild (curve_);
	
	curve_->getStartPoint().moveRel (dx_, dy_);
	curve_->getEndPoint().moveRel (dx_, dy_);
	curve_->getStartControlPoint().moveRel (dx_, dy_);
	curve_->getEndControlPoint().moveRel (dx_, dy_);
	
	return true;	
}

bool ReparentCurveCommand::unexecute()
{
	new_parent_->eraseChild (curve_);
	old_parent_->addChild (curve_);
	
	curve_->getStartPoint().moveRel (-dx_, -dy_);
	curve_->getEndPoint().moveRel (-dx_, -dy_);
	curve_->getStartControlPoint().moveRel (-dx_, -dy_);
	curve_->getEndControlPoint().moveRel (-dx_, -dy_);
	
	return true;	
}

}

