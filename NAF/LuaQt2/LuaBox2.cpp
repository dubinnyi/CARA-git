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

#include "LuaBox2.h"
#include <Q3Accel>
#include <Qtl2/Variant.h>
#include <LuaQt2/LuaButton2.h>
#include <Script/ValueInstaller.h>
using namespace Lua;
using namespace Qtl;

const luaL_reg LuaHBox2::methods[] =
{
	{"setSpacing",  &LuaHBox2::setSpacing },
	{0,0}
};

int LuaHBox2::setSpacing(lua_State *L)
{
	Q3HBox* obj = ObjectHelper<Q3HBox>::check( L, 1 );
	obj->setSpacing( luaL_checknumber( L, 2 ) );
	return 0;
}

const luaL_reg LuaGrid2::methods[] =
{
	{"setSpacing",  &LuaGrid2::setSpacing },
	{0,0}
};

int LuaGrid2::setSpacing(lua_State *L)
{
	Q3Grid* obj = ObjectHelper<Q3Grid>::check( L, 1 );
	obj->setSpacing( luaL_checknumber( L, 2 ) );
	return 0;
}

const luaL_reg LuaGroupBox2::methods[] =
{
	{"addSpace",  &LuaGroupBox2::addSpace },
	{"setOrientation",  &LuaGroupBox2::setOrientation },
	{"setColumns",  &LuaGroupBox2::setColumns },
	{"setAlignment",  &LuaGroupBox2::setAlignment },
	{"setTitle",  &LuaGroupBox2::setTitle },
	{0,0}
};

int LuaGroupBox2::setTitle(lua_State *L)
{
	Q3GroupBox* obj = ObjectHelper<Q3GroupBox>::check( L, 1 );
	obj->setTitle( luaL_checkstring( L, 2 ) );
	return 0;
}

int LuaGroupBox2::setAlignment(lua_State *L)
{
	Q3GroupBox* obj = ObjectHelper<Q3GroupBox>::check( L, 1 );
	obj->setAlignment( luaL_checknumber( L, 2 ) ); //  GroupBox.AlignLeft, AlignRight, AlignCenter
	return 0;
}

