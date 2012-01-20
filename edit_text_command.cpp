#include "edit_text_command.h"

namespace cppgef
{

EditTextCommand::EditTextCommand(shared_ptr< Diagram > diagram, shared_ptr< ITextElement> text_element, const Glib::ustring& new_value) :
	diagram_( diagram ),
	text_element_( text_element ),
	new_value_( new_value ),
	old_value_( text_element->getText() )
{
}

EditTextCommand::~EditTextCommand()
{
}

shared_ptr<Diagram> EditTextCommand::getDiagram()
{
	return diagram_;
}

bool EditTextCommand::allowUndo() const
{
	return true;
}

bool EditTextCommand::execute()
{
	text_element_->setText (new_value_);
	
	return true;
}

bool EditTextCommand::unexecute()
{
	text_element_->setText (old_value_);
	
	return true;
}
	
}

