#ifndef FIGURE_H
#define FIGURE_H

#include <cairomm/context.h>
#include <glibmm/ustring.h>

#include <vector>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <rectangle.h>
#include <i_layout_manager.h>
#include <i_layout_constraint.h>
#include <i_visual_style.h>

using std::vector;
using boost::shared_ptr;
using boost::weak_ptr;

namespace cppgef
{

class Figure : public boost::enable_shared_from_this< Figure >
{
public:
	typedef shared_ptr< Figure > item_t;
	typedef vector< item_t > children_t;
	typedef children_t::iterator iterator;
	typedef children_t::const_iterator const_iterator;
	typedef children_t::reverse_iterator reverse_iterator;
	typedef children_t::const_reverse_iterator const_reverse_iterator;
	typedef children_t::size_type size_t;
	
public:
	Figure();
	virtual ~Figure();

	weak_ptr< Figure > getParent();
	
	shared_ptr< ILayoutManager > getLayoutManager();
	void setLayoutManager(shared_ptr< ILayoutManager > value);
	
	virtual void addChild(shared_ptr< Figure > child);
	virtual void addChild(shared_ptr< Figure > child, shared_ptr< ILayoutConstraint > constraint);
	
	virtual void insertChild(shared_ptr< Figure > child, size_t pos);
	virtual void insertChild(shared_ptr< Figure > child, shared_ptr< ILayoutConstraint > constraint, size_t pos);

	virtual void eraseChild(shared_ptr< Figure > child);
	
	shared_ptr< Figure > getChild(int index);
	int getChildIndex(shared_ptr< Figure > figure);
	
	virtual shared_ptr< IVisualStyle > getVisualStyle(const Glib::ustring& id);
	
	virtual Rectangle getOutlineRect() const = 0;

	virtual void fitRect(const Rectangle& rect) = 0;

	virtual bool isPointIn(const Point& pt) = 0;
	virtual bool isBoundsOut(const Rectangle& rect) = 0;
	
	virtual void paint(Cairo::RefPtr< Cairo::Context > context) = 0;

	virtual void paintWithAlpha(double alpha) = 0;
	
	virtual void paintChildren(Cairo::RefPtr< Cairo::Context > context);

protected:
	weak_ptr< Figure > parent_;
	shared_ptr< ILayoutManager > layout_manager_;
	children_t children_;
};

}

#endif // FIGURE_H
