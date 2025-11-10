/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "ags2/plugins/core/gui_control.h"
#include "ags2/engine/ac/gui_control.h"

namespace AGS2 {
namespace Plugins {
namespace Core {

void GUIControl::AGS_EngineStartup(IAGSEngine *engine) {
	ScriptContainer::AGS_EngineStartup(engine);

	SCRIPT_METHOD(GUIControl::BringToFront^0, GUIControl::BringToFront);
	SCRIPT_METHOD(GUIControl::GetAtScreenXY^2, GUIControl::GetGUIControlAtLocation);
	SCRIPT_METHOD(GUIControl::SendToBack^0, GUIControl::SendToBack);
	SCRIPT_METHOD(GUIControl::SetPosition^2, GUIControl::SetPosition);
	SCRIPT_METHOD(GUIControl::SetSize^2, GUIControl::SetSize);
	SCRIPT_METHOD(GUIControl::get_AsButton, GUIControl::GetAsButton);
	SCRIPT_METHOD(GUIControl::get_AsInvWindow, GUIControl::GetAsInvWindow);
	SCRIPT_METHOD(GUIControl::get_AsLabel, GUIControl::GetAsLabel);
	SCRIPT_METHOD(GUIControl::get_AsListBox, GUIControl::GetAsListBox);
	SCRIPT_METHOD(GUIControl::get_AsSlider, GUIControl::GetAsSlider);
	SCRIPT_METHOD(GUIControl::get_AsTextBox, GUIControl::GetAsTextBox);
	SCRIPT_METHOD(GUIControl::get_Clickable, GUIControl::GetClickable);
	SCRIPT_METHOD(GUIControl::set_Clickable, GUIControl::SetClickable);
	SCRIPT_METHOD(GUIControl::get_Enabled, GUIControl::GetEnabled);
	SCRIPT_METHOD(GUIControl::set_Enabled, GUIControl::SetEnabled);
	SCRIPT_METHOD(GUIControl::get_Height, GUIControl::GetHeight);
	SCRIPT_METHOD(GUIControl::set_Height, GUIControl::SetHeight);
	SCRIPT_METHOD(GUIControl::get_ID, GUIControl::GetID);
	SCRIPT_METHOD(GUIControl::get_OwningGUI, GUIControl::GetOwningGUI);
	SCRIPT_METHOD(GUIControl::get_Visible, GUIControl::GetVisible);
	SCRIPT_METHOD(GUIControl::set_Visible, GUIControl::SetVisible);
	SCRIPT_METHOD(GUIControl::get_Width, GUIControl::GetWidth);
	SCRIPT_METHOD(GUIControl::set_Width, GUIControl::SetWidth);
	SCRIPT_METHOD(GUIControl::get_X, GUIControl::GetX);
	SCRIPT_METHOD(GUIControl::set_X, GUIControl::SetX);
	SCRIPT_METHOD(GUIControl::get_Y, GUIControl::GetY);
	SCRIPT_METHOD(GUIControl::set_Y, GUIControl::SetY);
}

void GUIControl::BringToFront(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	AGS2::GUIControl_BringToFront(guio);
}

void GUIControl::GetGUIControlAtLocation(ScriptMethodParams &params) {
	PARAMS2(int, xx, int, yy);
	params._result = AGS2::GetGUIControlAtLocation(xx, yy);
}

void GUIControl::SendToBack(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	AGS2::GUIControl_SendToBack(guio);
}

void GUIControl::SetPosition(ScriptMethodParams &params) {
	PARAMS3(GUIObject *, guio, int, xx, int, yy);
	AGS2::GUIControl_SetPosition(guio, xx, yy);
}

void GUIControl::SetSize(ScriptMethodParams &params) {
	PARAMS3(GUIObject *, guio, int, newwid, int, newhit);
	AGS2::GUIControl_SetSize(guio, newwid, newhit);
}

void GUIControl::GetAsButton(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetAsButton(guio);
}

void GUIControl::GetAsInvWindow(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetAsInvWindow(guio);
}

void GUIControl::GetAsLabel(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetAsLabel(guio);
}

void GUIControl::GetAsListBox(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetAsListBox(guio);
}

void GUIControl::GetAsSlider(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetAsSlider(guio);
}

void GUIControl::GetAsTextBox(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetAsTextBox(guio);
}

void GUIControl::GetClickable(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetClickable(guio);
}

void GUIControl::SetClickable(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, enabled);
	AGS2::GUIControl_SetClickable(guio, enabled);
}

void GUIControl::GetEnabled(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetEnabled(guio);
}

void GUIControl::SetEnabled(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, enabled);
	AGS2::GUIControl_SetEnabled(guio, enabled);
}

void GUIControl::GetHeight(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetHeight(guio);
}

void GUIControl::SetHeight(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, newhit);
	AGS2::GUIControl_SetHeight(guio, newhit);
}

void GUIControl::GetID(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetID(guio);

}

void GUIControl::GetOwningGUI(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetOwningGUI(guio);
}

void GUIControl::GetVisible(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetVisible(guio);

}

void GUIControl::SetVisible(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, visible);
	AGS2::GUIControl_SetVisible(guio, visible);
}

void GUIControl::GetWidth(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetWidth(guio);
}

void GUIControl::SetWidth(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, newwid);
	AGS2::GUIControl_SetWidth(guio, newwid);
}

void GUIControl::GetX(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetX(guio);
}

void GUIControl::SetX(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, xx);
	AGS2::GUIControl_SetX(guio, xx);
}

void GUIControl::GetY(ScriptMethodParams &params) {
	PARAMS1(GUIObject *, guio);
	params._result = AGS2::GUIControl_GetY(guio);
}

void GUIControl::SetY(ScriptMethodParams &params) {
	PARAMS2(GUIObject *, guio, int, yy);
	AGS2::GUIControl_SetY(guio, yy);
}


} // namespace Core
} // namespace Plugins
} // namespace AGS2
