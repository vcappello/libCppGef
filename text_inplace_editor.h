#ifndef TEXTINPLACEEDITOR_H
#define TEXTINPLACEEDITOR_H

#include <i_inplace_editor.h> // Base class: cppgef::IInplaceEditor

#include <set>

#include <pango_text_buffer.h>
#include <label_figure.h>
#include <text_format.h>
#include <color.h>
#include <text_event_stack.h>

namespace cppgef
{

/**
 * @class TextInplaceEditor
 * @author vincenzo.cappello
 * @date 04/11/2011
 * @file text_inplace_editor.h
 * @brief Editor for text figure cppgef::LabelFigure
 */
class TextInplaceEditor : public cppgef::IInplaceEditor
{
public:
	typedef sigc::signal< void, const cppgef::TextFormat& > signal_update_style_t;
	
public:
	TextInplaceEditor(shared_ptr< LabelFigure > label);
	virtual ~TextInplaceEditor();

public:
	/**
	 * @brief The cppgef::LabelFigure rectangle bounds
	 * @return the cppgef::LabelFigure rectangle bounds
	 */
	virtual Rectangle getBounds() const;
	
	/**
	 * @brief Return the widget for edit text, previously
	 * created by TextInplaceEditor::createWidget()
	 * @return the widget for edit text
	 */
	virtual Gtk::Widget* getWidget();
	
	/**
	 * @brief Create the Gtk::Widget for edit text. The
	 * widget is a Gtk::TextView
	 * @return the Gtk::TextView widget for edit text
	 */
	virtual Gtk::Widget* createWidget();
	
	/**
	 * @brief Apply current edited text to the binded cppgef::LabelFigure
	 */
	virtual void apply();
	
	virtual void executeUndo();
	virtual void executeRedo();
	virtual void executeCut();
	virtual void executeCopy();
	virtual void executePaste();
	
	void setBold (bool value);
	void setItalic (bool value);
	void setUnderline (bool value);
	void setFontFamily(const Glib::ustring& font_family);
	void setFontSize(const int font_size);
	void setTextColor(const cppgef::Color& color);
	void setTextBackColor(const cppgef::Color& color);
	void setTextBackColorNone();
	
	TextFormat getTextFormat();
	
	void setCursorPosition(const Point& point);
	
	virtual signal_stop_edit_t signalStopEdit();
	virtual signal_cancel_edit_t signalCancelEdit();
	
	signal_update_style_t signalUpdateStyle();
	
protected:
	Gtk::TextView* widget_;
	shared_ptr< LabelFigure > label_;
	
	TextFormat prev_text_format_;
	bool force_signal_update_style_;
	TextEventStack text_event_stack_;

	signal_update_style_t signal_update_style_;
	signal_cancel_edit_t signal_cancel_edit_;
	signal_stop_edit_t signal_stop_edit_;
	
protected:
	void setSelectionTag (Glib::RefPtr<Gtk::TextBuffer::Tag> tag, bool value);
	void setSelectionUDefTag (const Glib::ustring& prefix, Glib::RefPtr<Gtk::TextBuffer::Tag> tag);
	
	void onTextBufferMarkSet(const Gtk::TextBuffer::iterator& location, const Glib::RefPtr<Gtk::TextBuffer::Mark>& mark);
	
	void onTextBufferInsert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text, int bytes);
	void onTextBufferErase(const Gtk::TextBuffer::iterator& start, const Gtk::TextBuffer::iterator& end);
	void onTextBufferApplyTag(const Glib::RefPtr< Gtk::TextBuffer::Tag >& tag, const Gtk::TextBuffer::iterator& range_begin, const Gtk::TextBuffer::iterator& range_end);
	void onTextBufferRemoveTag(const Glib::RefPtr< Gtk::TextBuffer::Tag >& tag, const Gtk::TextBuffer::iterator& range_begin, const Gtk::TextBuffer::iterator& range_end);
	bool onWidgetKeyPressEvent(GdkEventKey* event);
};

}

#endif // TEXTINPLACEEDITOR_H
