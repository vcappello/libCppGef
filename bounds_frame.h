#ifndef BOUNDSFRAME_H
#define BOUNDSFRAME_H

#include <i_bounds_model_element.h> // Base class: cppgef::IBoundsModelElement

using boost::dynamic_pointer_cast;

namespace cppgef
{

class BoundsFrame : public cppgef::IBoundsModelElement
{

public:
	BoundsFrame();
	virtual ~BoundsFrame();

public:
	virtual void setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds);
	virtual void setBounds(shared_ptr< Figure > figure, const Rectangle& bounds);
};

}

#endif // BOUNDSFRAME_H
