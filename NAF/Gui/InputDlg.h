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
#ifndef _Gui_InputDlg
#define _Gui_InputDlg

#include <QDialog>
#include <QGridLayout>

class QLayout;

namespace Gui
{
	class InputDlg : public QDialog
	{
	public:
		InputDlg( QWidget*, const QString& title = "" );
		void add( QWidget*, int row, int col );
		void add( QWidget*, int row, int col, int rowSpan, int colSpan = 1 );
		void add( QLayout*, int row, int col );
		void add( QLayout*, int row, int col, int rowSpan, int colSpan = 1 );
		void addLabel( const QString&,int row, int col, int rowSpan = 1, int colSpan = 1 );
		bool exec();
	private:
		QGridLayout* d_grid;
	};
}

#endif
