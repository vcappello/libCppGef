#ifndef VLAYOUTCONSTRAINT_H
#define VLAYOUTCONSTRAINT_H

#include <i_layout_constraint.h> // Base class: cppgef::ILayoutConstraint

namespace cppgef
{

class VLayoutConstraint : public cppgef::ILayoutConstraint
{
public:
	enum HorizontalConstraint
	{
		HorizontalShrink,
		HorizontalExpand
	};

	enum VerticalConstraint
	{
		VerticalShrink,
		VerticalFill
	};

public:
	VLayoutConstraint();
	VLayoutConstraint(HorizontalConstraint h, VerticalConstraint v, int pad_left=0, int pad_top=0, int pad_right=0, int pad_bottom=0);
	VLayoutConstraint(HorizontalConstraint h, int v, int pad_left=0, int pad_top=0, int pad_right=0, int pad_bottom=0);
	
	virtual ~VLayoutConstraint();

	HorizontalConstraint getHorizontalConstraint() const;
	int getHorizontalSize() const;
	
	VerticalConstraint getVerticalConstraint() const;
	int getVerticalSize() const;
	
	int getPadLeft() const;
	int getPadTop() const;
	int getPadRight() const;
	int getPadBottom() const;
	
protected:
	HorizontalConstraint horizontal_constraint_;
	int horizontal_size_;
	VerticalConstraint vertical_constraint_;
	int vertical_size_;
	int pad_left_;
	int pad_top_;
	int pad_right_;
	int pad_bottom_;
};

}

#endif // VLAYOUTCONSTRAINT_H
