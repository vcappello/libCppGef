#ifndef PANGOTEXTBUFFER_H
#define PANGOTEXTBUFFER_H

#include <gtkmm.h>

namespace cppgef
{
	
class PangoTextBuffer
{
public:
	PangoTextBuffer();
	virtual ~PangoTextBuffer();

	Glib::RefPtr<Gtk::TextBuffer> createTextBuffer(const Glib::ustring& markup);
	Glib::ustring createMarkup(Glib::RefPtr<Gtk::TextBuffer> text_buffer);
	
protected:
	Glib::ustring tagToMarkup(Glib::RefPtr<Gtk::TextTag> tag);
	
	Glib::ustring translatePangoWeight(Pango::Weight weight);
	Glib::ustring translatePangoStyle(Pango::Style style);
	Glib::ustring translatePangoUnderline(Pango::Underline underline);
};

}

#endif // PANGOTEXTBUFFER_H
