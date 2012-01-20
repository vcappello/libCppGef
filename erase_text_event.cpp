#include "erase_text_event.h"

namespace cppgef
{

EraseTextEvent::EraseTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, int start_pos, int end_pos) :
	text_buffer_( text_buffer ),
	start_pos_( start_pos ),
	end_pos_( end_pos )
{
	deleted_text_ = text_buffer_->get_text (
		text_buffer_->get_iter_at_offset (start_pos_),
		text_buffer_->get_iter_at_offset (end_pos_));
}

EraseTextEvent::~EraseTextEvent()
{
}

void EraseTextEvent::undo()
{
	text_buffer_->insert (
		text_buffer_->get_iter_at_offset (start_pos_),
		deleted_text_);	
}

void EraseTextEvent::redo()
{
	text_buffer_->erase (
		text_buffer_->get_iter_at_offset (start_pos_),
		text_buffer_->get_iter_at_offset (end_pos_));	
}

}

