#include "pango_text_buffer.h"

// This code can be removed once the following bug is closed:
// http://bugzilla.gnome.org/show_bug.cgi?id=59390

//#include <gtk/gtktextbuffer.h>
#include <gtk/gtk.h>

namespace cppgef
{

static void
text_buffer_insert_markup_real (GtkTextBuffer *buffer,
                                GtkTextIter   *textiter,
                                const gchar   *markup,
                                gint           len)
{
	PangoAttrIterator  *paiter;
	PangoAttrList      *attrlist;
	GtkTextMark        *mark;
	GError             *error = NULL;
	gchar              *text;

	g_return_if_fail (GTK_IS_TEXT_BUFFER (buffer));
	g_return_if_fail (textiter != NULL);
	g_return_if_fail (markup != NULL);
	g_return_if_fail (gtk_text_iter_get_buffer (textiter) == buffer);

	if (len == 0)
		return;

	if (!pango_parse_markup(markup, len, 0, &attrlist, &text, NULL, &error))
	{
		g_warning("Invalid markup string: %s", error->message);
		g_error_free(error);
		return;
	}

	len = strlen(text); /* TODO: is this needed? */

	if (attrlist == NULL)
	{
		gtk_text_buffer_insert(buffer, textiter, text, len);
		g_free(text);
		return;
	}

	/* create mark with right gravity */
	mark = gtk_text_buffer_create_mark(buffer, NULL, textiter, FALSE);

	paiter = pango_attr_list_get_iterator(attrlist);

	do
	{
		PangoAttribute *attr;
		GtkTextTag     *tag = NULL;
		gint            start, end;
		bool 			add_udef_italic_tag = false;
		bool 			add_udef_bold_tag = false;
		bool 			add_udef_underline_tag = false;
		bool 			add_udef_font_family_tag = false;
		Glib::ustring 	font_family_tag;
		bool 			add_udef_font_size_tag = false;
		Glib::ustring 	font_size_tag;
		bool 			add_udef_text_Colorag = false;
		Glib::ustring 	text_color_tag;
		bool 			add_udef_text_back_Colorag = false;
		Glib::ustring 	text_back_color_tag;
		
		pango_attr_iterator_range(paiter, &start, &end);

		if (end == G_MAXINT)  /* last chunk */
			end = strlen(text);

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_STYLE)))
		{
			if (((PangoAttrInt*)attr)->value == Pango::STYLE_ITALIC)
				add_udef_italic_tag = true;
		}
		
		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_WEIGHT)))
		{
			if (((PangoAttrInt*)attr)->value == Pango::WEIGHT_BOLD)
				add_udef_bold_tag = true;
		}
		
		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_UNDERLINE)))
		{
			if (((PangoAttrInt*)attr)->value == Pango::UNDERLINE_SINGLE)
				add_udef_underline_tag = true;
		}
		
		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_FAMILY)))
		{
			gchar* udef_font_family;
			udef_font_family = ((PangoAttrString*)attr)->value;
			
			font_family_tag = "udef-font-family:";
			font_family_tag += udef_font_family;
			
			GtkTextTag* text_tag = gtk_text_tag_table_lookup (gtk_text_buffer_get_tag_table(buffer), font_family_tag.c_str());
			
			if (!text_tag)
			{
				text_tag = gtk_text_tag_new (font_family_tag.c_str());
				g_object_set (text_tag, "family", udef_font_family, NULL);
				gtk_text_tag_table_add (gtk_text_buffer_get_tag_table(buffer), text_tag);
			}
			
			add_udef_font_family_tag = true;
		}

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_SIZE)))
		{
			int udef_font_size;
			udef_font_size = ((PangoAttrInt*)attr)->value;
			
			font_size_tag = Glib::ustring::compose ("udef-font-size:%1", udef_font_size);
			
			GtkTextTag* text_tag = gtk_text_tag_table_lookup (gtk_text_buffer_get_tag_table(buffer), font_size_tag.c_str());
			
			if (!text_tag)
			{
				text_tag = gtk_text_tag_new (font_size_tag.c_str());
				g_object_set (text_tag, "size", udef_font_size, NULL);
				gtk_text_tag_table_add (gtk_text_buffer_get_tag_table(buffer), text_tag);
			}
			
			add_udef_font_size_tag = true;			
		}

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_FOREGROUND)))
		{
			GdkColor udef_text_color = { 0,
			                 ((PangoAttrColor*)attr)->color.red,
			                 ((PangoAttrColor*)attr)->color.green,
			                 ((PangoAttrColor*)attr)->color.blue
			               };

			text_color_tag = Glib::ustring::compose ("udef-text-color:%1", gdk_color_to_string(&udef_text_color));

			GtkTextTag* text_tag = gtk_text_tag_table_lookup (gtk_text_buffer_get_tag_table(buffer), text_color_tag.c_str());

			if (!text_tag)
			{
				text_tag = gtk_text_tag_new (text_color_tag.c_str());
				g_object_set (text_tag, "foreground-gdk", &udef_text_color, NULL);
				gtk_text_tag_table_add (gtk_text_buffer_get_tag_table(buffer), text_tag);
			}

			add_udef_text_Colorag = true;
		}

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_BACKGROUND)))
		{
			GdkColor udef_text_back_color = { 0,
			                 ((PangoAttrColor*)attr)->color.red,
			                 ((PangoAttrColor*)attr)->color.green,
			                 ((PangoAttrColor*)attr)->color.blue
			               };

			text_back_color_tag = Glib::ustring::compose ("udef-text-back-color:%1", gdk_color_to_string(&udef_text_back_color));

			GtkTextTag* text_tag = gtk_text_tag_table_lookup (gtk_text_buffer_get_tag_table(buffer), text_back_color_tag.c_str());

			if (!text_tag)
			{
				text_tag = gtk_text_tag_new (text_back_color_tag.c_str());
				g_object_set (text_tag, "background-gdk", &udef_text_back_color, NULL);
				gtk_text_tag_table_add (gtk_text_buffer_get_tag_table(buffer), text_tag);
			}

			add_udef_text_back_Colorag = true;
		}

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_LANGUAGE)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "language", pango_language_to_string(((PangoAttrLanguage*)attr)->value), NULL);
		}
		
		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_VARIANT)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "variant", ((PangoAttrInt*)attr)->value, NULL);
		}
	
		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_STRETCH)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "stretch", ((PangoAttrInt*)attr)->value, NULL);			
		}

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_FONT_DESC)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "font-desc", ((PangoAttrFontDesc*)attr)->desc, NULL);
		}

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_STRIKETHROUGH)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "strikethrough", (gboolean)(((PangoAttrInt*)attr)->value != 0), NULL);
		}
		
		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_RISE)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "rise", ((PangoAttrInt*)attr)->value, NULL);
		}

		/* PANGO_ATTR_SHAPE cannot be defined via markup text */

		if ((attr = pango_attr_iterator_get(paiter, PANGO_ATTR_SCALE)))
		{
			if (!tag) tag = gtk_text_tag_new(NULL);
			g_object_set(tag, "scale", ((PangoAttrFloat*)attr)->value, NULL);
		}
		
		gtk_text_buffer_insert(buffer, textiter, text+start, end - start);
		
		GtkTextIter end_textiter;
		gtk_text_buffer_get_end_iter(buffer, &end_textiter);
		gtk_text_iter_backward_chars(&end_textiter, end - start);
		
		if (tag)
		{
			gtk_text_tag_table_add(gtk_text_buffer_get_tag_table(buffer), tag);
			gtk_text_buffer_apply_tag(buffer, tag, textiter, &end_textiter);
		}

		if (add_udef_bold_tag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, "udef-bold", textiter, &end_textiter);
		}
		
		if (add_udef_italic_tag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, "udef-italic", textiter, &end_textiter);
		}
		
		if (add_udef_underline_tag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, "udef-underline", textiter, &end_textiter);
		}

		if (add_udef_font_family_tag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, font_family_tag.c_str(), textiter, &end_textiter);
		}

		if (add_udef_font_size_tag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, font_size_tag.c_str(), textiter, &end_textiter);
		}

		if (add_udef_text_Colorag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, text_color_tag.c_str(), textiter, &end_textiter);
		}

		if (add_udef_text_back_Colorag)
		{
			gtk_text_buffer_apply_tag_by_name(buffer, text_back_color_tag.c_str(), textiter, &end_textiter);
		}

		/* mark had right gravity, so it should be
		 *  at the end of the inserted text now */
		gtk_text_buffer_get_iter_at_mark(buffer, textiter, mark);
	}
	while (pango_attr_iterator_next(paiter));

	gtk_text_buffer_delete_mark(buffer, mark);
	pango_attr_iterator_destroy(paiter);
	pango_attr_list_unref(attrlist);
	g_free(text);
}

