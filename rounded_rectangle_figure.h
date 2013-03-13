#ifndef ROUNDEDRECTANGLEFIGURE_H
#define ROUNDEDRECTANGLEFIGURE_H

#include <rect_figure_base.h> // Base class: cppgef::RectFigureBase

#include <stroke_style.h>
#include <fill_style.h>

namespace cppgef
{

class RoundedRectangleFigure : public cppgef::RectFigureBase
{

public:
	RoundedRectangleFigure();
	virtual ~RoundedRectangleFigure();

public:
	void paint(Cairo::RefPtr<Cairo::Context> context);
	void paintWithAlpha(double alpha);

	double getRadius() const;
	void setRadius(double radius);
	
protected:
	double radius_;
};

}

#endif // ROUNDEDRECTANGLEFIGURE_H
