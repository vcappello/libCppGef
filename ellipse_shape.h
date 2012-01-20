#ifndef ELLIPSESHAPE_H
#define ELLIPSESHAPE_H

#include <shape_base.h> // Base class: cppgef::Shape

namespace cppgef
{

class EllipseShape : public cppgef::ShapeBase
{

public:
	EllipseShape();
	virtual ~EllipseShape();

private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
};

}

#endif // ELLIPSESHAPE_H
