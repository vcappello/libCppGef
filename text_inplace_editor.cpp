#include "text_inplace_editor.h"

#include <boost/lexical_cast.hpp>

#include <insert_text_event.h>
#include <erase_text_event.h>
#include <remove_tag_text_event.h>
#include <apply_tag_text_event.h>

namespace cppgef
{

TextInplaceEditor::TextInplaceEditor(shared_ptr< LabelFigure > label) :
	label_( label ),
	force_signal_update_style_( true )
{
}

TextInplaceEditor::~TextInplaceEditor()
{
}

Rectangle TextInplaceEditor::getBounds() const
{
	return label_->getBounds();
}

Gtk::Widget* TextInplaceEditor::getWidget()
{
	return widget_;
}

Gtk::Widget* TextInplaceEditor::createWidget()
{
	widget_ = new Gtk::TextView();
	
	PangoTextBuffer pango_buffer;
	Glib::RefPtr<Gtk::TextBuffer> buffer = pango_buffer.createTextBuffer (label_->getText());
	
	buffer->signal_mark_set().connect (
		sigc::mem_fun(this, &TextInplaceEditor::onTextBufferMarkSet));

#ifdef GTKMM_3
	widget_->override_font (label_->getTextStyle()->getFontDesc());
#else
	widget_->modify_font (label_->getTextStyle()->getFontDesc());
#endif
	
	widget_->set_left_margin (0);
	widget_->set_right_margin (0);

	widget_->set_wrap_mode (Gtk::WRAP_WORD);

	widget_->set_buffer (buffer);
	
	buffer->signal_insert().connect(
		sigc::mem_fun(this, &TextInplaceEditor::onTextBufferInsert), false);
	
	buffer->signal_erase().connect(
		sigc::mem_fun(this, &TextInplaceEditor::onTextBufferErase), false);

	buffer->signal_apply_tag().connect(
		sigc::mem_fun(this, &TextInplaceEditor::onTextBufferApplyTag));
	
	buffer->signal_remove_tag().connect(
		sigc::mem_fun(this, &TextInplaceEditor::onTextBufferRemoveTag));
	
	return widget_;
}

void TextInplaceEditor::apply()
{
	PangoTextBuffer pango_buffer;
	
	Glib::ustring markup = pango_buffer.createMarkup (widget_->get_buffer());
	
	label_->applyText (markup);
}

void TextInplaceEditor::executeUndo()
{
	if (!text_event_stack_.undo())
		signal_stop_edit_.emit();
}

void TextInplaceEditor::executeRedo()
{
	text_event_stack_.redo();
}

void TextInplaceEditor::executeCut()
{
	Glib::RefPtr< Gtk::Clipboard > clipboard = Gtk::Clipboard::get();
	widget_->get_buffer()->cut_clipboard (clipboard);
}

void TextInplaceEditor::executeCopy()
{
	Glib::RefPtr< Gtk::Clipboard > clipboard = Gtk::Clipboard::get();
	widget_->get_buffer()->copy_clipboard (clipboard);
}

void TextInplaceEditor::executePaste()
{
	Glib::RefPtr< Gtk::Clipboard > clipboard = Gtk::Clipboard::get();
	widget_->get_buffer()->paste_clipboard (clipboard);
}

void TextInplaceEditor::setBold (bool value)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();
	Glib::RefPtr<Gtk::TextTag> bold_tag = buffer->get_tag_table()->lookup ("udef-bold");
	setSelectionTag (bold_tag, value);
	
	force_signal_update_style_ = true;
}

void TextInplaceEditor::setItalic (bool value)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();
	Glib::RefPtr<Gtk::TextTag> italic_tag = buffer->get_tag_table()->lookup ("udef-italic");
	setSelectionTag (italic_tag, value);

	force_signal_update_style_ = true;
}

void TextInplaceEditor::setUnderline (bool value)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();
	Glib::RefPtr<Gtk::TextTag> underline_tag = buffer->get_tag_table()->lookup ("udef-underline");
	setSelectionTag (underline_tag, value);

	force_signal_update_style_ = true;
}

void TextInplaceEditor::setFontFamily(const Glib::ustring& font_family)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();
	
	Glib::ustring tag_id = "udef-font-family:" + font_family;
	
	Glib::RefPtr<Gtk::TextTag> font_tag_ = buffer->get_tag_table()->lookup (tag_id);
	if (!font_tag_)
	{
		font_tag_ = buffer->create_tag(tag_id);
		font_tag_->property_family() = font_family;
	}
	
	setSelectionUDefTag ("udef-font-family:", font_tag_);
	
	force_signal_update_style_ = true;
}

