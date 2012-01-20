#ifndef INSERTTEXTEVENT_H
#define INSERTTEXTEVENT_H

#include <i_text_event.h> // Base class: cppgef::ITextEvent

#include <gtkmm.h>

namespace cppgef
{

class InsertTextEvent : public cppgef::ITextEvent
{

public:
	InsertTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, const Glib::ustring& inserted_text, int pos);
	virtual ~InsertTextEvent();

public:
	virtual void undo();
	virtual void redo();
	
protected:
	Glib::RefPtr< Gtk::TextBuffer > text_buffer_;
	Glib::ustring inserted_text_;
	int pos_;
};

}

#endif // INSERTTEXTEVENT_H
