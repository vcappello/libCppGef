#include "insert_text_event.h"

namespace cppgef
{

InsertTextEvent::InsertTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, const Glib::ustring& inserted_text, int pos) :
	text_buffer_( text_buffer ),
	inserted_text_( inserted_text ),
	pos_( pos )
{
}

InsertTextEvent::~InsertTextEvent()
{
}

void InsertTextEvent::undo()
{
	text_buffer_->erase (
		text_buffer_->get_iter_at_offset (pos_),
		text_buffer_->get_iter_at_offset (pos_ + inserted_text_.length()));
}

void InsertTextEvent::redo()
{
	text_buffer_->insert (
		text_buffer_->get_iter_at_offset (pos_),
		inserted_text_);
}

}

