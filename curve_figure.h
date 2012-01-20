#ifndef CURVEFIGURE_H
#define CURVEFIGURE_H

#include <figure.h> // Base class: cppgef::Figure

#include <stroke_style.h>

namespace cppgef
{

class CurveFigure : public cppgef::Figure
{

public:
	CurveFigure();
	virtual ~CurveFigure();

	Point& getStartPoint();
	void setStartPoint(const Point& value);
	
	Point& getEndPoint();
	void setEndPoint(const Point& value);

	Point& getStartControlPoint();
	void setStartControlPoint(const Point& value);
	
	Point& getEndControlPoint();
	void setEndControlPoint(const Point& value);
	
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
	Point start_control_point_;
	Point end_control_point_;
	
	shared_ptr< StrokeStyle > stroke_style_;
	
	mutable vector< Point > points_;
	mutable Rectangle bounds_points_;
	mutable bool points_validated_;
	
protected:
	void onPointChanged();
	
	Point relPointProportion(const Point& rel_point, const Point& source_point, const Point& new_rel_point, double fx, double fy);
};

}

#endif // CURVEFIGURE_H
