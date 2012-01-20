#ifndef DRAGTRACKERFIGURE_H
#define DRAGTRACKERFIGURE_H

#include <rect_figure_base.h> // Base class: cppgef::RectFigureBase

#include <stroke_style.h>
#include <fill_style.h>

namespace cppgef
{

class DragTrackerFigure : public cppgef::RectFigureBase
{

public:
	DragTrackerFigure();
	virtual ~DragTrackerFigure();

public:
	void paint(Cairo::RefPtr<Cairo::Context> context);
	void paintWithAlpha(double alpha);

protected:
};

}

#endif // DRAGTRACKERFIGURE_H