static void
text_buffer_insert_markup (GtkTextBuffer *buffer,
                           GtkTextIter   *iter,
                           const gchar   *markup,
                           gint           len)
{
	text_buffer_insert_markup_real (buffer, iter, markup, len);
}



static void
text_buffer_set_markup (GtkTextBuffer *buffer,
                        const gchar   *markup,
                        gint           len)
{
	GtkTextIter start, end;

	g_return_if_fail (GTK_IS_TEXT_BUFFER (buffer));
	g_return_if_fail (markup != NULL);

	GtkTextTag *udef_bold_tag = gtk_text_tag_new("udef-bold");
	g_object_set(udef_bold_tag, "weight", Pango::WEIGHT_BOLD, NULL);
	gtk_text_tag_table_add(gtk_text_buffer_get_tag_table(buffer), udef_bold_tag);
	
	GtkTextTag *udef_italic_tag = gtk_text_tag_new("udef-italic");
	g_object_set(udef_italic_tag, "style", Pango::STYLE_ITALIC, NULL);
	gtk_text_tag_table_add(gtk_text_buffer_get_tag_table(buffer), udef_italic_tag);
	
	GtkTextTag *udef_underline_tag = gtk_text_tag_new("udef-underline");
	g_object_set(udef_underline_tag, "underline", Pango::UNDERLINE_SINGLE, NULL);
	gtk_text_tag_table_add(gtk_text_buffer_get_tag_table(buffer), udef_underline_tag);

	if (len < 0)
		len = strlen (markup);

	gtk_text_buffer_get_bounds (buffer, &start, &end);

	gtk_text_buffer_delete (buffer, &start, &end);

	if (len > 0)
	{
		gtk_text_buffer_get_iter_at_offset (buffer, &start, 0);
		text_buffer_insert_markup (buffer, &start, markup, len);
	}
}
// (end code to be removed)

