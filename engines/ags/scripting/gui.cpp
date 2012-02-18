/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

/* Based on the Adventure Game Studio source code, copyright 1999-2011 Chris Jones,
 * which is licensed under the Artistic License 2.0.
 */

#include "engines/ags/scripting/scripting.h"

namespace AGS {

class ScriptGUIArray : public ScriptArray {
public:
	ScriptObject *getObjectAt(uint32 &offset) {
		warning("ScriptGUIArray unimplemented"); // FIXME
		offset = offset % 8;
		return NULL;
	}
};

void addGUISystemScripting(GlobalScriptState *state) {
	// GUI functions
	state->addSystemFunctionImport("GUI::Centre^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::GetAtScreenXY^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::SetPosition^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::SetSize^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_BackgroundGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_BackgroundGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_Clickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_Clickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_ControlCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::geti_Controls", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_Height", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_Height", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_ID", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_Transparency", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_Transparency", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_Visible", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_Visible", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_Width", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_Width", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_Y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_Y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::get_ZOrder", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUI::set_ZOrder", &Script_UnimplementedStub);

	// GUIControl functions
	state->addSystemFunctionImport("GUIControl::BringToFront^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::GetAtScreenXY^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::SendToBack^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::SetPosition^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::SetSize^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_AsButton", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_AsInvWindow", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_AsLabel", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_AsListBox", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_AsSlider", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_AsTextBox", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_Clickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_Clickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_Enabled", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_Enabled", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_Height", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_Height", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_ID", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_OwningGUI", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_Visible", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_Visible", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_Width", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_Width", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_X", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::get_Y", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GUIControl::set_Y", &Script_UnimplementedStub);

	// Label functions
	state->addSystemFunctionImport("Label::GetText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::SetText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::get_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::set_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::get_Text", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::set_Text", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::get_TextColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Label::set_TextColor", &Script_UnimplementedStub);

	// Button functions
	state->addSystemFunctionImport("Button::Animate^4", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::GetText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::SetText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_ClipImage", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_ClipImage", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_Graphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_MouseOverGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_MouseOverGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_NormalGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_NormalGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_PushedGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_PushedGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_Text", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_Text", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::get_TextColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Button::set_TextColor", &Script_UnimplementedStub);

	// Slider functions
	state->addSystemFunctionImport("Slider::get_BackgroundGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::set_BackgroundGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::get_HandleGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::set_HandleGraphic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::get_HandleOffset", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::set_HandleOffset", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::get_Max", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::set_Max", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::get_Min", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::set_Min", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::get_Value", &Script_UnimplementedStub);
	state->addSystemFunctionImport("Slider::set_Value", &Script_UnimplementedStub);

	// TextBox functions
	state->addSystemFunctionImport("TextBox::GetText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::SetText^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::get_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::set_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::get_Text", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::set_Text", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::get_TextColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("TextBox::set_TextColor", &Script_UnimplementedStub);

	// InvWindow functions
	state->addSystemFunctionImport("InvWindow::ScrollDown^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::ScrollUp^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_CharacterToUse", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::set_CharacterToUse", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::geti_ItemAtIndex", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_ItemCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_ItemHeight", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::set_ItemHeight", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_ItemWidth", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::set_ItemWidth", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_ItemsPerRow", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_RowCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::get_TopItem", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InvWindow::set_TopItem", &Script_UnimplementedStub);

	// ListBox functions
	state->addSystemFunctionImport("ListBox::AddItem^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::Clear^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::FillDirList^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::FillSaveGameList^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::GetItemAtLocation^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::GetItemText^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::InsertItemAt^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::RemoveItem^1", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::ScrollDown^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::ScrollUp^0", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::SetItemText^2", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::set_Font", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_HideBorder", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::set_HideBorder", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_HideScrollArrows", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::set_HideScrollArrows", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_ItemCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::geti_Items", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::seti_Items", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_RowCount", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::geti_SaveGameSlots", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_SelectedIndex", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::set_SelectedIndex", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::get_TopItem", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBox::set_TopItem", &Script_UnimplementedStub);

	// GUI-related functions
	state->addSystemFunctionImport("AnimateButton", &Script_UnimplementedStub);
	state->addSystemFunctionImport("CentreGUI", &Script_UnimplementedStub);
	state->addSystemFunctionImport("DisableInterface", &Script_UnimplementedStub);
	state->addSystemFunctionImport("EnableInterface", &Script_UnimplementedStub);
	state->addSystemFunctionImport("FindGUIID", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetButtonPic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGUIAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetGUIObjectAt", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetSliderValue", &Script_UnimplementedStub);
	state->addSystemFunctionImport("GetTextBoxText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InterfaceOff", &Script_UnimplementedStub);
	state->addSystemFunctionImport("InterfaceOn", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsGUIOn", &Script_UnimplementedStub);
	state->addSystemFunctionImport("IsInterfaceEnabled", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxAdd", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxClear", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxDirList", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxGetItemText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxGetNumItems", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxGetSelected", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxRemove", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxSaveGameList", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxSetSelected", &Script_UnimplementedStub);
	state->addSystemFunctionImport("ListBoxSetTopItem", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetButtonPic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetButtonText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIBackgroundPic", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIClickable", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIObjectEnabled", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIObjectPosition", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIObjectSize", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIPosition", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUISize", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUITransparency", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetGUIZOrder", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetLabelColor", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetLabelFont", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetLabelText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetSliderValue", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetTextBoxFont", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetTextBoxText", &Script_UnimplementedStub);
	state->addSystemFunctionImport("SetTextWindowGUI", &Script_UnimplementedStub);

	state->addSystemObjectImport("gui", new ScriptGUIArray);
}

} // End of namespace AGS
