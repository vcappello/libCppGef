#include "set_dash_style_command.h"

namespace cppgef
{

SetDashStyleCommand::SetDashStyleCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportLineStyle > element, const std::vector< double >& dash_style, double dash_offset) :
	diagram_( diagram ),
	element_( element ),
	old_dash_style_( element->getLineDashStyle() ),
	old_dash_offset_( element->getLineDashStyleOffset() ),
	new_dash_style_( dash_style ),
	new_dash_offset_( dash_offset )
{
}

SetDashStyleCommand::~SetDashStyleCommand()
{
}

shared_ptr<Diagram> SetDashStyleCommand::getDiagram()
{
	return diagram_;
}

bool SetDashStyleCommand::allowUndo() const
{
	return true;
}

bool SetDashStyleCommand::execute()
{
	element_->setLineDashStyle (new_dash_style_);
	element_->setLineDashStyleOffset (new_dash_offset_);
	
	return true;
}

bool SetDashStyleCommand::unexecute()
{
	element_->setLineDashStyle (old_dash_style_);
	element_->setLineDashStyleOffset (old_dash_offset_);
	
	return true;
}

}

