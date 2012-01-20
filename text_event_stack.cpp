#include "text_event_stack.h"

namespace cppgef
{

TextEventStack::TextEventStack() :
	working_( false )
{
}

TextEventStack::~TextEventStack()
{
}

bool TextEventStack::isWorking() const
{
	return working_;
}

void TextEventStack::storeEvent(shared_ptr< ITextEvent > event)
{
	undo_stack_.push (event);
	
	// Clear redo stack
	while (!redo_stack_.empty())
	{
		redo_stack_.pop();
	}	
}

bool TextEventStack::TextEventStack::undo()
{
	if (undo_stack_.empty())
		return false;
		
	shared_ptr< ITextEvent > event = undo_stack_.top();
	undo_stack_.pop();
	
	working_ = true;
	event->undo();
	working_ = false;
	
	redo_stack_.push (event);
	
	return true;
}

bool TextEventStack::TextEventStack::redo()
{
	if (redo_stack_.empty())
		return false;
		
	shared_ptr< ITextEvent > event = redo_stack_.top();
	redo_stack_.pop();
	
	working_ = true;
	event->redo();
	working_ = false;
	
	undo_stack_.push (event);
	
	return true;
}

}

