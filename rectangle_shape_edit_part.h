#ifndef RECTANGLESHAPEEDITPART_H
#define RECTANGLESHAPEEDITPART_H

#include <shape_edit_part.h> // Base class: cppgef::ShapeEditPart

#include <boost/shared_ptr.hpp>

#include <rectangle_shape.h>
#include <rectangle_figure.h>

using boost::shared_ptr;

namespace cppgef
{

class RectangleShapeEditPart : public cppgef::ShapeEditPart
{

public:
	RectangleShapeEditPart();
	virtual ~RectangleShapeEditPart();

	shared_ptr< Figure > getFigure();
	void setFigure(shared_ptr< Figure > value);

	shared_ptr< ModelElementBase > createModelElement();

	shared_ptr< Figure >  createFigure();

	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point);

protected:
	shared_ptr< RectangleFigure > figure_;
	
protected:
	void onFigureBoundsChanged();
};

}

#endif // RECTANGLESHAPEEDITPART_H
