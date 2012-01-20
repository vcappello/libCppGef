#include "set_line_color_command.h"

namespace cppgef
{

SetLineColorCommand::SetLineColorCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportLineStyle > element, const Color& color) :
	diagram_( diagram ),
	element_( element ),
	old_color_( element->getLineColor() ),
	new_color_( color )
{
}

SetLineColorCommand::~SetLineColorCommand()
{
}

shared_ptr<Diagram> SetLineColorCommand::getDiagram()
{
	return diagram_;
}

bool SetLineColorCommand::allowUndo() const
{
	return true;
}

bool SetLineColorCommand::execute()
{
	element_->setLineColor (new_color_);
	
	return true;
}

bool SetLineColorCommand::unexecute()
{
	element_->setLineColor (old_color_);
	
	return true;
}

}
