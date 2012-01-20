#ifndef TESTSTYLE_H
#define TESTSTYLE_H

#include <i_visual_style.h> // Base class: cppgef::IVisualStyle

#include <gdkmm/color.h>
#include <pangomm/fontdescription.h>

namespace cppgef
{

class TextStyle : public cppgef::IVisualStyle
{

public:
	TextStyle();
	virtual ~TextStyle();

	double getAlpha() const;
	void setAlpha(double value);
	
	Gdk::Color getColor() const;
	void setColor(const Gdk::Color& value);
	
	Pango::FontDescription getFontDesc() const;
	void setFontDesc(const Pango::FontDescription& font_desc);
	
public:
	virtual void apply(Cairo::RefPtr<Cairo::Context> context);
	
protected:
	Gdk::Color color_;
	double alpha_;
	Pango::FontDescription font_desc_;
};

}

#endif // TESTSTYLE_H