void TextInplaceEditor::setFontSize(const int font_size)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();
	
	Glib::ustring tag_id = Glib::ustring::compose ("udef-font-size:%1", font_size * Pango::SCALE);
	
	Glib::RefPtr<Gtk::TextTag> font_size_tag_ = buffer->get_tag_table()->lookup (tag_id);
	if (!font_size_tag_)
	{
		font_size_tag_ = buffer->create_tag(tag_id);
		font_size_tag_->property_size() = font_size * Pango::SCALE;
	}
	
	setSelectionUDefTag ("udef-font-size:", font_size_tag_);
	
	force_signal_update_style_ = true;	
}

void TextInplaceEditor::setTextColor(const cppgef::Color& color)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();

	Glib::ustring tag_id = Glib::ustring::compose ("udef-text-color:%1", color.toString());

	Glib::RefPtr<Gtk::TextTag> text_color_tag_ = buffer->get_tag_table()->lookup (tag_id);
	if (!text_color_tag_)
	{
		text_color_tag_ = buffer->create_tag(tag_id);
		text_color_tag_->property_foreground_gdk() = color.getGdkColor();
	}

	setSelectionUDefTag ("udef-text-color:", text_color_tag_);

	force_signal_update_style_ = true;
}

void TextInplaceEditor::setTextBackColor(const cppgef::Color& color)
{
	Glib::RefPtr<Gtk::TextBuffer> buffer = widget_->get_buffer();

	Glib::ustring tag_id = Glib::ustring::compose ("udef-text-back-color:%1", color.toString());

	Glib::RefPtr<Gtk::TextTag> text_backcolor_tag_ = buffer->get_tag_table()->lookup (tag_id);
	if (!text_backcolor_tag_)
	{
		text_backcolor_tag_ = buffer->create_tag(tag_id);
		text_backcolor_tag_->property_background_gdk() = color.getGdkColor();
	}

	setSelectionUDefTag ("udef-text-back-color:", text_backcolor_tag_);

	force_signal_update_style_ = true;
}

void TextInplaceEditor::setTextBackColorNone()
{
	Glib::RefPtr<Gtk::TextBuffer::Tag> null_tag;

	setSelectionUDefTag ("udef-text-back-color:", null_tag);

	force_signal_update_style_ = true;
}

IInplaceEditor::signal_stop_edit_t TextInplaceEditor::signalStopEdit()
{
	return signal_stop_edit_;
}

TextInplaceEditor::signal_update_style_t TextInplaceEditor::signalUpdateStyle()
{
	return signal_update_style_;
}

void TextInplaceEditor::setSelectionTag (Glib::RefPtr<Gtk::TextBuffer::Tag> tag, bool value)
{
	if (!widget_)
		return;
		
	Glib::RefPtr<Gtk::TextBuffer> entry_buffer;
    entry_buffer = widget_->get_buffer();
	
	if (entry_buffer)
	{
		if (entry_buffer->get_has_selection())
		{
			Gtk::TextBuffer::iterator selection_start;
			Gtk::TextBuffer::iterator selection_end;
			entry_buffer->get_selection_bounds (selection_start, selection_end);
			
			if (value)
				entry_buffer->apply_tag (tag, selection_start, selection_end);
			else
				entry_buffer->remove_tag (tag, selection_start, selection_end);
			
		}
	}		
}

