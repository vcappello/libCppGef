#ifndef APPLYTAGTEXTEVENT_H
#define APPLYTAGTEXTEVENT_H

#include <i_text_event.h> // Base class: cppgef::ITextEvent

#include <gtkmm.h>

namespace cppgef
{

class ApplyTagTextEvent : public cppgef::ITextEvent
{

public:
	ApplyTagTextEvent(Glib::RefPtr< Gtk::TextBuffer > text_buffer, const Glib::ustring& tag_name, int start_pos, int end_pos);
	virtual ~ApplyTagTextEvent();

public:
	virtual void undo();
	virtual void redo();
	
protected:
	Glib::RefPtr< Gtk::TextBuffer > text_buffer_;
	Glib::ustring tag_name_;
	int start_pos_;
	int end_pos_;
};

}

#endif // APPLYTAGTEXTEVENT_H
