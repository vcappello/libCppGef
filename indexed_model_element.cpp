#include "indexed_model_element.h"

namespace cppgef
{

bool IndexedModelElementSort(IndexedModelElement a, IndexedModelElement b)
{
	return a.index < b.index;
}


}

