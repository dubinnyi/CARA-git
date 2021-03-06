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

#include "PointSelectCtrl.h"
#include <SpecView/ViewAreaMdl.h>
#include <Lexi/MainWindow.h>
#include <Lexi/SelectorCmd.h>
#include <Lexi/Event.h>
using namespace Spec;
using namespace Lexi;

const int s_limit = 5;

//////////////////////////////////////////////////////////////////////


static void notifySelection( SpinPointView* v, SpinPointView::Label l )
{
	Viewport* vp = v->getViewport(false);
	if( vp == 0 )
		return;
	QByteArray  s;
	if( SpinPointView::formatSelection( s, v, l, s_limit ) )
	{
		Lexi::ShowStatusMessage msg( s.data() );
		vp->traverseUp( msg ); 
	}
}

//////////////////////////////////////////////////////////////////////

class _SelectPointCmd : public SelectorCmd
{
	SpinPointView* d_view;
public:
	_SelectPointCmd( Glyph* ctrl, SpinPointView* m ):
	  SelectorCmd( ctrl ), d_view( m )
	{
	}
	Result handleMouseDrag( Viewport& v, const Allocation& a, const DragEvent& e )
	{
		Result res = SelectorCmd::handleMouseDrag( v, a, e );
		Allocation r;
		getAllocation( r );
		d_view->getViewArea()->printPpmRect( &v, r );
		return res;
	}
	void execute()
	{
		Allocation r;
		getAllocation( r );
		ViewAreaMdl* mdl = d_view->getViewArea();
		Allocation a = mdl->getAllocation();
		PpmCube c;
		c.assign( 2, PpmRange() );
		c[ DimX ].first = mdl->toPpm( r.getLeft(), a.getLeft(), DimX );
		c[ DimX ].second = mdl->toPpm( r.getRight(), a.getLeft(), DimX );
		c[ DimY ].first = mdl->toPpm( r.getTop(), a.getTop(), DimY );
		c[ DimY ].second = mdl->toPpm( r.getBottom(), a.getTop(), DimY );
		d_view->selectPeak( c );

		notifySelection( d_view, SpinPointView::PairIdLabelSysOrResi );
		Command::execute();
	}
};

//////////////////////////////////////////////////////////////////////

PointSelectCtrl::PointSelectCtrl(SpinPointView* mdl, bool swallow ):
	d_view( mdl ), d_swallow( swallow )
{
	assert( !d_view.isNull() );
}

PointSelectCtrl::~PointSelectCtrl()
{

}

bool PointSelectCtrl::handleMousePress( Viewport& v, const Allocation& a, const MouseEvent& e ) 
{ 
	if( e.isLeft() && e.isShift() && !e.isAlt() && !e.isCtrl() )
	{
		return installCommand( new _SelectPointCmd( this, d_view ) ); // && d_swallow; 
	}else if( e.isLeft() && e.isNoKey() )
	{
		ViewAreaMdl* mdl = d_view->getViewArea();
		d_view->selectPeak( mdl->toPpm( e.getX(), a.getLeft(), DimX ), 
			mdl->toPpm( e.getY(), a.getTop(), DimY ) );	
		// Nicht immer true zur�ck, damit Cursor funktioniert.
		notifySelection( d_view, SpinPointView::PairIdLabelSysOrResi );
		return d_swallow;
	}
	// else
		return false;
}

//////////////////////////////////////////////////////////////////////

class _SelectPoint1DCmd : public SelectorCmd
{
	SpinPoint1DView* d_view;
	bool d_notify;
public:
	_SelectPoint1DCmd( Glyph* ctrl, SpinPoint1DView* m, bool n ):
	  SelectorCmd( ctrl ), d_view( m ), d_notify( n )
	{
	}
	Result handleMouseDrag( Viewport& v, const Allocation& a, const DragEvent& e )
	{
		Result res = SelectorCmd::handleMouseDrag( v, a, e );
		if( d_notify )
		{
			Allocation r;
			getAllocation( r );
			d_view->getViewArea()->printPpmRect( &v, r );
		}
		return res;
	}
	void execute()
	{
		Allocation r;
		getAllocation( r );
		ViewAreaMdl* mdl = d_view->getViewArea();
		Allocation a = mdl->getAllocation();
		PpmCube c;
		c.assign( 1, PpmRange() );
		Dimension dim = d_view->getDir();
		c[ DimX ].first = mdl->toPpm( r.getAllotment( dim ).getBegin(), 
			a.getAllotment( dim ).getBegin(), dim );
		c[ DimX ].second = mdl->toPpm( r.getAllotment( dim ).getEnd(), 
			a.getAllotment( dim ).getBegin(), dim );
		d_view->selectPeak( c );
		Command::execute();
	}
};

//////////////////////////////////////////////////////////////////////

PointSelect1DCtrl::PointSelect1DCtrl(SpinPoint1DView* mdl,bool swallow, bool n ):
	d_view( mdl ), d_swallow( swallow ), d_notify( n )
{
	assert( !d_view.isNull() );
}

PointSelect1DCtrl::~PointSelect1DCtrl()
{

}

bool PointSelect1DCtrl::handleMousePress( Viewport& v, const Allocation& a, const MouseEvent& e ) 
{ 
	if( e.isLeft() && e.isShift() && !e.isAlt() && !e.isCtrl() )
	{
		return installCommand( new _SelectPoint1DCmd( this, d_view, d_notify ) ); // && d_swallow; 
	}else if( e.isLeft() && e.isNoKey() )
	{
		ViewAreaMdl* mdl = d_view->getViewArea();
		if( d_view->getDir() == DimX )
			d_view->selectPeak( mdl->toPpm( e.getX(), a.getLeft(), DimX ) );
		else
			d_view->selectPeak( mdl->toPpm( e.getY(), a.getTop(), DimY ) );	
		// Nicht immer true zur�ck, damit Cursor funktioniert.
		notifySelection( d_view, SpinPointView::SysOrResiTagAll );
		return d_swallow;
	}
	// else
		return false;
}