void TextInplaceEditor::setSelectionUDefTag (const Glib::ustring& prefix, Glib::RefPtr<Gtk::TextBuffer::Tag> tag)
{
	if (!widget_)
		return;
		
	Glib::RefPtr<Gtk::TextBuffer> entry_buffer;
    entry_buffer = widget_->get_buffer();
	
	if (entry_buffer)
	{
		if (entry_buffer->get_has_selection())
		{
			Gtk::TextBuffer::iterator selection_start;
			Gtk::TextBuffer::iterator selection_end;
			entry_buffer->get_selection_bounds (selection_start, selection_end);

			// Remove all font style in the selection
			std::set< Glib::ustring > selected_fonts;
			
			for (Gtk::TextBuffer::iterator iter=selection_start; iter != selection_end; iter++)
			{
				Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> > tags = iter.get_tags();

				typedef Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> >::iterator tags_iterator;
				for (tags_iterator tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++)
				{
					Glib::ustring tag_name = (*tag_iter)->property_name();
					
					if (tag_name.substr(0, prefix.size()) == prefix)
					{
						selected_fonts.insert (tag_name);
					}
				}				
			}
			
			typedef std::set< Glib::ustring >::iterator font_iterator;
			for (font_iterator iter=selected_fonts.begin(); iter!=selected_fonts.end(); iter++)
			{
				entry_buffer->remove_tag_by_name (*iter, selection_start, selection_end);
			}
				
			// Apply new tag
			if (tag)
				entry_buffer->apply_tag (tag, selection_start, selection_end);
		}
	}		
}

TextFormat TextInplaceEditor::getTextFormat()
{
	TextFormat text_format;
	
	if (!widget_)
		return text_format; // TODO: throw exception
		
	Glib::RefPtr<Gtk::TextBuffer> entry_buffer;
    entry_buffer = widget_->get_buffer();
	
	if (entry_buffer)
	{
		Glib::RefPtr<Gtk::TextTag> bold_tag = entry_buffer->get_tag_table()->lookup ("udef-bold");
		Glib::RefPtr<Gtk::TextTag> italic_tag = entry_buffer->get_tag_table()->lookup ("udef-italic");
		Glib::RefPtr<Gtk::TextTag> underline_tag = entry_buffer->get_tag_table()->lookup ("udef-underline");
			
		if (entry_buffer->get_has_selection())
		{
			Gtk::TextBuffer::iterator selection_start;
			Gtk::TextBuffer::iterator selection_end;
			entry_buffer->get_selection_bounds (selection_start, selection_end);
			
			std::set< Glib::ustring > font_families;
			bool font_family_toggled = false;
			
			std::set< int > font_sizes;
			bool font_size_toggled = false;
			
			for (Gtk::TextBuffer::iterator iter = selection_start; iter != selection_end; iter++)
			{
				if (iter.has_tag (bold_tag))
				{
					text_format.setBold (true);
				}
				
				if (iter.has_tag (italic_tag))
				{
					text_format.setItalic (true);
				}
				
				if (iter.has_tag (underline_tag))
				{
					text_format.setUnderline (true);
				}
				
				Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> > tags = iter.get_tags();

				typedef Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> >::iterator tags_iterator;
				for (tags_iterator tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++)
				{
					Glib::ustring tag_name = (*tag_iter)->property_name();
					
					if (tag_name.substr(0, 17) == "udef-font-family:")
					{
						if (!font_family_toggled)
							font_family_toggled = iter.toggles_tag (*tag_iter);
							
						font_families.insert (tag_name.substr (17));
					}
					
					if (tag_name.substr(0, 15) == "udef-font-size:")
					{
						if (!font_size_toggled)
							font_size_toggled = iter.toggles_tag (*tag_iter);
							
						try
						{
							font_sizes.insert (boost::lexical_cast< int >(tag_name.substr (15)));
						}
						catch (boost::bad_lexical_cast &ex)
						{
							// TODO: manage exception
						}
					}					
				}					
			}
			
			if (font_families.empty())
			{
				text_format.setFontFamily (label_->getTextStyle()->getFontDesc().get_family());
			}
			else if (font_families.size() == 1 && !font_family_toggled)
			{
				text_format.setFontFamily (*(font_families.begin()));
			}
			else
			{
				text_format.setFontFamily ("");
			}
			
			if (font_sizes.empty())
			{
				text_format.setFontSize (label_->getTextStyle()->getFontDesc().get_size() / Pango::SCALE);
			}
			else if (font_sizes.size() == 1 && !font_size_toggled)
			{
				text_format.setFontSize (*(font_sizes.begin()) / Pango::SCALE);
			}
			else
			{
				text_format.setFontSize (0);
			}
		}
		else
		{
			Gtk::TextBuffer::iterator iter;
			
			iter = entry_buffer->get_iter_at_mark (entry_buffer->get_mark ("insert"));
			
			if (iter.has_tag (bold_tag))
			{
				text_format.setBold (true);
			}
			
			if (iter.has_tag (italic_tag))
			{
				text_format.setItalic (true);
			}
			
			if (iter.has_tag (underline_tag))
			{
				text_format.setUnderline (true);
			}
			
			Glib::ustring font_family;
			int font_size = 0;
			Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> > tags = iter.get_tags();

			typedef Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> >::iterator tags_iterator;
			for (tags_iterator tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++)
			{
				Glib::ustring tag_name = (*tag_iter)->property_name();
				
				if (tag_name.substr(0, 17) == "udef-font-family:")
				{
					font_family = tag_name.substr (17);
					break;
				}
			}
			for (tags_iterator tag_iter = tags.begin(); tag_iter != tags.end(); tag_iter++)
			{
				Glib::ustring tag_name = (*tag_iter)->property_name();
				
				if (tag_name.substr(0, 15) == "udef-font-size:")
				{
					try
					{
						font_size = (boost::lexical_cast< int >(tag_name.substr (15)));
					}
					catch (boost::bad_lexical_cast &ex)
					{
						font_size = 0;
						// TODO: manage exception
					}
					break;
				}				
			}
			
			if (font_family.empty())
			{
				font_family = label_->getTextStyle()->getFontDesc().get_family();
			}

			if (font_size == 0)
			{
				font_size = label_->getTextStyle()->getFontDesc().get_size();
			}
			
			text_format.setFontFamily (font_family);
			text_format.setFontSize (font_size / Pango::SCALE);
		}
	}
	
	return text_format;
}

