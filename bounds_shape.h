#ifndef BOUDSSHAPE_H
#define BOUDSSHAPE_H

#include <i_bounds_model_element.h> // Base class: cppgef::IBoundsModelElement

using boost::dynamic_pointer_cast;

namespace cppgef
{

class BoundsShape : public cppgef::IBoundsModelElement
{

public:
	BoundsShape();
	virtual ~BoundsShape();

public:
	virtual void setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds);
	virtual void setBounds(shared_ptr< Figure > figure, const Rectangle& bounds);
};

}

#endif // BOUDSSHAPE_H
