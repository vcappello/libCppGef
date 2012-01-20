#ifndef LABELFIGURE_H
#define LABELFIGURE_H

#include <rect_figure_base.h> // Base class: cppgef::RectFigureBase

#include <gdkmm/color.h>
#include <glibmm/ustring.h>
#include <pangomm/layout.h>

#include <text_style.h>

namespace cppgef
{

class LabelFigure : public cppgef::RectFigureBase
{
public:
	typedef sigc::signal< void > text_applied_signal_t;
	
public:
	LabelFigure();
	virtual ~LabelFigure();

	shared_ptr< TextStyle > getTextStyle();
	
	Glib::ustring getText() const;
	void setText(const Glib::ustring& value);

	void applyText(const Glib::ustring& value);
	
	virtual shared_ptr< IVisualStyle > getVisualStyle(const Glib::ustring& id);
	
	text_applied_signal_t signalTextApplied();
	
public:
	void paint(Cairo::RefPtr<Cairo::Context> context);
	void paintWithAlpha(double alpha);

protected:
	shared_ptr< TextStyle > text_style_;
	Glib::ustring text_;
	text_applied_signal_t text_applied_signal_;
	
protected:
	Glib::RefPtr< Pango::Layout > createPangoLayout(Cairo::RefPtr<Cairo::Context> context);
};

}

#endif // LABELFIGURE_H
