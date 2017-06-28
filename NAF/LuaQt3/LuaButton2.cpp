/*
 * Copyright 2000-2015 Rochus Keller <mailto:rkeller@nmr.ch>
 *
 * This file is part of the CARA (Computer Aided Resonance Assignment,
 * see <http://cara.nmr.ch/>) NMR Application Framework (NAF) library.
 *
 * The following is the license that applies to this copy of the
 * library. For a license to use the library under conditions
 * other than those described here, please email to rkeller@nmr.ch.
 *
 * GNU General Public License Usage
 * This file may be used under the terms of the GNU General Public
 * License (GPL) versions 2.0 or 3.0 as published by the Free Software
 * Foundation and appearing in the file LICENSE.GPL included in
 * the packaging of this file. Please review the following information
 * to ensure GNU General Public Licensing requirements will be met:
 * http://www.fsf.org/licensing/licenses/info/GPLv2.html and
 * http://www.gnu.org/copyleft/gpl.html.
 */

#include "LuaButton2.h"
#include <Q3Accel>
#include <QMenu>
#include <Qtl2/Variant.h>
#include <Script2/QtValue.h>
using namespace Lua;
using namespace Qtl;

static const luaL_reg LuaButton2_methods[] =
{
	{"isOn",  LuaButton2::isOn },
	{"isToggleButton",  LuaButton2::isToggleButton },
	{"setAccel",  LuaButton2::setAccel },
	{"setIcon",  LuaButton2::setIcon },
	{"getText",  LuaButton2::getText },
	{"setText",  LuaButton2::setText },
	{"setChecked",  LuaButton2::setChecked },
	{"isChecked",  LuaButton2::isChecked },
	{0,0}
};

int LuaButton2::setText(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	obj->setText( luaL_checkstring( L, 2 ) );
	return 0;
}

int LuaButton2::getText(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	lua_pushstring( L, obj->text() );
	return 1;
}

int LuaButton2::setIcon(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	QPixmap* img = QtValue<QPixmap>::check( L, 2 );
	obj->setPixmap( *img );
	return 0;
}

int LuaButton2::setAccel(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	obj->setAccel( Q3Accel::stringToKey( luaL_checkstring( L, 2 ) ) );
	return 0;
}

int LuaButton2::isToggleButton(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	lua_pushboolean( L, obj->isToggleButton() );
	return 1;
}

int LuaButton2::isOn(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	lua_pushboolean( L, obj->isOn() );
	return 1;
}

int LuaButton2::isChecked(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	lua_pushboolean( L, obj->isChecked() );
	return 1;
}

int LuaButton2::setChecked(lua_State *L)
{
	QAbstractButton* obj = QtObject<QAbstractButton>::check( L, 1 );
	bool b = true;
	if( lua_gettop(L) > 0 && lua_isboolean( L, 2 ) )
		b = lua_toboolean( L, 2 );
	obj->setChecked( b );
	return 0;
}

static const luaL_reg LuaPushButton2_methods[] =
{
	{"setOn",  LuaPushButton2::setOn },
	{"setFlat",  LuaPushButton2::setFlat },
	{"setPopup",  LuaPushButton2::setPopup },
	{"setDefault",  LuaPushButton2::setDefault },
	{"setToggleButton",  LuaPushButton2::setToggleButton },
	{0,0}
};

void LuaPushButton2::install( lua_State *L )
{
	// Sollte bereits erfolgt sein: QtObject<LuaPushButton2>::install( L );
	QtObject<QPushButton>::addMethods( L, LuaButton2_methods );
	QtObject<QPushButton>::addMethods( L, LuaPushButton2_methods );
}

void LuaPushButton2::doClicked(bool b)
{
    if( LuaWidgetCallbacks::pushCallbackAndMe( this, LuaWidget2::Clicked ) )
	{
        lua_pushboolean( LuaWidgetCallbacks::getLua(), b );
        LuaWidgetCallbacks::call( 2, 0 );
	}
}

void LuaPushButton2::doToggled(bool b)
{
    if( LuaWidgetCallbacks::pushCallbackAndMe( this, LuaWidget2::Toggled ) )
	{
        lua_pushboolean( LuaWidgetCallbacks::getLua(), b );
        LuaWidgetCallbacks::call( 2, 0 );
	}
}

int LuaPushButton2::setToggleButton(lua_State *L)
{
	QPushButton* obj = QtObject<QPushButton>::check( L, 1 );
	bool b = true;
	if( lua_gettop(L) > 0 && lua_isboolean( L, 2 ) )
		b = lua_toboolean( L, 2 );
	obj->setToggleButton( b );
	return 0;
}

int LuaPushButton2::setDefault(lua_State *L)
{
	QPushButton* obj = QtObject<QPushButton>::check( L, 1 );
	bool b = true;
	if( lua_gettop(L) > 0 && lua_isboolean( L, 2 ) )
		b = lua_toboolean( L, 2 );
	obj->setDefault( b );
	return 0;
}

int LuaPushButton2::setPopup(lua_State *L)
{
	QPushButton* obj = QtObject<QPushButton>::check( L, 1 );
	QMenu* menu = QtObject<QMenu>::check( L, 2 );
	obj->setPopup( menu );
	return 0;
}

int LuaPushButton2::setFlat(lua_State *L)
{
	QPushButton* obj = QtObject<QPushButton>::check( L, 1 );
	bool b = true;
	if( lua_gettop(L) > 0 && lua_isboolean( L, 2 ) )
		b = lua_toboolean( L, 2 );
	obj->setFlat( b );
	return 0;
}

int LuaPushButton2::setOn(lua_State *L)
{
	QPushButton* obj = QtObject<QPushButton>::check( L, 1 );
	bool b = true;
	if( lua_gettop(L) > 0 && lua_isboolean( L, 2 ) )
		b = lua_toboolean( L, 2 );
	obj->setOn( b );
	return 0;
}

void LuaCheckBox2::install( lua_State *L )
{
	QtObject<QCheckBox>::addMethods( L, LuaButton2_methods );
}

void LuaRadioButton2::install( lua_State *L )
{
	QtObject<QRadioButton>::addMethods( L, LuaButton2_methods );
}

void LuaCheckBox2::doClicked(bool b)
{
    if( LuaWidgetCallbacks::pushCallbackAndMe( this, LuaWidget2::Clicked ) )
	{
        lua_pushboolean( LuaWidgetCallbacks::getLua(), b );
        LuaWidgetCallbacks::call( 2, 0 );
	}
}

void LuaCheckBox2::doToggled(bool b)
{
    if( LuaWidgetCallbacks::pushCallbackAndMe( this, LuaWidget2::Toggled ) )
	{
        lua_pushboolean( LuaWidgetCallbacks::getLua(), b );
        LuaWidgetCallbacks::call( 2, 0 );
	}
}

void LuaRadioButton2::doClicked(bool b)
{
    if( LuaWidgetCallbacks::pushCallbackAndMe( this, LuaWidget2::Clicked ) )
	{
        lua_pushboolean( LuaWidgetCallbacks::getLua(), b );
        LuaWidgetCallbacks::call( 2, 0 );
	}
}

void LuaRadioButton2::doToggled(bool b)
{
    if( LuaWidgetCallbacks::pushCallbackAndMe( this, LuaWidget2::Toggled ) )
	{
        lua_pushboolean( LuaWidgetCallbacks::getLua(), b );
        LuaWidgetCallbacks::call( 2, 0 );
	}
}

