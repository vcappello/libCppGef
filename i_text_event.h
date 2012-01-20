#ifndef ITEXTEVENT_H
#define ITEXTEVENT_H

namespace cppgef
{

class ITextEvent
{

public:
	ITextEvent()
	{
	}
	virtual ~ITextEvent()
	{
	}

	virtual void undo() = 0;
	virtual void redo() = 0;
};

}

#endif // ITEXTEVENT_H
