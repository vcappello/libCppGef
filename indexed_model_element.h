#ifndef INDEXEDMODELELEMENT_H
#define INDEXEDMODELELEMENT_H

#include <boost/shared_ptr.hpp>

#include <model_element_base.h>
#include <complex_model_element_base.h>

using boost::shared_ptr;

namespace cppgef
{

struct IndexedModelElement
{
	shared_ptr< ModelElementBase > element;
	shared_ptr< ComplexModelElementBase > parent;
	int index;
};

bool IndexedModelElementSort(IndexedModelElement a, IndexedModelElement b);

}

#endif // INDEXEDMODELELEMENT_H
