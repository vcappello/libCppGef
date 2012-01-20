#include "rectangle_shape_edit_part.h"

namespace cppgef
{

RectangleShapeEditPart::RectangleShapeEditPart()
{
}

RectangleShapeEditPart::~RectangleShapeEditPart()
{
}

shared_ptr< Figure > RectangleShapeEditPart::getFigure()
{
	return figure_;
}

void RectangleShapeEditPart::setFigure(shared_ptr< Figure > value)
{
	figure_ = dynamic_pointer_cast< RectangleFigure >(value);
	
	figure_->getBounds().signalRectangleChanged().connect (
			sigc::mem_fun(this, &RectangleShapeEditPart::onFigureBoundsChanged));
}

shared_ptr< ModelElementBase > RectangleShapeEditPart::createModelElement()
{
	return shared_ptr< RectangleShape >( new RectangleShape() );
}

shared_ptr< Figure > RectangleShapeEditPart::createFigure()
{
	shared_ptr< RectangleFigure > rectangle_figure( new RectangleFigure() );
	rectangle_figure->setBounds (shape_->getBounds());	
	rectangle_figure->getStrokeStyle()->setColor (shape_->getLineColor());
	rectangle_figure->getStrokeStyle()->setLineWidth (shape_->getLineWidth());
	rectangle_figure->getStrokeStyle()->setLineDashStyle (shape_->getLineDashStyle());
	rectangle_figure->getStrokeStyle()->setLineDashStyleOffset (shape_->getLineDashStyleOffset());
	rectangle_figure->getFillStyle()->setColor (shape_->getFillColor());
	
	return rectangle_figure;
}

shared_ptr< IInplaceEditor > RectangleShapeEditPart::queryInplaceEditor(const Point& point)
{
	return shared_ptr< IInplaceEditor >();
}

void RectangleShapeEditPart::onFigureBoundsChanged()
{
	updateResizeDragTrackerFigure();
}

}

