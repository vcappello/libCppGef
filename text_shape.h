#ifndef TEXTSHAPE_H
#define TEXTSHAPE_H

#include <shape_base.h> // Base class: cppgef::Shape
#include <i_text_element.h>

#include <glibmm/ustring.h>

namespace cppgef
{

class TextShape : public cppgef::ShapeBase, 
				  public cppgef::ITextElement
{
public:
	TextShape();
	virtual ~TextShape();

	Glib::ustring getText() const;
	void setText(const Glib::ustring& value);
	
	signal_property_changed_t signalTextChanged();
	
private:
	// Serialization
	friend class boost::serialization::access;

	template< class Archive >
	void serialize(Archive & ar, const unsigned int version);
	
protected:
	Glib::ustring text_;
	
	signal_property_changed_t signal_text_changed_;
};

}

#endif // TEXTSHAPE_H
