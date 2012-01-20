#ifndef XYLAYOUTCONSTRAINT_H
#define XYLAYOUTCONSTRAINT_H

#include <i_layout_constraint.h> // Base class: cppgef::ILayoutConstraint

#include <rectangle.h>

namespace cppgef
{

class XYLayoutConstraint : public cppgef::ILayoutConstraint
{
public:
	XYLayoutConstraint();
	XYLayoutConstraint(const Rectangle& bounds);
	virtual ~XYLayoutConstraint();

	Rectangle& getBounds();
	void setBounds(const Rectangle& value);
	
protected:
	Rectangle bounds_;
};

}

#endif // XYLAYOUTCONSTRAINT_H
