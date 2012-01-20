#ifndef RECTANGLESHAPE_H
#define RECTANGLESHAPE_H

#include <shape_base.h> // Base class: cppgef::Shape

namespace cppgef
{

class RectangleShape : public cppgef::ShapeBase
{
public:
	RectangleShape();
	virtual ~RectangleShape();

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // RECTANGLESHAPE_H
