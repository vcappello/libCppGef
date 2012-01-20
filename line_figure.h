#ifndef SIMPLELINEFIGURE_H
#define SIMPLELINEFIGURE_H

#include <figure.h> // Base class: cppgef::Figure

#include <stroke_style.h>

namespace cppgef
{

class LineFigure : public cppgef::Figure
{

public:
	LineFigure();
	virtual ~LineFigure();

	Point& getStartPoint();
	void setStartPoint(const Point& value);
	
	Point& getEndPoint();
	void setEndPoint(const Point& value);
	
	shared_ptr< StrokeStyle > getStrokeStyle();
	
	virtual shared_ptr< IVisualStyle > getVisualStyle(const Glib::ustring& id);
	
	virtual Rectangle getOutlineRect() const;

	virtual void fitRect(const Rectangle& rect);

	virtual bool isPointIn(const Point& pt);
	virtual bool isBoundsOut(const Rectangle& rect);
	
public:
	virtual void paint(Cairo::RefPtr<Cairo::Context> context);
	virtual void paintWithAlpha(double alpha);
	
protected:
	Point start_point_;
	Point end_point_;
	shared_ptr< StrokeStyle > stroke_style_;

protected:
	void onStartPointChanged();
	void onEndPointChanged();
};

}

#endif // SIMPLELINEFIGURE_H
