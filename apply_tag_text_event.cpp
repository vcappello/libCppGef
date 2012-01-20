#include "apply_tag_text_event.h"

namespace cppgef
{

ApplyTagTextEvent::ApplyTagTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, const Glib::ustring& tag_name, int start_pos, int end_pos) :
	text_buffer_( text_buffer ),
	tag_name_( tag_name ),
	start_pos_( start_pos ),
	end_pos_( end_pos )
{
}

ApplyTagTextEvent::~ApplyTagTextEvent()
{
}

void ApplyTagTextEvent::undo()
{
	text_buffer_->remove_tag_by_name (
		tag_name_, 
		text_buffer_->get_iter_at_offset (start_pos_),
		text_buffer_->get_iter_at_offset (end_pos_));
}

void ApplyTagTextEvent::redo()
{
	text_buffer_->apply_tag_by_name (
		tag_name_, 
		text_buffer_->get_iter_at_offset (start_pos_),
		text_buffer_->get_iter_at_offset (end_pos_));
}

}

