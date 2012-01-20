/*
 * RectFigureBase.h
 *
 *  Created on: 03/mar/2011
 *      Author: vincenzo
 */

#ifndef RECTFIGUREBASE_H_
#define RECTFIGUREBASE_H_

#include <figure.h>

#include <stroke_style.h>
#include <fill_style.h>

namespace cppgef
{

class RectFigureBase : public Figure
{
public:
	RectFigureBase();
	virtual ~RectFigureBase();

	Rectangle& getBounds();
	void setBounds(const Rectangle& value);

	virtual Rectangle getOutlineRect() const;

	shared_ptr< StrokeStyle > getStrokeStyle();
	shared_ptr< FillStyle > getFillStyle();

	virtual shared_ptr< IVisualStyle > getVisualStyle(const Glib::ustring& id);

	virtual void fitRect(const Rectangle& rect);

	virtual bool isPointIn(const Point& pt);
	virtual bool isBoundsOut(const Rectangle& rect);
	
protected:
	Rectangle bounds_;
	shared_ptr< StrokeStyle > stroke_style_;
	shared_ptr< FillStyle > fill_style_;

protected:
	virtual void onBoundsRectangleChanged();
};

}

#endif /* RECTFIGUREBASE_H_ */
