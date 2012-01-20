#ifndef ITEXTELEMENT_H
#define ITEXTELEMENT_H

#include <glibmm/ustring.h>

namespace cppgef
{

class ITextElement
{

public:
	ITextElement()
	{
	}
	virtual ~ITextElement()
	{
	}

	virtual Glib::ustring getText() const = 0;
	virtual void setText(const Glib::ustring& value) = 0;
};

}

#endif // ITEXTELEMENT_H
