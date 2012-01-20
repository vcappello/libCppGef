#ifndef BOUNDSLINE_H
#define BOUNDSLINE_H

#include <i_bounds_model_element.h> // Base class: cppgef::IBoundsModelElement

using boost::dynamic_pointer_cast;

namespace cppgef
{

class BoundsLine : public cppgef::IBoundsModelElement
{

public:
	BoundsLine();
	virtual ~BoundsLine();

public:
	virtual void setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds);
	virtual void setBounds(shared_ptr< Figure > figure, const Rectangle& bounds);
};

}

#endif // BOUNDSLINE_H
