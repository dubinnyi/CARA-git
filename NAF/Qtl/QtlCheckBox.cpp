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

#include <QCheckBox>
#include "QtlCheckBox.h"
#include <Script/ObjectInstaller.h>
#include <Script/ValueInstaller.h>
using namespace Qtl;
using namespace Lua;
int CheckBox::checkState(lua_State * L) // const : Qt::CheckState
{
	QCheckBox* obj = ObjectHelper<QCheckBox>::check( L, 1);
	Util::push( L, obj->checkState());
	return 1;
}
int CheckBox::setCheckState(lua_State * L) // ( Qt::CheckState state )
{
	QCheckBox* obj = ObjectHelper<QCheckBox>::check( L, 1);
	Qt::CheckState f;
	int fInt = Util::toInt( L, 2);
	if( (fInt >= 0 && fInt <= 2) )
	{
		f = (Qt::CheckState) fInt;
		obj->setCheckState( f );
	}
	else
		Lua::Util::error( L, "expecting a valid Qt::CheckState" );
	return 0;
}
static const luaL_reg _CheckBox[] = 
{
	{ "checkState", CheckBox::checkState },
	{ "setCheckState", CheckBox::setCheckState },
	{ 0, 0 }
};
void CheckBox::install(lua_State * L){
	ObjectInstaller<QCheckBox>::install( L, _CheckBox, ObjectInstaller<QAbstractButton>::className());
}