void TextInplaceEditor::setCursorPosition(const Point& point)
{
	Gtk::TextBuffer::iterator iter;

	int x = point.getX() - label_->getBounds().getLeft();
	int y = point.getY() - label_->getBounds().getTop();
	
	widget_->window_to_buffer_coords (Gtk::TEXT_WINDOW_WIDGET, x, y, x, y);
	
	widget_->get_iter_at_location (iter, x, y);
	
	widget_->get_buffer()->move_mark_by_name ("insert", iter);
	widget_->get_buffer()->move_mark_by_name ("selection_bound", iter);
}

void TextInplaceEditor::onTextBufferMarkSet(const Gtk::TextBuffer::iterator& location, const Glib::RefPtr<Gtk::TextBuffer::Mark>& mark)
{
	if (!widget_)
		return;
		
	TextFormat text_format = getTextFormat();

	if (prev_text_format_ != text_format || force_signal_update_style_)
	{
		signal_update_style_.emit (text_format);
		prev_text_format_ = text_format;
		
		force_signal_update_style_ = false;
	}
}

void TextInplaceEditor::onTextBufferInsert(const Gtk::TextBuffer::iterator& pos, const Glib::ustring& text, int bytes)
{
	if (text_event_stack_.isWorking())
		return;
		
	text_event_stack_.storeEvent (
		shared_ptr< ITextEvent >( 
			new InsertTextEvent(
				widget_->get_buffer(),
				text,
				pos.get_offset() )));
}

void TextInplaceEditor::onTextBufferErase(const Gtk::TextBuffer::iterator& start, const Gtk::TextBuffer::iterator& end)
{
	if (text_event_stack_.isWorking())
		return;
		
	text_event_stack_.storeEvent (
		shared_ptr< ITextEvent >( 
			new EraseTextEvent(
				widget_->get_buffer(),
				start.get_offset(),
				end.get_offset() )));
}

void TextInplaceEditor::onTextBufferApplyTag(const Glib::RefPtr< Gtk::TextBuffer::Tag >& tag, const Gtk::TextBuffer::iterator& range_begin, const Gtk::TextBuffer::iterator& range_end)
{
	if (text_event_stack_.isWorking())
		return;
		
	text_event_stack_.storeEvent (
		shared_ptr< ITextEvent >( 
			new ApplyTagTextEvent(
				widget_->get_buffer(),
				tag->property_name(),
				range_begin.get_offset(),
				range_end.get_offset() )));	
}

void TextInplaceEditor::onTextBufferRemoveTag(const Glib::RefPtr< Gtk::TextBuffer::Tag >& tag, const Gtk::TextBuffer::iterator& range_begin, const Gtk::TextBuffer::iterator& range_end)
{
	if (text_event_stack_.isWorking())
		return;
		
	text_event_stack_.storeEvent (
		shared_ptr< ITextEvent >( 
			new RemoveTagTextEvent(
				widget_->get_buffer(),
				tag->property_name(),
				range_begin.get_offset(),
				range_end.get_offset() )));	
}

}

