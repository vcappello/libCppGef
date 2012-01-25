#ifndef ICONTAINEREDITPART_H
#define ICONTAINEREDITPART_H

#include <i_edit_part.h> // Base class: cppgef::IEditPart

#include <boost/shared_ptr.hpp>
#include <vector>

using boost::shared_ptr;

namespace cppgef
{

class IContainerEditPart : public cppgef::IEditPart
{
public:
	typedef std::vector< shared_ptr< IEditPart > > children_container_t;
	typedef children_container_t::iterator iterator;
	typedef children_container_t::const_iterator const_iterator;
	typedef children_container_t::reverse_iterator reverse_iterator;
	typedef children_container_t::const_reverse_iterator const_reverse_iterator;
	typedef children_container_t::size_type size_type;
	
public:
	IContainerEditPart() {}
	virtual ~IContainerEditPart() {}

	virtual shared_ptr< Figure > getClientFigure() = 0;

	virtual void clearSelection() = 0;
	virtual void selectAll() = 0;
	
	virtual shared_ptr< IEditPart > getChildAt(int index) = 0;
	
	virtual iterator getChildrenBegin() = 0;
	virtual const_iterator getChildrenBegin() const = 0;
	virtual iterator getChildrenEnd() = 0;
	virtual const_iterator getChildrenEnd() const = 0;
	virtual size_type getChildrenSize() const = 0;
	
	virtual iterator getSelectedChildrenBegin() = 0;
	virtual const_iterator getSelectedChildrenBegin() const = 0;
	virtual iterator getSelectedChildrenEnd() = 0;
	virtual const_iterator getSelectedChildrenEnd() const = 0;
	virtual size_type getSelectedChildrenSize() const = 0;
	
	virtual bool hasLayoutManager() const = 0;
	virtual void setLayoutConstraint(shared_ptr< ModelElementBase > element, const Rectangle& bounds) = 0;
	
	virtual shared_ptr< ICommand > createInsertModelElementCommand(shared_ptr< ModelElementBase > element) = 0;
};

}

#endif // ICONTAINEREDITPART_H
