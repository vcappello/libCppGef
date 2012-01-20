#ifndef STROKESTYLE_H
#define STROKESTYLE_H

#include <i_visual_style.h> // Base class: cppgef::IVisualStyle

#include <vector>

#include <color.h>
#include <settings.h>

namespace cppgef
{

class StrokeStyle : public cppgef::IVisualStyle
{

public:
	StrokeStyle();
	virtual ~StrokeStyle();

	Color& getColor();
	void setColor(const Color& value);
	
	double getLineWidth() const;
	void setLineWidth(double value);
	
	std::vector< double > getLineDashStyle() const;
	void setLineDashStyle(const std::vector< double >& value);
	
	double getLineDashStyleOffset() const;
	void setLineDashStyleOffset(double value);
	
public:
	virtual void apply(Cairo::RefPtr<Cairo::Context> context);

protected:
	Color color_;
	double line_width_;
	std::vector< double > line_dash_style_;
	double line_dash_style_offset_;	
};

}

#endif // STROKESTYLE_H
