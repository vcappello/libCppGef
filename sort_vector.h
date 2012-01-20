#ifndef SORT_VECTOR_H_
#define SORT_VECTOR_H_

#include <vector>

namespace cppgef
{
	
template< class T, class Q >
struct SortItem
{
	T item;
	Q key;
};

template< class T, class Q >
bool SortKeyCriteria(SortItem< T, Q > a, SortItem< T, Q > b)
{
	return a.key < b.key;
}

template< class T, class Q >
struct SortVector
{
	std::vector< SortItem< T, Q > > items;

	void sort()
	{
		std::sort (items.begin(), items.end(), SortKeyCriteria< T, Q >);
	}
};

}

#endif // SORT_VECTOR_H_

