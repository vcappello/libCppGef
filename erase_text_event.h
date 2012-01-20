#ifndef ERASETEXTEVENT_H
#define ERASETEXTEVENT_H

#include <i_text_event.h> // Base class: cppgef::ITextEvent

#include <gtkmm.h>

namespace cppgef
{

class EraseTextEvent : public cppgef::ITextEvent
{

public:
	EraseTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, int start_pos, int end_pos);
	virtual ~EraseTextEvent();

public:
	virtual void undo();
	virtual void redo();
	
protected:
	Glib::RefPtr< Gtk::TextBuffer > text_buffer_;
	int start_pos_;
	int end_pos_;
	Glib::ustring deleted_text_;
};

}

#endif // ERASETEXTEVENT_H
