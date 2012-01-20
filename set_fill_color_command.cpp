#include "set_fill_color_command.h"

namespace cppgef
{

SetFillColorCommand::SetFillColorCommand(shared_ptr< Diagram > diagram, shared_ptr< ISupportFillStyle > element, const Color& color) :
	diagram_( diagram ),
	element_( element ),
	old_color_( element->getFillColor() ),
	new_color_( color )
{
}

SetFillColorCommand::~SetFillColorCommand()
{
}

shared_ptr<Diagram> SetFillColorCommand::getDiagram()
{
	return diagram_;
}

bool SetFillColorCommand::allowUndo() const
{
	return true;
}

bool SetFillColorCommand::execute()
{
	element_->setFillColor (new_color_);
	
	return true;	
}

bool SetFillColorCommand::unexecute()
{
	element_->setFillColor (old_color_);
	
	return true;	
}

}

