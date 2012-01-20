#ifndef TEXTEDITPART_H
#define TEXTEDITPART_H

#include <shape_edit_part.h> // Base class: cppgef::ShapeEditPart

#include <text_shape.h>
#include <label_figure.h>

#include <text_inplace_editor.h>

namespace cppgef
{

// TODO: rename to TextShapeEditPart
class TextEditPart : public cppgef::ShapeEditPart
{
public:
	TextEditPart();
	virtual ~TextEditPart();
	
	shared_ptr< Figure > getFigure();
	void setFigure(shared_ptr< Figure > value);

	virtual void setModel(shared_ptr< ModelElementBase > value);
	
	shared_ptr< ModelElementBase > createModelElement();
	
	shared_ptr< Figure > createFigure();

	virtual shared_ptr< IInplaceEditor > queryInplaceEditor(const Point& point);

protected:
	shared_ptr< LabelFigure > figure_;
	
	auto_ptr< SignalDisconnect > text_changed_disconnect;
	
protected:
	void onFigureBoundsChanged();
	void onLabelTextApplied();
	
	void onModelTextChanged();
};

}

#endif // TEXTEDITPART_H
