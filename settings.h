#ifndef SETTINGS_H
#define SETTINGS_H

#include <color.h>

namespace cppgef
{

class Settings
{
private:
	static Settings* instance_;

public:
	/**
	 * @brief The singleton instance
	 * @return the singleton instance
	 */
	static Settings* getInstance();
	
	Color getLineColor() const;
	void setLineColor(const Color& value);
	
	double getLineWidth() const;
	void setLineWidth(double value);
	
	std::vector< double > getLineDashStyle() const;
	void setLineDashStyle(const std::vector< double >& value);
	
	double getLineDashStyleOffset() const;
	void setLineDashStyleOffset(double value);
	
	Color getFillColor() const;
	void setFillColor(const Color& value);
	
	Color getTextForegroundColor() const;
	void setTextForegroundColor(const Color& value);
	
	Color getTextBackgroundColor() const;
	void setTextBackgroundColor(const Color& value);
	
	bool getTextHasBackgroundColor() const;
	void setTextHasBackgroundColor(bool value);
	
protected:	
	Color line_color_;
	double line_width_;
	std::vector< double > line_dash_style_;
	double line_dash_style_offset_;
	Color fill_color_;
	Color text_foreground_color_;
	Color text_background_color_;
	bool text_has_background_color_;

private:
	Settings();
	virtual ~Settings();
};

}

#endif // SETTINGS_H
