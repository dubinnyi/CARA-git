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

#if !defined(AFX_ACTION_H__2D0B9A81_5436_11D5_8DB0_00D00918E99C__INCLUDED_)
#define AFX_ACTION_H__2D0B9A81_5436_11D5_8DB0_00D00918E99C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <Root/Object.h>
#include <Root/Sequence.h>

namespace Root
{
	class Any;

	//* Action
	//. Eine Action ist ein parameterloser Ben�tzerbefehl, der
	//. entweder via Men�, Toolbar oder Interpreter-Command ins
	//. System gelangt. Eine Action ist ein Message, die etwas �ber ihre
	//. eigene Identit�t und Verf�gbarkeit weiss. In diesem
	//. Sinne bietet sie ihrem Owner Properties f�r ihren
	//. eigenen Aktivit�tszustand und Dekorationsattribute.
	//. Alle Objekte, die auf Actions reagieren, m�ssen die beiden F�lle
	//. Execution (isUpdate=false) und Update (isUpdate=true) unterscheiden.
	//. Mit diesem Unterschied kann dieselbe Action vor der eigentlichen 
	//. Ausf�hrung zur Pr�fung ihrer Verf�gbarkeit und allenfalls ihres 
	//. Flipflop-Zustands verwendet werden.
	//. Actions k�nnen einfach in den Chain-Of-Command geworfen werden, ohne
	//. an einen konkreten Executor gebunden zu sein. Auf diese Weise wandert
	//. die Action solange durch das System, bis sie jemand ausf�hrt. Normalerweise
	//. geht eine Action an den Focus-Glyph.
	//. Im Gegensatz zu den �brigen Messages unterscheidet man eine konkrete
	//. Action durch den Command-String (operator==). Ein Command ist ein statischer
	//. const-String, dessen Adresse als Identifikation des Commands dient.

	class Action : public Message
	{
	public:
		typedef const char* CmdStr;
		typedef SymbolString CmdSym;
		typedef QVector<Any> Params;

		//* Lokale Helper
		bool checkAndRun();
		//. Ruft zuerst update() auf und dann execute(), falls isEnabled().
		//. Gibt true zur�ck, falls execute() aufgerufen wurde.

		//* Interface f�r Executor
		virtual void setEnabled( bool = true );
		virtual void setOn( bool = true );
		//. Executors k�nnen dieser Action w�hrend einem Update-Zyklus oder
		//. auch sonst Enabled setzen.
		virtual CmdSym getCommand() const;
		//. Gibt die mit der Action assoziierte Befehlszeile zur�ck.
		virtual bool isUpdate() const;
		//. Falls True handelt es sich nur um einen Update
		//. Request (initiiert durch update() ) und nicht
		//. die Ausf�hrung (initiiert durch execute() ).
		virtual bool operator==( CmdStr ) const;
		virtual Index getParamCount() const;
		virtual const Any& getParam( Index ) const;
		//. Die Action kann optional �ber Parameter verf�gen. getParamCount()
		//. gibt die Anzahl enthaltener Parameter zur�ck. getParam() erlaubt
		//. Lesezugriff auf die Parameter.

		//* Interface zu Men�/Toolbar
		virtual bool isFlipFlop() const;
		//. Gibt dem Caller an, ob die Action �ber einen
		//. bin�ren Schalterzustand verf�gt oder nur ein Event ist.
		virtual const char* getPrettyName() const;
		//. Der Caller erh�lt eine Bezeichnung der Action zur 
		//. Darstellung im GUI (z.B. Menue-Item). 
		//. RISK: Internationalisierung.
		virtual void execute();
		//. Wird vom Client aufgerufen, um die mit dieser Action
		//. verbundene Aktivit�ten auszuf�hren.
		virtual void update();
		//. Bereitet die Action darauf vor, dass demn�chst isEnabled und
		//. isOn aufgerufen werden. 
		virtual bool isEnabled() const;
		//. Der Client fragt, ob die mit der Action verbundenen
		//. Aktivit�ten zur Zeit ausgef�hrt werden k�nnten.
		virtual bool isOn() const;
		//. Der Client fragt, ob die Aktion den Zustand "eingeschaltet"
		//. hat, was sich im GUI mit einem H�kchen oder einem gedr�ckten
		//. Button manifestiert.

		//* TODO
		// Icon getIcon() const;
		Action();

		//* Standard-Actions
		static CmdStr Nop;
		static CmdStr ExecuteLine;	// z.B. Lua-Befehlszeile
		static CmdStr ExecuteCmd;	// z.B. CmdStr mit Standardparameter
		static CmdStr FileNew;
		static CmdStr FileOpen;
		static CmdStr FileClose;
		static CmdStr FileSave;
		static CmdStr FileSaveAs;
		static CmdStr FilePageSetup;
		static CmdStr FilePrintPreview;
		static CmdStr FileExportPdf;
		static CmdStr FilePrint;
		static CmdStr FileQuit;
		static CmdStr EditUndo;
		static CmdStr EditRedo;
		static CmdStr EditCut;
		static CmdStr EditCopy;
		static CmdStr EditPaste;
		static CmdStr EditDelete;
		static CmdStr EditSelectAll;
		static CmdStr EditFind;
		static CmdStr EditFindAgain;
		static CmdStr EditReplace;
		static CmdStr WindowClose;
		static CmdStr WindowCloseAll;
		static CmdStr WindowCloseAllButThis;
		static CmdStr WindowCascade;
		static CmdStr WindowNew;
		static CmdStr WindowShowStatusBar;
		static CmdStr SystemKillUndo;
		static CmdStr HelpContents;
		static CmdStr HelpSearch;
		static CmdStr HelpContext;
		static CmdStr HelpAbout;
		static CmdStr ViewBackward;
		static CmdStr ViewForward;
		static CmdStr ViewRefresh;
		static CmdStr ViewAbort;
		static CmdStr ViewFullScreen;
		static CmdStr RecordUpdate;
		static CmdStr RecordRestore;
		static CmdStr RecordCreate;
		static CmdStr RecordCreateSub;
		static CmdStr RecordDelete;
		static CmdStr RecordNext;
		static CmdStr RecordPrev;

		static const char* prettyName( CmdStr cmd );

		virtual ~Action();
	};

	//* StateAction
	//. Diese Variante f�hrt den Zustand redundant mit. Dies ist dann praktisch,
	//. wenn - im Falle eines FlipFlops - der betroffene Executor die Action gerade
	//. als Zustandsvariable einsetzt.

	class StateAction : public Action
	{
	public:
		StateAction( CmdStr cmd );
		StateAction( const char* name, CmdStr cmd, bool flipflop = false );

		//* Overrides
		bool operator==( CmdStr ) const;
		void setOn( bool );
		void setEnabled( bool );
		bool isEnabled() const { return d_enabled; }
		bool isOn() const { return d_on; }
		bool isFlipFlop() const { return d_flipflop; }
		const char* getPrettyName() const { return d_name; }
		CmdSym getCommand() const { return d_command; }
		~StateAction() {}
	protected:
		bool d_on;
		bool d_enabled;
		bool d_flipflop;
		QByteArray d_name;
		CmdSym d_command;
	};
}

#endif // !defined(AFX_ACTION_H__2D0B9A81_5436_11D5_8DB0_00D00918E99C__INCLUDED_)