int LuaGroupBox2::setColumns(lua_State *L)
{
	Q3GroupBox* obj = ObjectHelper<Q3GroupBox>::check( L, 1 );
	obj->setColumns( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaGroupBox2::setOrientation(lua_State *L)
{
	Q3GroupBox* obj = ObjectHelper<Q3GroupBox>::check( L, 1 );
	obj->setOrientation( Qt::Orientation( int( luaL_checknumber( L, 2 ) ) ) ); 
	// GroupBox.Orientation
	return 0;
}

int LuaGroupBox2::addSpace(lua_State *L)
{
	Q3GroupBox* obj = ObjectHelper<Q3GroupBox>::check( L, 1 );
	obj->addSpace( luaL_checknumber( L, 2 ) );
	return 0;
}

const luaL_reg LuaLabel2::methods[] =
{
	{"getMidLineWidth",  &LuaLabel2::getMidLineWidth },
	{"setMidLineWidth",  &LuaLabel2::setMidLineWidth },
	{"getMargin",  &LuaLabel2::getMargin },
	{"setMargin",  &LuaLabel2::setMargin },
	{"getLineWidth",  &LuaLabel2::getLineWidth },
	{"setLineWidth",  &LuaLabel2::setLineWidth },
	{"getContentsRect",  &LuaLabel2::getContentsRect },
	{"setFrameStyle",  &LuaLabel2::setFrameStyle },
	{"setBuddy",  &LuaLabel2::setBuddy },
	{"setIndent",  &LuaLabel2::setIndent },
	{"setAlignment",  &LuaLabel2::setAlignment },
	{"setText",  &LuaLabel2::setText },
	{0,0}
};

int LuaLabel2::setFrameStyle(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	int style = luaL_checknumber( L, 2 );
	int shadow = QFrame::Plain;
	if( lua_gettop(L) > 2 )
		shadow = luaL_checknumber( L, 3 );
	obj->setFrameStyle( style | shadow );
	return 0;
}

int LuaLabel2::getContentsRect(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	QRect r = obj->contentsRect();
	lua_pushnumber( L, r.x() );
	lua_pushnumber( L, r.y() );
	lua_pushnumber( L, r.width() );
	lua_pushnumber( L, r.height() );
	return 4;
}

int LuaLabel2::setLineWidth(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	obj->setLineWidth( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaLabel2::getLineWidth(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	lua_pushnumber( L, obj->lineWidth() );
	return 1;
}

int LuaLabel2::setMargin(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	obj->setMargin( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaLabel2::getMargin(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	lua_pushnumber( L, obj->margin() );
	return 1;
}

int LuaLabel2::setMidLineWidth(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	obj->setMidLineWidth( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaLabel2::getMidLineWidth(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	lua_pushnumber( L, obj->midLineWidth() );
	return 1;
}

int LuaLabel2::setText(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	obj->setText( luaL_checkstring( L, 2 ) );
	return 0;
}

int LuaLabel2::setAlignment(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	// Hori, Verti, expand tabs, word break
	int hori = Qt::AlignLeft;
	int verti = Qt::AlignTop;
	if( lua_gettop(L) > 1 )
		hori = luaL_checknumber( L, 2 );
	if( lua_gettop(L) > 2 )
		verti = luaL_checknumber( L, 3 );
	if( verti == Qt::AlignHCenter )
		verti = Qt::AlignVCenter;
	int rest = 0;
	if( lua_gettop(L) > 3 && lua_isboolean( L, 4 ) && lua_toboolean( L, 4 ) )
		rest |= Qt::ExpandTabs;
	if( lua_gettop(L) > 4 && lua_isboolean( L, 5 ) && lua_toboolean( L, 5 ) )
		rest |= Qt::WordBreak;
	obj->setAlignment( hori | verti | rest );
	return 0;
}

int LuaLabel2::setIndent(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	obj->setIndent( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaLabel2::setBuddy(lua_State *L)
{
	QLabel* obj = ObjectHelper<QLabel>::check( L, 1 );
	QWidget* w = ObjectHelper<QWidget>::check( L, 2 );
	obj->setBuddy( w );
	return 0;
}

const luaL_reg LuaMenuBar2::methods[] =
{
	{"insertMenu",  &LuaMenuBar2::insertMenu },
	{"removeMenu",  &LuaMenuBar2::removeMenu },
	{"setEnabled",  &LuaMenuBar2::setEnabled },
	{"isEnabled",  &LuaMenuBar2::isEnabled },
	{"clear",  &LuaMenuBar2::clear },
	{0,0}
};

int LuaMenuBar2::insertMenu(lua_State *L)
{
	QMenuBar* obj = ObjectHelper<QMenuBar>::check( L, 1 );
	QMenu* p = ObjectHelper<QMenu>::check( L, 2 );
	const char* str = luaL_checkstring( L, 3 );
	int id = -1;
	if( lua_gettop(L) > 3 )
		id = luaL_checknumber( L, 4 );
	int index = -1;
	if( lua_gettop(L) > 4 )
		index = luaL_checknumber( L, 5 ) - 1;
	lua_pushnumber( L, obj->insertItem( str, p, id, index ) );
	return 1;
}

int LuaMenuBar2::removeMenu(lua_State *L)
{
	QMenuBar* obj = ObjectHelper<QMenuBar>::check( L, 1 );
	// id
	obj->removeItem( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaMenuBar2::setEnabled(lua_State *L)
{
	// id, bool
	QMenuBar* obj = ObjectHelper<QMenuBar>::check( L, 1 );
	bool en = true;
	if( lua_gettop(L) > 2 && lua_isboolean( L, 3 ) )
		en = lua_toboolean( L, 3 );
	obj->setItemEnabled( luaL_checknumber( L, 2 ), en );
	return 0;
}	

int LuaMenuBar2::isEnabled(lua_State *L)
{
	// id
	QMenuBar* obj = ObjectHelper<QMenuBar>::check( L, 1 );
	lua_pushboolean( L, obj->isItemEnabled( luaL_checknumber( L, 2 ) ) );
	return 1;
}

int LuaMenuBar2::clear(lua_State *L)
{
	QMenuBar* obj = ObjectHelper<QMenuBar>::check( L, 1 );
	obj->clear();
	return 0;
}

const luaL_reg LuaPopupMenu2::methods[] =
{
	{"getText",  &LuaPopupMenu2::getText },
	{"insertSubmenu",  &LuaPopupMenu2::insertSubmenu },
	{"setAccel",  &LuaPopupMenu2::setAccel },
	{"popup",  &LuaPopupMenu2::popup },
	{"isChecked",  &LuaPopupMenu2::isChecked },
	{"setChecked",  &LuaPopupMenu2::setChecked },
	{"isEnabled",  &LuaPopupMenu2::isEnabled },
	{"setEnabled",  &LuaPopupMenu2::setEnabled },
	{"setWhatsThis",  &LuaPopupMenu2::setWhatsThis },
	{"setText",  &LuaPopupMenu2::setText },
	{"setIcon",  &LuaPopupMenu2::setIcon },
	{"clear",  &LuaPopupMenu2::clear },
	{"removeItem",  &LuaPopupMenu2::removeItem },
	{"insertSeparator",  &LuaPopupMenu2::insertSeparator },
	{"insertItem",  &LuaPopupMenu2::insertItem },
	{0,0}
};

int LuaPopupMenu2::insertItem(lua_State *L)
{
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	const char* str = luaL_checkstring( L, 2 );
	int id = -1;
	if( lua_gettop(L) > 2 )
		id = luaL_checknumber( L, 3 );
	int index = -1;
	if( lua_gettop(L) > 3 )
		index = luaL_checknumber( L, 4 ) - 1;
	lua_pushnumber( L, obj->insertItem( str, id, index ) );
	return 1;
}

int LuaPopupMenu2::insertSeparator(lua_State *L)
{
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	lua_pushnumber( L, obj->insertSeparator() );
	return 1;
}

int LuaPopupMenu2::removeItem(lua_State *L)
{
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	// id
	obj->removeItem( luaL_checknumber( L, 2 ) );
	return 0;
}

int LuaPopupMenu2::clear(lua_State *L)
{
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	// RISK	luaL_error( L, "Cannot call clear during a call back to itself" );
	obj->clear();
	return 0;
}

int LuaPopupMenu2::setText(lua_State *L)
{
	// id, text
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	obj->changeItem( luaL_checknumber( L, 2 ),
		luaL_checkstring( L, 3 ) );
	return 0;
}

int LuaPopupMenu2::setWhatsThis(lua_State *L)
{
	// id, text
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	obj->setWhatsThis( luaL_checknumber( L, 2 ),
		luaL_checkstring( L, 3 ) );
	return 0;
}

int LuaPopupMenu2::setEnabled(lua_State *L)
{
	// id, bool
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	bool en = true;
	if( lua_gettop(L) > 2 && lua_isboolean( L, 3 ) )
		en = lua_toboolean( L, 3 );
	obj->setItemEnabled( luaL_checknumber( L, 2 ), en );
	return 0;
}

int LuaPopupMenu2::isEnabled(lua_State *L)
{
	// id
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	lua_pushboolean( L, obj->isItemEnabled( luaL_checknumber( L, 2 ) ) );
	return 1;
}	

int LuaPopupMenu2::setChecked(lua_State *L)
{
	// id, bool
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	bool en = true;
	if( lua_gettop(L) > 2 && lua_isboolean( L, 3 ) )
		en = lua_toboolean( L, 3 );
	obj->setItemChecked( luaL_checknumber( L, 2 ), en );
	return 0;
}

int LuaPopupMenu2::isChecked(lua_State *L)
{
	// id
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	lua_pushboolean( L, obj->isItemChecked( luaL_checknumber( L, 2 ) ) );
	return 1;
}

int LuaPopupMenu2::popup(lua_State *L)
{
	// x, y
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	QPoint p( luaL_checknumber( L, 2 ), luaL_checknumber( L, 3 ) );
	obj->popup( p );
	return 0;
}

int LuaPopupMenu2::setAccel(lua_State *L)
{
	// id, key-string
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	obj->setAccel( luaL_checknumber( L, 2 ),
		Q3Accel::stringToKey( luaL_checkstring( L, 3 ) ) );
	return 0;
}

int LuaPopupMenu2::insertSubmenu(lua_State *L)
{
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	QMenu* p = ObjectHelper<QMenu>::check( L, 2 );
	const char* str = luaL_checkstring( L, 3 );
	int id = -1;
	if( lua_gettop(L) > 3 )
		id = luaL_checknumber( L, 4 );
	int index = -1;
	if( lua_gettop(L) > 4 )
		index = luaL_checknumber( L, 5 ) - 1;
	lua_pushnumber( L, obj->insertItem( str, p, id, index ) );
	return 1;
}

int LuaPopupMenu2::getText(lua_State *L)
{
	// id
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	lua_pushstring( L, obj->text( luaL_checknumber( L, 2 ) ) );
	return 1;
}

void LuaMenuBar2::_activated(int itemId)
{
	ObjectPeer* peer = ObjectPeer::getPeer( this );
	if( LuaWidget2::pushCallback( peer, LuaWidget2::Activated ) )
	{
		peer->pushMe();
		lua_pushnumber( peer->getLua(), itemId );
		LuaWidget2::call( peer->getLua(), 2, 0 );
	}
}

void LuaPopupMenu2::_activated(int itemId)
{
	ObjectPeer* peer = ObjectPeer::getPeer( this );
	if( LuaWidget2::pushCallback( peer, LuaWidget2::Activated ) )
	{
		peer->pushMe();
		lua_pushnumber( peer->getLua(), itemId );
		LuaWidget2::call( peer->getLua(), 2, 0 );
	}
}

void LuaPopupMenu2::_aboutToShow()
{
	ObjectPeer* peer = ObjectPeer::getPeer( this );
	if( LuaWidget2::pushCallback( peer, LuaWidget2::Showing ) )
	{
		peer->pushMe();
		LuaWidget2::call( peer->getLua(), 1, 0 );
	}
}

int LuaPopupMenu2::setIcon(lua_State *L)
{
	QMenu* obj = ObjectHelper<QMenu>::check( L, 1 );
	QPixmap* img = ValueInstaller<QPixmap>::check( L, 3 );
	obj->changeItem( luaL_checknumber( L, 2 ), *img );
	return 0;
}

const luaL_reg LuaButtonGroup2::methods[] =
{
	{"setExclusive",  &LuaButtonGroup2::setExclusive },
	{"addButton",  &LuaButtonGroup2::addButton },
	{0,0}
};

int LuaButtonGroup2::setExclusive(lua_State *L)
{
	Q3ButtonGroup* obj = ObjectHelper<Q3ButtonGroup>::check( L, 1 );
	bool b = true;
	if( lua_gettop(L) > 1 && lua_isboolean( L, 2 ) )
		b = lua_toboolean( L, 2 );
	obj->setExclusive( b );
	return 0;
}

int LuaButtonGroup2::addButton(lua_State *L)
{
	Q3ButtonGroup* obj = ObjectHelper<Q3ButtonGroup>::check( L, 1 );
	QAbstractButton* b = ObjectHelper<QAbstractButton>::check( L, 2 );
	int id = -1;
	if( lua_gettop(L) > 2 )
		id = luaL_checknumber( L, 3 );
	lua_pushnumber( L, obj->insert( b, id ) );
	return 1;
}

void LuaButtonGroup2::_clicked(int id)
{
	ObjectPeer* peer = ObjectPeer::getPeer( this );
	if( LuaWidget2::pushCallback( peer, LuaWidget2::Clicked ) )
	{
		peer->pushMe();
		lua_pushnumber( peer->getLua(), id );
		LuaWidget2::call( peer->getLua(), 2, 0 );
	}
}

void LuaBox2::install( lua_State *L )
{
	ObjectInstaller<Q3HBox>::install( L, LuaHBox2::methods,
		ObjectInstaller<QFrame>::className() );

	ObjectInstaller<Q3VBox>::install( L, 0,
		ObjectInstaller<Q3HBox>::className() );

	ObjectInstaller<Q3Grid, AbstractObjectFactory>::install( L,
		LuaGrid2::methods, ObjectInstaller<QFrame>::className() );

	ObjectInstaller<Q3GroupBox>::install( L, LuaGroupBox2::methods,
		ObjectInstaller<QWidget>::className() );

	ObjectInstaller<Q3ButtonGroup>::install( L, LuaButtonGroup2::methods,
		ObjectInstaller<Q3GroupBox>::className() );

	// ObjectInstaller<QLabel>::install( L );
	ObjectInstaller<QLabel>::addMethods( L, LuaLabel2::methods );

	// ObjectInstaller<QMenuBar>::install( L );
	ObjectInstaller<QMenuBar>::addMethods( L, LuaMenuBar2::methods );

	// ObjectInstaller<QMenu>::install( L );
	ObjectInstaller<QMenu>::addMethods( L, LuaPopupMenu2::methods );

}
