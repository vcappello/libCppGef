#include "xy_layout_constraint.h"

namespace cppgef
{

XYLayoutConstraint::XYLayoutConstraint()
{
}

XYLayoutConstraint::XYLayoutConstraint(const Rectangle& bounds) :
	bounds_( bounds )
{
}

XYLayoutConstraint::~XYLayoutConstraint()
{
}

Rectangle& XYLayoutConstraint::getBounds()
{
	return bounds_;
}

void XYLayoutConstraint::setBounds(const Rectangle& value)
{
	bounds_ = value;
}

}

