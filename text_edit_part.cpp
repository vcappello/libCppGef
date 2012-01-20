#include "text_edit_part.h"

#include <diagram_edit_part.h>
#include <command_manager.h>
#include <edit_text_command.h>

namespace cppgef
{

TextEditPart::TextEditPart()
{
}

TextEditPart::~TextEditPart()
{
}

shared_ptr< Figure > TextEditPart::getFigure()
{
	return figure_;
}

void TextEditPart::setFigure(shared_ptr< Figure > value)
{
	figure_ = dynamic_pointer_cast< LabelFigure >(value);
	
	figure_->getBounds().signalRectangleChanged().connect (
			sigc::mem_fun(this, &TextEditPart::onFigureBoundsChanged));
			
	figure_->signalTextApplied().connect (
			sigc::mem_fun(this, &TextEditPart::onLabelTextApplied));
}

void TextEditPart::setModel(shared_ptr< ModelElementBase > value)
{
	ShapeEditPart::setModel (value);
	
	shared_ptr< TextShape > text_shape = dynamic_pointer_cast< TextShape >(getModel());
	
	text_changed_disconnect = auto_ptr< SignalDisconnect >( new SignalDisconnect( 
		text_shape->signalTextChanged().connect (
			sigc::mem_fun(this, &TextEditPart::onModelTextChanged)) ) );	
}

shared_ptr< ModelElementBase > TextEditPart::createModelElement()
{
	return shared_ptr< TextShape >( new TextShape() );
}

shared_ptr< Figure > TextEditPart::createFigure()
{
	shared_ptr< TextShape > text_shape = dynamic_pointer_cast< TextShape >(shape_);
	
	shared_ptr< LabelFigure > label_figure( new LabelFigure() );
	label_figure->getTextStyle()->setColor (Gdk::Color("black"));
	
	label_figure->setBounds (text_shape->getBounds());
	label_figure->setText (text_shape->getText());

	return label_figure;
}

shared_ptr< IInplaceEditor > TextEditPart::queryInplaceEditor(const Point& point)
{
	if (selected_ && getFigure()->getOutlineRect().contains (point))
	{
		shared_ptr< LabelFigure > label_figure = dynamic_pointer_cast< LabelFigure >(getFigure());
		
		return shared_ptr< IInplaceEditor >( 
			new TextInplaceEditor( label_figure ) );
	}
	
	return shared_ptr< IInplaceEditor >();
}

void TextEditPart::onFigureBoundsChanged()
{
	updateResizeDragTrackerFigure();
}

void TextEditPart::onLabelTextApplied()
{
	shared_ptr< LabelFigure > label_figure = dynamic_pointer_cast< LabelFigure >(getFigure());
	shared_ptr< TextShape > text_shape = dynamic_pointer_cast< TextShape >(getModel());
	
	shared_ptr< DiagramEditPart > diagram_edit_part = DiagramEditPart::queryDiagramFromChild (shared_from_this());
	
	if (diagram_edit_part)
	{
		shared_ptr< EditTextCommand > cmd( new EditTextCommand(
			dynamic_pointer_cast< Diagram >(diagram_edit_part->getModel()),
			text_shape,
			label_figure->getText()) );
		
		CommandManager::getInstance()->execute (cmd);
	}
}

void TextEditPart::onModelTextChanged()
{
	shared_ptr< LabelFigure > label_figure = dynamic_pointer_cast< LabelFigure >(getFigure());
	shared_ptr< TextShape > text_shape = dynamic_pointer_cast< TextShape >(getModel());

	label_figure->setText (text_shape->getText());
}

}

