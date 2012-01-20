#ifndef FILLSTYLE_H
#define FILLSTYLE_H

#include <i_visual_style.h> // Base class: cppgef::IVisualStyle

#include <color.h>
#include <settings.h>

namespace cppgef
{

class FillStyle : public cppgef::IVisualStyle
{

public:
	FillStyle();
	virtual ~FillStyle();

	Color& getColor();
	void setColor(const Color& value);

public:
	virtual void apply(Cairo::RefPtr<Cairo::Context> context);
	
protected:
	Color color_;
};

}

#endif // FILLSTYLE_H
