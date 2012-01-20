#ifndef IBOUNDSMODELELEMENT_H
#define IBOUNDSMODELELEMENT_H

#include <boost/shared_ptr.hpp>

#include <model_element_base.h>
#include <figure.h>
#include <rectangle.h>

using boost::shared_ptr;

namespace cppgef
{

class IBoundsModelElement
{

public:
	IBoundsModelElement() {}
	virtual ~IBoundsModelElement() {}

	virtual void setBounds(shared_ptr< ModelElementBase > element, const Rectangle& bounds) = 0;
	virtual void setBounds(shared_ptr< Figure > figure, const Rectangle& bounds) = 0;
};

}

#endif // IBOUNDSMODELELEMENT_H
