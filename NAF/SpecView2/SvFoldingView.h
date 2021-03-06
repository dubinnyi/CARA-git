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

#if !defined(AFX_FOLDINGVIEW_H__6B237716_23B8_4312_9645_21277E0D048E__INCLUDED_)
#define AFX_FOLDINGVIEW_H__6B237716_23B8_4312_9645_21277E0D048E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <SpecView2/SvUnits.h>
#include <SpecView2/SvSpecBufferMdl.h>


namespace Spec2
{
	class FoldingView  : public Lexi2::Glyph 
	{
	public:
		SpecBufferMdl* getModel() const { return d_model; }

		FoldingView( SpecBufferMdl*, QColor = Qt::darkGray);
		//* Overrides von Glyph
		void draw(Canvas&, const Allocation&);
	protected:
		virtual ~FoldingView();
	private:
		Root::Ref<SpecBufferMdl> d_model;
		QColor d_clr;
	};
}

#endif // !defined(AFX_FOLDINGVIEW_H__6B237716_23B8_4312_9645_21277E0D048E__INCLUDED_)
