#ifndef IDIAGRAMEDITOR_H
#define IDIAGRAMEDITOR_H

#include <i_inplace_editor.h>
#include <point.h>

using boost::shared_ptr;

namespace cppgef
{

class IDiagramEditor
{
public:
	IDiagramEditor()
	{
	}
	virtual ~IDiagramEditor()
	{
	}

	virtual void activateInplaceEditor(shared_ptr< IInplaceEditor > editor, const Point& point) = 0;
	
	virtual void stopInplaceEditing() = 0;
	
	virtual void clearSelection() = 0;
	virtual void selectAll() = 0;
};

}

#endif // IDIAGRAMEDITOR_H
