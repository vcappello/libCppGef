#ifndef GROUPFIGURE_H
#define GROUPFIGURE_H

#include <rect_figure_base.h> // Base class: cppgef::RectFigureBase

#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace cppgef
{

class GroupFigure : public cppgef::RectFigureBase
{

public:
	GroupFigure();
	virtual ~GroupFigure();

	shared_ptr< RectFigureBase > getBackgroundFigure();
	void setBackgroundFigure(shared_ptr< RectFigureBase > value);
	
public:
	void paint(Cairo::RefPtr<Cairo::Context> context);
	void paintWithAlpha(double alpha);
	
protected:
	shared_ptr< RectFigureBase > background_figure_;
	
protected:
	virtual void onBoundsRectangleChanged();
};

}

#endif // GROUPFIGURE_H
