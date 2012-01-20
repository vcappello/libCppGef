#ifndef ELLIPSESHAPEEDITPART_H
#define ELLIPSESHAPEEDITPART_H

#include <shape_edit_part.h> // Base class: cppgef::ShapeEditPart

#include <boost/shared_ptr.hpp>

#include <ellipse_figure.h>
#include <ellipse_shape.h>

using boost::shared_ptr;

namespace cppgef
{

class EllipseShapeEditPart : public cppgef::ShapeEditPart
{

public:
	EllipseShapeEditPart();
	virtual ~EllipseShapeEditPart();

	shared_ptr< Figure > getFigure();
	void setFigure(shared_ptr< Figure > value);

	shared_ptr< ModelElementBase > createModelElement();
	
	shared_ptr< Figure > createFigure();

	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point);
	
protected:
	shared_ptr< EllipseFigure > figure_;
	
protected:
	void onFigureBoundsChanged();
};

}

#endif // ELLIPSESHAPEEDITPART_H
