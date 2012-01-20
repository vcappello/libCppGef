#include "ellipse_shape_edit_part.h"

namespace cppgef
{

EllipseShapeEditPart::EllipseShapeEditPart()
{
}

EllipseShapeEditPart::~EllipseShapeEditPart()
{
}

shared_ptr< Figure > EllipseShapeEditPart::getFigure()
{
	return figure_;
}

void EllipseShapeEditPart::setFigure(shared_ptr< Figure > value)
{
	figure_ = dynamic_pointer_cast< EllipseFigure >(value);
	
	figure_->getBounds().signalRectangleChanged().connect (
			sigc::mem_fun(this, &EllipseShapeEditPart::onFigureBoundsChanged));
}

shared_ptr< ModelElementBase > EllipseShapeEditPart::createModelElement()
{
	return shared_ptr< EllipseShape >( new EllipseShape() );
}

shared_ptr< Figure > EllipseShapeEditPart::createFigure()
{
	shared_ptr< EllipseFigure > ellipse_figure( new EllipseFigure() );
	ellipse_figure->setBounds (shape_->getBounds());
	ellipse_figure->getStrokeStyle()->setColor (shape_->getLineColor());
	ellipse_figure->getStrokeStyle()->setLineWidth (shape_->getLineWidth());
	ellipse_figure->getStrokeStyle()->setLineDashStyle (shape_->getLineDashStyle());
	ellipse_figure->getStrokeStyle()->setLineDashStyleOffset (shape_->getLineDashStyleOffset());
	ellipse_figure->getFillStyle()->setColor (shape_->getFillColor());
	
	return ellipse_figure;
}

shared_ptr< IInplaceEditor > EllipseShapeEditPart::queryInplaceEditor(const Point& point)
{
	return shared_ptr< IInplaceEditor >();
}

void EllipseShapeEditPart::onFigureBoundsChanged()
{
	updateResizeDragTrackerFigure();
}

}

