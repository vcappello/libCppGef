#include "text_format.h"

namespace cppgef
{

TextFormat::TextFormat() :
	bold_( false ),
	italic_( false ),
	underline_( false )
{
}

TextFormat::~TextFormat()
{
}

bool TextFormat::isBold() const
{
	return bold_;
}

void TextFormat::setBold(bool value)
{
	this->bold_ = value;
}

bool TextFormat::isItalic() const
{
	return italic_;
}

void TextFormat::setItalic(bool value)
{
	this->italic_ = value;
}

bool TextFormat::isUnderline() const
{
	return underline_;
}

void TextFormat::setUnderline(bool value)
{
	this->underline_ = value;
}

Glib::ustring TextFormat::getFontFamily() const
{
	return this->font_family_;
}

void TextFormat::setFontFamily(const Glib::ustring& value)
{
	this->font_family_ = value;
}

int TextFormat::getFontSize() const
{
	return this->font_size_;
}

void TextFormat::setFontSize(const int value)
{
	this->font_size_ = value;
}

bool TextFormat::operator== (const TextFormat& rhs)
{
	return bold_ == rhs.bold_ &&
		italic_ == rhs.italic_ &&
		underline_ == rhs.underline_ &&
		font_family_ == rhs.font_family_ &&
		font_size_ == rhs.font_size_;
}

bool TextFormat::operator!= (const TextFormat& rhs)
{
	return !(*this == rhs);
}

}

