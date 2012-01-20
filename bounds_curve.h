#ifndef BOUNDSCURVE_H
#define BOUNDSCURVE_H

#include <i_bounds_model_element.h> // Base class: cppgef::IBoundsModelElement

using boost::dynamic_pointer_cast;

namespace cppgef
{

class BoundsCurve : public cppgef::IBoundsModelElement
{

public:
	BoundsCurve();
	virtual ~BoundsCurve();

public:
	virtual void setBounds(shared_ptr<ModelElementBase> element, const Rectangle& bounds);
	virtual void setBounds(shared_ptr<Figure> figure, const Rectangle& bounds);
	
protected:
	void computePoints(const Rectangle& bounds, Point& start_point, Point& end_point, Point& start_control_point, Point& end_control_point);
};

}

#endif // BOUNDSCURVE_H
