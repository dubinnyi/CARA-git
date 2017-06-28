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

#if !defined(AFX_GLYPHTRAVERSAL_H__DE3CA062_D2D1_11D4_831C_9DADD2F88E11__INCLUDED_)
#define AFX_GLYPHTRAVERSAL_H__DE3CA062_D2D1_11D4_831C_9DADD2F88E11__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Lexi/Units.h>
#include <Lexi/Allocation.h>
#include <QHash>
#include <Root/Sequence.h>

namespace Lexi
{
	class Glyph;
	class Viewport;


	//* GlyphTraversal
	//. Stark minimierte Version per 12.2.07. Wir nur noch von Glyph::traverse verwendet.

	class GlyphTraversal
	{
	public: 
		bool carryOn() const;

		//* Neue Traversalmethode
		bool traverse( Glyph*, const Allocation& );
		//. Jeder Glyph ruft diese Methode f�r jede seiner Komponenten auf. Der R�ckgabewert
		//. bedeutet "CarryOn", d.h. Komponenten werden solange durchlaufen, bis false
		//. zur�ckkommt.

		Message* getMessage() const;
		Glyph* getTarget() const;
		Viewport* getViewport() const;
		//. Da der Traversal eine Referenz auf Viewport speichert, muss diese nicht mehr
		//. in der Methodensignatur stehen.
		bool accepts( const Glyph*, const Allocation& ) const;
		//. Pr�fe, ob Traversal den gegebenen Glyph mit der gegebenen Allocation 
		//. akzeptiert (true) oder nicht (false). Ruft intern Condition::check auf.

		enum VisitType { 
			All,			// Alle Glyphs erhalten Rechenzeit f�r handle()
			AllAccept,		// Alle akzeptierten Glyphs erhalten Rechenzeit f�r handle()
			AnyAccept,		// Nur erster akzeptierter Glyphs erh�lt Rechenzeit f�r handle().
			AllDone,		// Alle akzeptierten Glyphs erhalten Rechenzeit f�r handle() solange done.
			AnyDone,		// Alle akzeptierten Glyphs erhalten Rechenzeit f�r handle(), bis done
			AllDF,			// Alle Glyphs erhalten Rechenzeit auf R�ckweg f�r handle()
			AllAcceptDF,	// Gehe in Tiefe solange accepted. R�ckweg handle f�r alle.
			AnyAcceptDF,	// Identisch mit AnyAccept.
			AllDoneDF,		// Gehe in die Tiefe solange accepted. R�ckweg handle solange done.
			AnyDoneDF,		// Gehe in die Tiefe solange accepted. R�ckweg handle() bis done.
			AnyAcceptDoneDF	// Gehe in die Tiefe bis accepted. R�ckweg handle() bis done.
		};
		//. DF...Depth First, d.h. zuerst traverse() dann handle() (bzw. handle() erst beim R�ckweg).

		VisitType getType() const;

		void push( Glyph* container, GlyphIndex component, Glyph* = nil );
		//. Wird von einem getroffenen PolyGlyph f�r jede Component aufgerufen (statt hit), bevor 
		//. deren pick aufgerufen wird (d.h. innerhalb der for-Schleife). Dies nur dann, 
		//. wenn er selber nicht auf die Liste kommen soll, falls seine Components keinen 
		//. Hit haben. Sonst hit verwenden.
		void pop();
		//. Pendant zu begin. Wird aufgerufen, nachdem pick() einer Component aufgerufen wurde hat.


		GlyphTraversal(Viewport*, VisitType, Glyph* to, Message* ); // used
		GlyphTraversal(Viewport*, VisitType, Message* ); // used

		virtual ~GlyphTraversal();
	private:
		class SendTo 
		{
			Glyph* d_to;
		public:
			Glyph* getTo() const { return d_to; }
			SendTo( Glyph* g ): d_to( g ) {}
			bool check( const Glyph* g, const Allocation& ) const
			{
				return d_to == 0 || g == d_to;
			}
			~SendTo() {}
		};

		struct Imp 
		{
			Imp(Viewport* v, GlyphTraversal::VisitType t, Glyph* to, Message* m);
			virtual ~Imp();

			// Glyph* d_root;
			//. Der erste Glyph, der auf dem Pfad zum Hit registriert wurde.
			// Root::Sequence<Glyph*> d_handlers;
			//. Handler aller getroffenen Glyphs sowie derjenigen auf dem Weg dorthin. Der Handler des
			//. in der visuellen Hierarchie vordersten Glyph hat den gr�ssten Index.
			SendTo d_condition;
			//. Die Condition, an welche check delegiert. Traversal wird Owner.
			Message* d_message;
			//. Die Message, welche im Rahmen des vorliegenden Traversals behandelt werden soll. Traversal wird Owner.
			Viewport* d_port;
			GlyphTraversal::VisitType d_visit;
			bool d_done;	// Separat von Message::consume, da letzteres extern zur�ckgesetzt werden kann.
							// Glyph::traverse implementiert einen von consume unabh�ngigen Zustellmechanismus.
			bool d_accepted;

		public:
			void clear();
		};
		Imp d_this;

		void reset( Viewport*, VisitType, SendTo&, Message* );
		//. Reinitialisiert diesen Traversal zwecks Wiederverwendung. Sonst
		//. gleich wie Konstruktor.

		DONT_CREATE_ON_HEAP;
	};
}

#endif // !defined(AFX_GLYPHTRAVERSAL_H__DE3CA062_D2D1_11D4_831C_9DADD2F88E11__INCLUDED_)
