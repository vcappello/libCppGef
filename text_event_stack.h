#ifndef TEXTEVENTSTACK_H
#define TEXTEVENTSTACK_H

#include <i_text_event.h>

#include <stack>
#include <boost/shared_ptr.hpp>

using boost::shared_ptr;

namespace cppgef
{

class TextEventStack
{
public:
	TextEventStack();
	virtual ~TextEventStack();
	
	bool isWorking() const;
	
	void storeEvent(shared_ptr< ITextEvent > event);
	
	bool undo();
	bool redo();
	
protected:
	std::stack< shared_ptr< ITextEvent > > undo_stack_;
	std::stack< shared_ptr< ITextEvent > > redo_stack_;
	bool working_;
};

}

#endif // TEXTEVENTSTACK_H
