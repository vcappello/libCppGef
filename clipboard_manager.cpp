/*
 * clipboard_manager.cpp
 *
 *  Created on: Oct 26, 2011
 *      Author: vincenzo
 */

#include "clipboard_manager.h"

const std::string CLIPBOARD_TARGET = "libcppgef";

namespace cppgef
{

ClipboardManager* ClipboardManager::instance_ = 0;

ClipboardManager::ClipboardManager()
{
}

ClipboardManager::~ClipboardManager()
{
}

ClipboardManager* ClipboardManager::getInstance()
{
	if(instance_ == 0)
	{
		instance_ = new ClipboardManager();
	}
	return instance_;
}

std::string ClipboardManager::getClipboardTarget() const
{
	return CLIPBOARD_TARGET;
}

void ClipboardManager::setCopyData(const std::string& data)
{
	data_store_ = data;
}

void ClipboardManager::copy()
{
	//Targets:
	std::vector<Gtk::TargetEntry> listTargets;

	listTargets.push_back( Gtk::TargetEntry(CLIPBOARD_TARGET) );
	//listTargets.push_back( Gtk::TargetEntry(example_target_text) );

	//Tell the clipboard to call our method when it is ready:
	Glib::RefPtr< Gtk::Clipboard > clipboard = Gtk::Clipboard::get();

	clipboard->set (listTargets,
		sigc::mem_fun(*this, &ClipboardManager::onClipboardGet),
		sigc::mem_fun(*this, &ClipboardManager::onClipboardClear) );

	// Store the copied data until it is pasted:
	// (Must be done after the call to refClipboard->set, because that call
	// may trigger a call to on_clipboard_clear.)
	data_ = data_store_;
}

void ClipboardManager::paste(const Gtk::Clipboard::SlotReceived& slot)
{
	//Tell the clipboard to call our method when it is ready:
	Glib::RefPtr<Gtk::Clipboard> clipboard = Gtk::Clipboard::get();

	clipboard->request_contents (CLIPBOARD_TARGET, slot );
}

void ClipboardManager::onClipboardGet(Gtk::SelectionData& selection_data, guint)
{
	//info is meant to indicate the target, but it seems to be always 0,
	//so we use the selection_data's target instead.
	const std::string target = selection_data.get_target();

	if(target == CLIPBOARD_TARGET)
	{
		// This set() override uses an 8-bit text format for the data.
		selection_data.set (CLIPBOARD_TARGET, data_);
	}
}

void ClipboardManager::onClipboardClear()
{
	data_.clear();
}

} /* namespace cppgef */
