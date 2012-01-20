#ifndef RECTANGLEFIGURE_H
#define RECTANGLEFIGURE_H

#include <rect_figure_base.h> // Base class: cppgef::RectFigureBase

namespace cppgef
{

class RectangleFigure : public cppgef::RectFigureBase
{
public:
	RectangleFigure();
	virtual ~RectangleFigure();

public:	
	/**
	 * @brief
	 * @param context
	 */
	void paint(Cairo::RefPtr<Cairo::Context> context);
	void paintWithAlpha(double alpha);
	
protected:
};

}

#endif // RECTANGLEFIGURE_H