PangoTextBuffer::PangoTextBuffer()
{
}

PangoTextBuffer::~PangoTextBuffer()
{
}

Glib::ustring PangoTextBuffer::createMarkup(Glib::RefPtr<Gtk::TextBuffer> text_buffer)
{
	Glib::ustring markup;

	Gtk::TextBuffer::iterator prev_iter = text_buffer->begin();

	for (Gtk::TextBuffer::iterator iter = text_buffer->begin(); iter != text_buffer->end(); iter++)
	{
		if (prev_iter != iter)
		{
			markup += iter.get_text (prev_iter);
			prev_iter = iter;
		}

		Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> > close_tags = iter.get_toggled_tags(false);

		typedef Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> >::iterator tags_iterator;
		for (tags_iterator tag_iter = close_tags.begin(); tag_iter != close_tags.end(); tag_iter++)
		{
			markup += "</span>";
		}

		Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> > open_tags = iter.get_toggled_tags(true);

		typedef Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> >::iterator tags_iterator;
		for (tags_iterator tag_iter = open_tags.begin(); tag_iter != open_tags.end(); tag_iter++)
		{
			markup += tagToMarkup(*tag_iter);
		}
	}

	markup += prev_iter.get_text (text_buffer->end());

	Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> > close_tags = text_buffer->end().get_toggled_tags(false);
	typedef Glib::SListHandle< Glib::RefPtr<Gtk::TextTag> >::iterator tags_iterator;
	for (tags_iterator tag_iter = close_tags.begin(); tag_iter != close_tags.end(); tag_iter++)
	{
		markup += "</span>";
	}

	return markup;
}

