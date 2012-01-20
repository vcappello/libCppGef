/*
 * clipboard_manager.h
 *
 *  Created on: Oct 26, 2011
 *      Author: vincenzo
 */

#ifndef CLIPBOARD_MANAGER_H_
#define CLIPBOARD_MANAGER_H_

#include <gtkmm.h>

#include <string>

namespace cppgef
{

class ClipboardManager
{
private:
	static ClipboardManager* instance_;

public:
	/**
	 * @brief The singleton instance
	 * @return the singleton instance
	 */
	static ClipboardManager* getInstance();

	std::string getClipboardTarget() const;

	void setCopyData(const std::string& data);
	void copy();
	void paste(const Gtk::Clipboard::SlotReceived& slot);

protected:
	std::string data_store_;
	std::string data_;

protected:
	void onClipboardGet(Gtk::SelectionData& selection_data, guint);
	void onClipboardClear();

private:
	ClipboardManager();
	virtual ~ClipboardManager();
};

} /* namespace cppgef */
#endif /* CLIPBOARD_MANAGER_H_ */
