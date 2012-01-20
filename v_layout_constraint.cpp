#include "v_layout_constraint.h"

namespace cppgef
{

VLayoutConstraint::VLayoutConstraint() :
	horizontal_constraint_( VLayoutConstraint::HorizontalExpand ), 
	horizontal_size_( 0 ),
	vertical_constraint_( VLayoutConstraint::VerticalFill ),
	vertical_size_( 0 ),
	pad_left_( 0 ),
	pad_top_( 0 ),
	pad_right_( 0 ),
	pad_bottom_( 0 )	
{
}

VLayoutConstraint::VLayoutConstraint(HorizontalConstraint h, VerticalConstraint v, int pad_left, int pad_top, int pad_right, int pad_bottom) :
	horizontal_constraint_( h ), 
	horizontal_size_( 0 ),
	vertical_constraint_( v ),
	vertical_size_( 0 ),
	pad_left_( pad_left ),
	pad_top_( pad_top ),
	pad_right_( pad_right ),
	pad_bottom_( pad_bottom )
{
}

VLayoutConstraint::VLayoutConstraint(HorizontalConstraint h, int v, int pad_left, int pad_top, int pad_right, int pad_bottom) :
	horizontal_constraint_( h ), 
	horizontal_size_( 0 ),
	vertical_constraint_( VLayoutConstraint::VerticalShrink ),
	vertical_size_( v ),
	pad_left_( pad_left ),
	pad_top_( pad_top ),
	pad_right_( pad_right ),
	pad_bottom_( pad_bottom )
{
}
	
VLayoutConstraint::~VLayoutConstraint()
{
}

VLayoutConstraint::HorizontalConstraint VLayoutConstraint::getHorizontalConstraint() const
{
	return horizontal_constraint_;
}

int VLayoutConstraint::getHorizontalSize() const
{
	return horizontal_size_;
}

VLayoutConstraint::VerticalConstraint VLayoutConstraint::getVerticalConstraint() const
{
	return vertical_constraint_;
}

int VLayoutConstraint::getVerticalSize() const
{
	return vertical_size_;
}

int VLayoutConstraint::getPadLeft() const
{
	return pad_left_;
}

int VLayoutConstraint::getPadTop() const
{
	return pad_top_;
}

int VLayoutConstraint::getPadRight() const
{
	return pad_right_;
}

int VLayoutConstraint::getPadBottom() const
{
	return pad_bottom_;
}

}