Glib::RefPtr<Gtk::TextBuffer> PangoTextBuffer::createTextBuffer(const Glib::ustring& markup)
{
	Glib::RefPtr<Gtk::TextBuffer> buf = Gtk::TextBuffer::create();

	text_buffer_set_markup (buf->gobj(), markup.c_str(), markup.length());

	return buf;
}

Glib::ustring PangoTextBuffer::tagToMarkup(Glib::RefPtr<Gtk::TextTag> tag)
{
	Pango::FontDescription font_desc = tag->property_font_desc();

	Glib::ustring markup;

	markup += Glib::ustring("<span ");

	Pango::FontMask font_mask = font_desc.get_set_fields();

	if (font_mask & Pango::FONT_MASK_FAMILY)
		markup += Glib::ustring::compose ("face='%1' ", font_desc.get_family());

	if (font_mask & Pango::FONT_MASK_SIZE)
		markup += Glib::ustring::compose ("size='%1' ", font_desc.get_size());

	if (font_mask & Pango::FONT_MASK_WEIGHT)
		markup += Glib::ustring::compose ("weight='%1' ", translatePangoWeight(font_desc.get_weight()));

	if (font_mask & Pango::FONT_MASK_STYLE )
		markup += Glib::ustring::compose ("style='%1' ", translatePangoStyle(font_desc.get_style()));

	if (tag->property_underline_set())
		markup += Glib::ustring::compose ("underline='%1' ", translatePangoUnderline(tag->property_underline()));

	if (tag->property_foreground_set())
		markup += Glib::ustring::compose ("foreground='%1' ", tag->property_foreground_gdk().get_value().to_string());

	if (tag->property_background_set())
		markup += Glib::ustring::compose ("background='%1' ", tag->property_background_gdk().get_value().to_string());

	markup += Glib::ustring(">");

	return markup;
}

Glib::ustring PangoTextBuffer::translatePangoWeight(Pango::Weight weight)
{
	Glib::ustring translated;

	switch (weight)
	{
	case Pango::WEIGHT_ULTRALIGHT:
		translated = "ultralight";
		break;
	case Pango::WEIGHT_LIGHT:
		translated = "light";
		break;
	case Pango::WEIGHT_NORMAL:
		translated = "normal";
		break;
	case Pango::WEIGHT_SEMIBOLD:
		translated = "semibold";
		break;
	case Pango::WEIGHT_BOLD:
		translated = "bold";
		break;
	case Pango::WEIGHT_ULTRABOLD:
		translated = "ultrabold";
		break;
	case Pango::WEIGHT_HEAVY:
		translated = "heavy";
		break;
	default:
		translated = "normal";
	}

	return translated;
}

Glib::ustring PangoTextBuffer::translatePangoStyle(Pango::Style style)
{
	Glib::ustring translated;

	switch (style)
	{
	case Pango::STYLE_NORMAL:
		translated = "normal";
		break;
	case Pango::STYLE_OBLIQUE:
		translated = "oblique";
		break;
	case Pango::STYLE_ITALIC:
		translated = "italic";
		break;
	default:
		translated = "normal";
	}

	return translated;
}

Glib::ustring PangoTextBuffer::translatePangoUnderline(Pango::Underline underline)
{
	Glib::ustring translated;

	switch (underline)
	{
	case Pango::UNDERLINE_NONE:
		translated = "none";
		break;
	case Pango::UNDERLINE_SINGLE:
		translated = "single";
		break;
	case Pango::UNDERLINE_DOUBLE:
		translated = "double";
		break;
	case Pango::UNDERLINE_LOW:
		translated = "low";
		break;
	case Pango::UNDERLINE_ERROR:
		translated = "error";
		break;
	default:
		translated = "none";
	}

	return translated;
}

}
