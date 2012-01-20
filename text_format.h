#ifndef TEXTFORMAT_H
#define TEXTFORMAT_H

#include <glibmm/ustring.h>

namespace cppgef
{

class TextFormat
{
public:
	TextFormat();
	virtual ~TextFormat();

	bool isBold() const;
	void setBold(bool value);
	
	bool isItalic() const;
	void setItalic(bool value);
	
	bool isUnderline() const;
	void setUnderline(bool value);
	
	Glib::ustring getFontFamily() const;
	void setFontFamily(const Glib::ustring& value);
	
	int getFontSize() const;
	void setFontSize(const int value);
	
	bool operator== (const TextFormat& rhs);
	bool operator!= (const TextFormat& rhs);
	
protected:
	bool bold_;
	bool italic_;
	bool underline_;
	Glib::ustring font_family_;
	int font_size_;
};

}

#endif // TEXTFORMAT_H
