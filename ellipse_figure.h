#ifndef ELLIPSEFIGURE_H
#define ELLIPSEFIGURE_H

#include <rect_figure_base.h> // Base class: cppgef::RectFigureBase

#include <stroke_style.h>
#include <fill_style.h>

namespace cppgef
{

class EllipseFigure : public cppgef::RectFigureBase
{
public:
	EllipseFigure();
	virtual ~EllipseFigure();

public:
	void paint(Cairo::RefPtr<Cairo::Context> context);
	void paintWithAlpha(double alpha);
	
protected:
};

}

#endif // ELLIPSEFIGURE_H
