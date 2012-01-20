#include "remove_tag_text_event.h"

namespace cppgef
{

RemoveTagTextEvent::RemoveTagTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, const Glib::ustring& tag_name, int start_pos, int end_pos) :
	text_buffer_( text_buffer ),
	tag_name_( tag_name ),
	start_pos_( start_pos ),
	end_pos_( end_pos )
{
}

RemoveTagTextEvent::~RemoveTagTextEvent()
{
}

void RemoveTagTextEvent::undo()
{
	text_buffer_->apply_tag_by_name (
		tag_name_, 
		text_buffer_->get_iter_at_offset (start_pos_),
		text_buffer_->get_iter_at_offset (end_pos_));	
}

void RemoveTagTextEvent::redo()
{
	text_buffer_->remove_tag_by_name (
		tag_name_, 
		text_buffer_->get_iter_at_offset (start_pos_),
		text_buffer_->get_iter_at_offset (end_pos_));	
}

}

