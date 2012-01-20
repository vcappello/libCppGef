#include "label_figure.h"

namespace cppgef
{

LabelFigure::LabelFigure() :
	cppgef::RectFigureBase()
{
	text_style_ = shared_ptr< TextStyle >(new TextStyle());
}

LabelFigure::~LabelFigure()
{
}

shared_ptr< TextStyle > LabelFigure::getTextStyle()
{
	return text_style_;
}

Glib::ustring LabelFigure::getText() const
{
	return text_;
}

void LabelFigure::setText(const Glib::ustring& value)
{
	text_ = value;
}

void LabelFigure::paint(Cairo::RefPtr<Cairo::Context> context)
{
	double x = bounds_.getLocation().getX();
	double y = bounds_.getLocation().getY();
	double width = bounds_.getSize().getWidth();
	double height = bounds_.getSize().getHeight();

	context->save();

#ifdef GTKMM_3
	context->begin_new_path();
	context->rectangle (x, y, width, height);
	context->clip();
#endif
	
	context->begin_new_path();
	
	Glib::RefPtr< Pango::Layout > pango_layout = createPangoLayout (context);
	
	context->move_to (x, y);
	
	pango_layout->update_from_cairo_context (context);  //gets cairo cursor position

	text_style_->apply (context);

	pango_layout->show_in_cairo_context (context);       //adds text to cairos stack of stuff to be drawn
	
	context->restore();	
}

void LabelFigure::paintWithAlpha(double alpha)
{
	// TODO: implemnt alpha on pango layout
	//text_style_.setAlpha (alpha);
}

Glib::RefPtr< Pango::Layout > LabelFigure::createPangoLayout(Cairo::RefPtr<Cairo::Context> context)
{
	Glib::RefPtr< Pango::Layout > pango_layout = Pango::Layout::create (context);

	Glib::ustring markup_text_to_set = Glib::ustring::compose (
		"<span font_desc='%1'>%2</span>", text_style_->getFontDesc().to_string(), text_);
	
	pango_layout->set_markup (markup_text_to_set);
	
	pango_layout->set_width (bounds_.getSize().getWidth() * Pango::SCALE);
	pango_layout->set_alignment (Pango::ALIGN_LEFT);
	
	return pango_layout;
}

void LabelFigure::applyText(const Glib::ustring& value)
{
	setText (value);
	text_applied_signal_.emit();
}

shared_ptr< IVisualStyle > LabelFigure::getVisualStyle(const Glib::ustring& id)
{
	shared_ptr< IVisualStyle > base_style = RectFigureBase::getVisualStyle(id);
	
	if (!base_style)
	{
		if (id == "text-style")
			return text_style_;
	}
	
	return shared_ptr< IVisualStyle >();
}

LabelFigure::text_applied_signal_t LabelFigure::signalTextApplied()
{
	return text_applied_signal_;
}

}

