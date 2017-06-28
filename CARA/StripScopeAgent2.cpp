/*
 * Copyright 2000-2015 Rochus Keller <mailto:rkeller@nmr.ch>
 *
 * This file is part of CARA (Computer Aided Resonance Assignment,
 * see <http://cara.nmr.ch/>).
 *
 * CARA is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (GPL) as
 * published by the Free Software Foundation, either version 2 of
 * the License, or (at your option) any later version.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "StripScopeAgent2.h"
#include <qtextstream.h> 
#include <qinputdialog.h> 
#include <qmessagebox.h>

#define INCLUDE_MENUITEM_DEF	
#include <qmenudata.h>
#include <qpainter.h>	// TEST

#include <Root/Any.h>
#include <Root/MakroCommand.h>
#include <Lexi/MainWindow.h>
#include <Lexi/Background.h>
#include <Lexi/Label.h>
#include <Lexi/ContextMenu.h>
#include <Spec/SpecProjector.h>
#include <Spec/SpectrumType.h>
#include <Spec/SpectrumPeer.h>
#include <Spec/Repository.h>
#include <Spec/SpecRotator.h>
#include <Spec/FragmentAssignment.h>
#include <SpecView/CursorMdl.h>
#include <SpecView/ViewAreaMdl.h>
#include <SpecView/SpecViewer.h>
#include <SpecView/FocusCtrl.h>
#include <SpecView/ContourView.h>
#include <SpecView/CursorView.h>
#include <SpecView/CursorCtrl.h>
#include <SpecView/ScrollCtrl.h>
#include <SpecView/ZoomCtrl.h>
#include <SpecView/SelectZoomCtrl.h>
#include <SpecView/SliceView.h>
#include <SpecView/CenterLine.h>
#include <SpecView/SelectRulerCtr.h>
#include <Spec/GlobalCursor.h>
#include <SpecView/PointSelectCtrl.h>
#include <SpecView/FoldingView.h>

#include <SpecView/DynValueEditor.h>
#include <SingleAlignmentView.h>
#include <SpecView/RotateDlg.h>
#include <Dlg.h>
#include <SpecView/CandidateDlg.h>
#include <SpecView/GotoDlg.h>
#include <ReportViewer.h>
#include "ProposeSpinDlg.h"
using namespace Spec;
using namespace Root;

static const int BACKGROUND = 0;
static const int INTENSITY = 1;
static const int CONTOUR = 2;
static const int FOLDING = 3;
static const int LABEL1 = 4;
static const int LABEL2 = 5;
static const int PEAKS = 6;
static const int CURSORS = 7;
static const int TUPLES = 8;
static const int VIEWCOUNT = 9;

static QColor g_clrHold = Qt::cyan;
static QColor g_clrLabel = Qt::yellow;

//////////////////////////////////////////////////////////////////////

Action::CmdStr StripScopeAgent2::ContourParams = "ContourParams";
Action::CmdStr StripScopeAgent2::AutoContour = "AutoContour";
Action::CmdStr StripScopeAgent2::FitWindow = "FitWindow";
Action::CmdStr StripScopeAgent2::DeleteSpins = "DeleteSpins";
Action::CmdStr StripScopeAgent2::PickSpin = "PickSpin";
Action::CmdStr StripScopeAgent2::MoveSpin = "MoveSpin";
Action::CmdStr StripScopeAgent2::MoveSpinAlias = "MoveSpinAlias";
Action::CmdStr StripScopeAgent2::LabelSpin = "LabelSpin";
Action::CmdStr StripScopeAgent2::ForceLabelSpin = "ForceLabelSpin";
Action::CmdStr StripScopeAgent2::ShowAlignment = "ShowAlignment";
Action::CmdStr StripScopeAgent2::SelectSpec = "SelectSpec";
Action::CmdStr StripScopeAgent2::SetResolution = "SetResolution";
Action::CmdStr StripScopeAgent2::ShowLowRes = "ShowLowRes";
Action::CmdStr StripScopeAgent2::ForwardStrip = "ForwardStrip";
Action::CmdStr StripScopeAgent2::BackwardStrip = "BackwardStrip";
Action::CmdStr StripScopeAgent2::SelectStrips = "SelectStrips";
Action::CmdStr StripScopeAgent2::HoldStrip = "HoldStrip";
Action::CmdStr StripScopeAgent2::GotoStrip = "GotoStrip";
Action::CmdStr StripScopeAgent2::LinkReference = "LinkReference";
Action::CmdStr StripScopeAgent2::UnlinkSucc = "UnlinkSucc";
Action::CmdStr StripScopeAgent2::UnlinkPred = "UnlinkPred";
Action::CmdStr StripScopeAgent2::LinkStrip = "LinkStrip";
Action::CmdStr StripScopeAgent2::AssignStrip = "AssignStrip";
Action::CmdStr StripScopeAgent2::AutoCenter = "AutoCenter";
Action::CmdStr StripScopeAgent2::UnassignStrip = "UnassignStrip";
Action::CmdStr StripScopeAgent2::StrictStripMatch = "StrictStripMatch";
Action::CmdStr StripScopeAgent2::UnlabeledStripMatch = "UnlabeledStripMatch";
Action::CmdStr StripScopeAgent2::ShowFolded = "ShowFolded";
Action::CmdStr StripScopeAgent2::SetCandidates = "SetCandidates";
Action::CmdStr StripScopeAgent2::SetWidth = "SetWidth";
Action::CmdStr StripScopeAgent2::SetTol = "SetTol";
Action::CmdStr StripScopeAgent2::CreateReport = "CreateReport";
Action::CmdStr StripScopeAgent2::EditAtts = "EditAtts";
Action::CmdStr StripScopeAgent2::ReplaceStrip = "ReplaceStrip";
Action::CmdStr StripScopeAgent2::CursorSync = "CursorSync";
Root::Action::CmdStr StripScopeAgent2::NextSpec = "NextSpec";
Root::Action::CmdStr StripScopeAgent2::PrevSpec = "PrevSpec";
Action::CmdStr StripScopeAgent2::AutoGain = "AutoGain";
Action::CmdStr StripScopeAgent2::NextStrip = "NextStrip";
Action::CmdStr StripScopeAgent2::PrevStrip = "PrevStrip";
Action::CmdStr StripScopeAgent2::ShowWithOff = "ShowWithOff";
Action::CmdStr StripScopeAgent2::ShowGhosts = "ShowGhosts";
Action::CmdStr StripScopeAgent2::DoPathSim = "DoPathSim";
Action::CmdStr StripScopeAgent2::ShowUnlabeled = "ShowUnlabeled";
Action::CmdStr StripScopeAgent2::ShowInfered = "ShowInfered";
Action::CmdStr StripScopeAgent2::ShowLinks = "ShowLinks";
Action::CmdStr StripScopeAgent2::ProposeSpin = "ProposeSpin";
Action::CmdStr StripScopeAgent2::PickLabel = "PickLabel";
Action::CmdStr StripScopeAgent2::SyncStrip = "SyncStrip";
Action::CmdStr StripScopeAgent2::ViewLabels = "ViewLabels";
Action::CmdStr StripScopeAgent2::SetDepth = "SetDepth";
Action::CmdStr StripScopeAgent2::AnchPathSim = "AnchPathSim";
Action::CmdStr StripScopeAgent2::GhostLabels = "GhostLabels";
Action::CmdStr StripScopeAgent2::PickBounds = "PickBounds";
Action::CmdStr StripScopeAgent2::SetBounds = "SetBounds";
Action::CmdStr StripScopeAgent2::AutoSlice = "AutoSlice";
Action::CmdStr StripScopeAgent2::PickBoundsSym = "PickBoundsSym";
Action::CmdStr StripScopeAgent2::DeleteLinks = "DeleteLinks";
Action::CmdStr StripScopeAgent2::ShowAllPeaks = "ShowAllPeaks";
Action::CmdStr StripScopeAgent2::HidePeak = "HidePeak";
Action::CmdStr StripScopeAgent2::KeepSpec = "KeepSpec";
Action::CmdStr StripScopeAgent2::EditAttsSys = "EditAttsSys";
Action::CmdStr StripScopeAgent2::EditAttsLink = "EditAttsLink";
Action::CmdStr StripScopeAgent2::Calibrate = "Calibrate";
Action::CmdStr StripScopeAgent2::ShowUnknown = "ShowUnknown";

ACTION_SLOTS_BEGIN( StripScopeAgent2 )
    { StripScopeAgent2::ShowUnknown, &StripScopeAgent2::handleShowUnknown },
    { StripScopeAgent2::Calibrate, &StripScopeAgent2::handleCalibrate },
    { StripScopeAgent2::EditAttsSys, &StripScopeAgent2::handleEditAttsSys },
    { StripScopeAgent2::EditAttsLink, &StripScopeAgent2::handleEditAttsLink },
    { StripScopeAgent2::KeepSpec, &StripScopeAgent2::handleKeepSpec },
    { StripScopeAgent2::HidePeak, &StripScopeAgent2::handleHidePeak },
    { StripScopeAgent2::ShowAllPeaks, &StripScopeAgent2::handleShowAllPeaks },
    { StripScopeAgent2::DeleteLinks, &StripScopeAgent2::handleDeleteLinks },
    { StripScopeAgent2::PickBounds, &StripScopeAgent2::handlePickBounds },
    { StripScopeAgent2::SetBounds, &StripScopeAgent2::handleSetBounds },
    { StripScopeAgent2::AutoSlice, &StripScopeAgent2::handleAutoSlice },
    { StripScopeAgent2::PickBoundsSym, &StripScopeAgent2::handlePickBoundsSym },
    { StripScopeAgent2::GhostLabels, &StripScopeAgent2::handleGhostLabels },
    { StripScopeAgent2::AnchPathSim, &StripScopeAgent2::handleAnchPathSim },
    { StripScopeAgent2::ViewLabels, &StripScopeAgent2::handleViewLabels },
    { StripScopeAgent2::SyncStrip, &StripScopeAgent2::handleSyncStrip },
    { StripScopeAgent2::ShowWithOff, &StripScopeAgent2::handleShowWithOff },
    { StripScopeAgent2::PickLabel, &StripScopeAgent2::handlePickLabel },
    { StripScopeAgent2::ProposeSpin, &StripScopeAgent2::handleProposeSpin },
    { StripScopeAgent2::ShowGhosts, &StripScopeAgent2::handleShowGhosts },
    { StripScopeAgent2::DoPathSim, &StripScopeAgent2::handleDoPathSim },
    { StripScopeAgent2::ShowUnlabeled, &StripScopeAgent2::handleShowUnlabeled },
    { StripScopeAgent2::ShowInfered, &StripScopeAgent2::handleShowInfered },
    { StripScopeAgent2::ShowLinks, &StripScopeAgent2::handleShowLinks },
    { StripScopeAgent2::NextStrip, &StripScopeAgent2::handleNextStrip },
    { StripScopeAgent2::PrevStrip, &StripScopeAgent2::handlePrevStrip },
    { StripScopeAgent2::AutoGain, &StripScopeAgent2::handleAutoGain },
    { StripScopeAgent2::CursorSync, &StripScopeAgent2::handleCursorSync },
    { StripScopeAgent2::ReplaceStrip, &StripScopeAgent2::handleReplaceStrip },
    { StripScopeAgent2::EditAtts, &StripScopeAgent2::handleEditAtts },
    { StripScopeAgent2::CreateReport, &StripScopeAgent2::handleCreateReport },
    { StripScopeAgent2::SetTol, &StripScopeAgent2::handleSetTol },
    { StripScopeAgent2::SetWidth, &StripScopeAgent2::handleSetWidth },
    { StripScopeAgent2::SetCandidates, &StripScopeAgent2::handleSetCandidates },
    { StripScopeAgent2::ForceLabelSpin, &StripScopeAgent2::handleForceLabelSpin },
    { StripScopeAgent2::ShowFolded, &StripScopeAgent2::handleShowFolded },
    { StripScopeAgent2::UnlabeledStripMatch, &StripScopeAgent2::handleUnlabeledStripMatch },
    { StripScopeAgent2::StrictStripMatch, &StripScopeAgent2::handleStrictStripMatch },
    { StripScopeAgent2::UnassignStrip, &StripScopeAgent2::handleUnassignStrip },
    { StripScopeAgent2::LinkStrip, &StripScopeAgent2::handleLinkStrip },
    { StripScopeAgent2::AssignStrip, &StripScopeAgent2::handleAssignStrip },
    { StripScopeAgent2::AutoCenter, &StripScopeAgent2::handleAutoCenter },
    { StripScopeAgent2::LinkReference, &StripScopeAgent2::handleLinkReference },
    { StripScopeAgent2::UnlinkSucc, &StripScopeAgent2::handleUnlinkSucc },
    { StripScopeAgent2::UnlinkPred, &StripScopeAgent2::handleUnlinkPred },
    { StripScopeAgent2::GotoStrip, &StripScopeAgent2::handleGotoStrip },
    { StripScopeAgent2::HoldStrip, &StripScopeAgent2::handleHoldStrip },
    { StripScopeAgent2::SelectStrips, &StripScopeAgent2::handleSelectStrips },
    { StripScopeAgent2::ForwardStrip, &StripScopeAgent2::handleForwardStrip },
    { StripScopeAgent2::BackwardStrip, &StripScopeAgent2::handleBackwardStrip },
    { StripScopeAgent2::ShowAlignment, &StripScopeAgent2::handleShowAlignment },
    { StripScopeAgent2::LabelSpin, &StripScopeAgent2::handleLabelSpin },
    { StripScopeAgent2::DeleteSpins, &StripScopeAgent2::handleDeleteSpins },
    { StripScopeAgent2::PickSpin, &StripScopeAgent2::handlePickSpin },
    { StripScopeAgent2::MoveSpin, &StripScopeAgent2::handleMoveSpin },
    { StripScopeAgent2::MoveSpinAlias, &StripScopeAgent2::handleMoveSpinAlias },
    { StripScopeAgent2::FitWindow, &StripScopeAgent2::handleFitWindow },
    { StripScopeAgent2::ContourParams, &StripScopeAgent2::handleContourParams },
    { StripScopeAgent2::AutoContour, &StripScopeAgent2::handleAutoContour },
    { StripScopeAgent2::ShowLowRes, &StripScopeAgent2::handleShowLowRes },
    { StripScopeAgent2::SetResolution, &StripScopeAgent2::handleSetResolution },
ACTION_SLOTS_END( StripScopeAgent2 )

//////////////////////////////////////////////////////////////////////

#define MW ((Lexi::MainWindow*) getParent())->getQt()

StripScopeAgent2::StripScopeAgent2(Root::Agent* parent,Project* pro, Spectrum* spec):
	Root::Agent( parent ), d_lock( false ), 
	d_popLabel( 0 ), d_hold( false ), d_first( 0 ), d_curLane( -1 ),
	d_popStrip( 0 ), d_popSpec( 0 ), d_lane( -1 )
{

	assert( pro );
	d_pro = pro;
	d_pro->addObserver( this );
	assert( spec && spec->getDimCount() == 3 );

	d_spec = spec;
	d_orig = spec;
	bool doPath = d_spec->getKeyLabel( DimX ).isNull() || 
		d_spec->getKeyLabel( DimZ ).isNull();
	Rotation rot( 2 );
	rot[ DimX ] = spec->mapToType( DimX );
	rot[ DimY ] = spec->mapToType( DimZ );
	d_mdl2D = new AnchorSpace( pro->getSpins(),
		pro->getRepository()->getTypes(), rot, doPath );
	d_mdl2D->setSpecType( spec->getType() );
	d_src3D = new SpinPointSpace( pro->getSpins(), pro->getRepository()->getTypes(), 0, 
		true, true, false );
	d_src3D->setSpec( d_spec );
	d_mdl3D = new LinkFilterRotSpace( d_src3D );
	d_mdl3D->setSpec( d_spec );

	d_src3DHold = new SpinPointSpace( pro->getSpins(), pro->getRepository()->getTypes(), 0, 
		true, true, false );
	d_mdl3DHold = new LinkFilterRotSpace( d_src3DHold );

	d_cursor.assign( 3, 0 );

	d_query = new StripQuery2( d_mdl2D, d_pro->getMatcher() );
	d_query->addObserver( this );

	setSpec( spec );	// TODO
	initParams();
	buildPopup();
}

StripScopeAgent2::~StripScopeAgent2()
{
	d_pro->removeObserver( this );
	d_query->removeObserver( this );
	
	GlobalCursor::removeObserver( this );

	delete d_popLabel;
	delete d_popSpec;
	delete d_popStrip;
}

void StripScopeAgent2::updateSpecType()
{
	Rotation rot( 2 );
	rot[ DimX ] = d_spec->mapToType( DimX );
	rot[ DimY ] = d_spec->mapToType( DimZ );
	d_mdl2D->setSpecType( rot, d_spec->getType(), false );
}

void StripScopeAgent2::updateSpecPop()
{
	d_popSpec->purge();
	ColorMap a, b;
	d_spec->getColors( a );
	Project::SpecSet l;
	Spectrum* spec = 0;
	a[ DimY ] = AtomType(); // Joker

	const Project::SpectrumMap& sm = d_pro->getSpectra();
	Project::SpectrumMap::const_iterator p;
	Rotation rot;
	l.insert( d_orig );
	for( p = sm.begin(); p != sm.end(); ++p )
	{
		spec = (*p).second;
		if( spec->getDimCount() == 3 && spec->getId() != d_orig->getId() )
		{
			spec->getColors( b );
			if( a[ DimX ] == b[ DimX ] && a[ DimZ ] == b[ DimZ ] )
				l.insert( spec );
			else
			{
				if( SpectrumType::autoRotate( a, spec, rot, false ) ) // Keine Auflsungsoptimierung
					l.insert( new SpecRotator( spec, rot ) );
			}
		}
	}
	Project::SpecSet::const_iterator p1;
	Sort::const_iterator pp1;
	for( p1 = l.begin(); p1 != l.end(); ++p1 )
		d_sort[ (*p1)->getName() ] = (*p1);
	for( pp1 = d_sort.begin(); pp1 != d_sort.end(); ++pp1 )
	{
		Gui::Menu::item( d_popSpec, this, (*pp1).first.data(), StripScopeAgent2::SelectSpec, 
			true )->addParam( Root::Any( (*pp1).second ) );
	}
}

void StripScopeAgent2::buildPopup()
{
	d_popLabel = new Gui::Menu(); // Explizit lschen

	d_popSpec = new Gui::Menu();
	updateSpecPop();

	d_popStrip = new Gui::Menu();
	Gui::Menu::item( d_popStrip, this, "&Pick Spin", PickSpin, false );
	Gui::Menu::item( d_popStrip, this, "Pick Label...", PickLabel, false );
	Gui::Menu::item( d_popStrip, this, "Propose Spin...", ProposeSpin, false );
	Gui::Menu::item( d_popStrip, this, "&Move Spin", MoveSpin, false );
	Gui::Menu::item( d_popStrip, this, "&Move Spin Alias", MoveSpinAlias, false );
	d_popStrip->insertItem( "Label Spin", d_popLabel );
	Gui::Menu::item( d_popStrip, this, "&Force Spin Label", ForceLabelSpin, false );
	Gui::Menu::item( d_popStrip, this, "&Delete Spins", DeleteSpins, false );
	Gui::Menu::item( d_popStrip, this, "Delete Links", DeleteLinks, false );
	Gui::Menu* menuAtts = new Gui::Menu( d_popStrip );
	Gui::Menu::item( menuAtts, this, "Spin...", EditAtts, false );
	Gui::Menu::item( menuAtts, this, "System...", EditAttsSys, false );
	Gui::Menu::item( menuAtts, this, "Spin Link...", EditAttsLink, false );
	d_popStrip->insertItem( "&Edit Attributes", menuAtts );
	d_popStrip->insertSeparator();
	d_popStrip->insertItem( "Select Spectrum", d_popSpec );

	Gui::Menu* sub = new Gui::Menu( d_popStrip );
	d_popStrip->insertItem( "Select Strips", sub );
	Gui::Menu::item( sub, this, "Strict Strip Matching", StrictStripMatch, true );
	Gui::Menu::item( sub, this, "Unlabeled Strip Matching", UnlabeledStripMatch, true );
	sub->insertSeparator();
	Gui::Menu::item( sub, this, "Fit Selected Spins", StripScopeAgent2::SelectStrips, 
		true, Qt::CTRL+Qt::Key_F )->addParam( short( StripQuery2::SpinFit ) );

	Gui::Menu::item( sub, this, "Fit All Spins", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::AllBest ) );

	Gui::Menu::item( sub, this, "Select Manually...", StripScopeAgent2::SelectStrips, true, 
		Qt::CTRL+Qt::Key_M )->addParam( short( StripQuery2::Manual ) );

	Gui::Menu::item( sub, this, "All Strips", StripScopeAgent2::SelectStrips, true, 
		Qt::CTRL+Qt::Key_A )->addParam( short( StripQuery2::All ) );

	Gui::Menu::item( sub, this, "All Assigned Strips", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::AllAssigned ) );

	Gui::Menu::item( sub, this, "Unpicked Strips", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::UnpickedStrips ) );

	Gui::Menu::item( sub, this, "Unlinked Strips", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::UnlinkedStrips ) );

	Gui::Menu::item( sub, this, "Fragment", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::Fragment ) );

	Gui::Menu::item( sub, this, "Best free Successors", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::BestSucc ) );

	Gui::Menu::item( sub, this, "Best free Predecessors", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::BestPred ) );

	Gui::Menu::item( sub, this, "Best free Pred. & Succ.", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::BestFit ) );

	Gui::Menu::item( sub, this, "All best Successors", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::AllBestSucc ) );

	Gui::Menu::item( sub, this, "All best Predecessors", StripScopeAgent2::SelectStrips, 
		true )->addParam( short( StripQuery2::AllBestPred ) );

	Gui::Menu::item( d_popStrip, this, "Replace Strip...", ReplaceStrip, false );
	Gui::Menu::item( d_popStrip, this, "Next Page", ForwardStrip, false );
	Gui::Menu::item( d_popStrip, this, "Previous Page", BackwardStrip, false );
	d_popStrip->insertSeparator();
	Gui::Menu::item( d_popStrip, this, "Hold Reference Strip", HoldStrip, true );
	Gui::Menu::item( d_popStrip, this, "Hold Reference Spectrum", KeepSpec, true );
	Gui::Menu::item( d_popStrip, this, "Show Alignment...", ShowAlignment, false );
	Gui::Menu::item( d_popStrip, this, "Assig. Candidates...", SetCandidates, false );
	d_popStrip->insertSeparator();
	Gui::Menu::item( d_popStrip, this, "&Link to Reference", LinkReference, false );
	Gui::Menu::item( d_popStrip, this, "&Link Spin Systems...", LinkStrip, false );
	Gui::Menu::item( d_popStrip, this, "Unlink &Predecessor", UnlinkPred, false );
	Gui::Menu::item( d_popStrip, this, "Unlink &Successor", UnlinkSucc, false );
	d_popStrip->insertSeparator();
	Gui::Menu::item( d_popStrip, this, "Set Peak Width...", SetWidth, false );
	Gui::Menu::item( d_popStrip, this, "Set Peak Depth...", SetDepth, false );
	Gui::Menu::item( d_popStrip, this, "Set Spin Tolerance...", SetTol, false );
	Gui::Menu::item( d_popStrip, this, "Sync to List", SyncStrip, false );
	Gui::Menu::item( d_popStrip, this, "Fit Window", FitWindow, false, Qt::Key_Home );
}

void StripScopeAgent2::createStrip(int lane)
{
	d_strips[ lane ].d_viewer = new SpecViewer( 
		new ViewAreaMdl( true, true, false, true ), VIEWCOUNT );
	d_strips[ lane ].d_viewer->getViewArea()->addObserver( this );

	d_strips[ lane ].d_buf = new SpecBufferMdl( d_strips[ lane ].d_viewer->getViewArea() );
	d_strips[ lane ].d_buf->setFolding( d_folding );

	d_strips[ lane ].d_viewer->getViews()->replace( BACKGROUND, new Lexi::Background() );
	createContour( lane );

	d_strips[ lane ].d_viewer->getViews()->replace( INTENSITY, 
		new CenterLine( d_strips[ lane ].d_viewer ) );

	d_strips[ lane ].d_cur = new CursorMdl();
	d_strips[ lane ].d_cur->addObserver( this );
	d_strips[ lane ].d_tuples = new SpinPointView( d_strips[ lane ].d_viewer );
	d_strips[ lane ].d_tuples->setLabel( SpinPointView::SysOrResiTagAll, DimY );
	d_strips[ lane ].d_viewer->getViews()->replace( TUPLES, d_strips[ lane ].d_tuples );
	CursorView* cv = new CursorView( d_strips[ lane ].d_viewer, 
		d_strips[ lane ].d_cur, false, true );
	d_strips[ lane ].d_viewer->getViews()->replace( CURSORS, cv );
	d_strips[ lane ].d_mdl = new RangeFilterSpace( d_mdl3D, DimZ );
	d_strips[ lane ].d_tuples->setModel( d_strips[ lane ].d_mdl );
	if( d_folding )
		d_strips[ lane ].d_viewer->getViews()->replace( FOLDING, 
		new FoldingView( d_strips[ lane ].d_buf ) );
	else
		d_strips[ lane ].d_viewer->getViews()->replace( FOLDING, 0 );

	d_strips[ lane ].d_viewer->getHandlers()->append( new ZoomCtrl( d_strips[ lane ].d_viewer, false, true ) );
	d_strips[ lane ].d_viewer->getHandlers()->append( new SelectZoomCtrl( d_strips[ lane ].d_viewer, false, true ) );
	d_strips[ lane ].d_viewer->getHandlers()->append( new ScrollCtrl( d_strips[ lane ].d_viewer, false, true ) );
	d_strips[ lane ].d_viewer->getHandlers()->append( new CursorCtrl( cv, false, false, true ) );
	d_strips[ lane ].d_viewer->getHandlers()->append( new PointSelectCtrl( d_strips[ lane ].d_tuples, false ) );
	d_strips[ lane ].d_viewer->getHandlers()->append( new Lexi::ContextMenu( d_popStrip, false ) );
	d_strips[ lane ].d_viewer->getHandlers()->append( 
		new FocusCtrl( d_strips[ lane ].d_viewer, this ) );
}

void StripScopeAgent2::createSlice()
{
	SpecViewer* slice = new SpecViewer( 
		new ViewAreaMdl( false, true, false, true ) );
	d_slice.d_viewer = slice;
	slice->getViewArea()->addObserver( this );

	d_slice.d_buf = new SpecBufferMdl( slice->getViewArea() );
	d_slice.d_buf->setFolding( d_folding );
	d_slice.d_slice = new SliceView( d_slice.d_buf );
	slice->getViews()->append( d_slice.d_slice );

	d_slice.d_bufHold = new SpecBufferMdl( slice->getViewArea() );
	d_slice.d_bufHold->setFolding( d_folding );
	d_slice.d_sliceHold = new SliceView( d_slice.d_bufHold, g_clrHold );
	slice->getViews()->append( d_slice.d_sliceHold );

	d_slice.d_cur = new CursorMdl();
	d_slice.d_cur->addObserver( this );
	CursorView* cv = new CursorView( slice, d_slice.d_cur );
	slice->getViews()->append( cv );

	slice->getHandlers()->append( new ZoomCtrl( slice, false, true ) );
	slice->getHandlers()->append( new SelectZoomCtrl( slice, false, true ) );
	slice->getHandlers()->append( new ScrollCtrl( slice ) );
	slice->getHandlers()->append( new SelectRulerCtr( slice, true ) );
	slice->getHandlers()->append( new CursorCtrl( cv, false ) );
	slice->getHandlers()->append( new FocusCtrl( slice ) );
}

void StripScopeAgent2::initParams()
{
	d_resol = 1;
	d_lowResol = false;
	d_autoContour = true;
	d_autoCenter = true;
	d_contourFactor = 1.4f;
	d_contourOption = ContourView::Both;
	d_folding = false;
	d_cursorSync = false;
	d_gain = 2.0;
	d_keep = false;
	// TODO: diese Werte sollen ab Konfigurations-Record gelesen werden
}

void StripScopeAgent2::initViews( int count, bool slice )
{
	assert( count > 0 );
	d_strips.clear();
	d_curLane = -1;
	d_strips.assign( count, StripSocket() );
	for( int i = 0; i < d_strips.size(); i++ )
	{
		createStrip( i );
	}
	if( slice )
		createSlice();
	fillPage(true);
}

void StripScopeAgent2::handle(Root::Message& msg)
{
	if( d_lock )
		return;
	d_lock = true;
	BEGIN_HANDLER();
	MESSAGE( ViewAreaMdl::Update, a, msg )
	{
		Lexi::Viewport::pushHourglass();
		if( d_slice.d_viewer->getViewArea() == a->getOrigin() )
			updateSlice( a );
		else
			for( int d = 0; d < d_strips.size(); d++ )
				if( d_strips[ d ].d_viewer->getViewArea() == a->getOrigin() )
				{
					updateStrip( d, a );
					break;
				}

		Lexi::Viewport::popCursor();
		msg.consume();
	}
	MESSAGE( CursorMdl::Update, a, msg )
	{
		if( d_slice.d_cur == a->getOrigin() )
			updateSlice( a );
		else if( a->getDim() == DimY )
			for( int d = 0; d < d_strips.size(); d++ )
				if( d_strips[ d ].d_cur == a->getOrigin() )
				{
					updateStrip( d, a );
					break;
				}
		msg.consume();
	}
	MESSAGE( Project::Changed, a, msg )
	{
		msg.consume();
		if( a->d_hint == Project::Width ||
			a->d_hint == Project::WidthFactor )
		{
			fillPage( false );
		}
	}
	MESSAGE( SpectrumPeer::Added, a, msg )
	{
        Q_UNUSED(a)
		updateSpecPop();
	}
	MESSAGE( SpectrumPeer::Removed, a, msg )
	{
        Q_UNUSED(a)
		updateSpecPop();
	}
	MESSAGE( StripQuery2::Update, e, msg )
	{
		msg.consume();
		switch( e->getType() )
		{
		case StripQuery2::Update::ResultSet:
			{
				fillPage( false );
				QString str;
				str.sprintf( "%d Strips Selected", d_query->getResult().size() );
				Lexi::ShowStatusMessage msg( str );
				traverseUp( msg );
			}
			break;
		case StripQuery2::Update::UpdateAt:
		case StripQuery2::Update::DeleteAt:
			if( e->getPos() == -1 || ( e->getPos() >= d_first && 
				e->getPos() < d_first + d_strips.size() ) )
			{
				fillPage( false );
			}
			break;
		}
	}
	MESSAGE( Root::Action, a, msg )
	{
		d_lock = false; // Kein Blocking fr Action-Ausfhrung
		EXECUTE_ACTION( StripScopeAgent2, *a );
	}
	MESSAGE( FocusCtrl::FocusIn, a, msg )
	{
		d_lane = -1;
		for( int l = 0; l < d_strips.size(); l++ )
		{
			if( d_strips[ l ].d_viewer == a->getViewer() )
			{
				d_lane = l;
				break;
			}
		}
		if( d_lane != -1 && d_strips[ d_lane ].d_spec )
		{
			d_curLane = d_lane;
			d_cursor[ DimX ] = d_strips[ d_lane ].d_spec->getOrigin()[ DimX ];
			d_cursor[ DimZ ] = d_strips[ d_lane ].d_spec->getOrigin()[ DimZ ];
			d_slice.d_spec->setOrigin( d_cursor );
		}else
			d_curLane = -1;
		msg.consume();
	}
	MESSAGE( GlobalCursor::UpdatePos, a, msg )
	{
		d_lock = false;
		d_cursorSync = false;
		if( !d_strips.empty() && ( a->getDim() == DimY || a->getDim() == DimUndefined ) &&
			d_strips[0].d_spec->getColor( DimY ) == a->getTy() )
		{
			d_strips[0].d_cur->setCursor( Dimension( DimY ), a->getY() );
		}
		d_cursorSync = true;
		msg.consume();
	}
	END_HANDLER();
	d_lock = false;
}

void StripScopeAgent2::updateStrip(int lane, ViewAreaMdl::Update *msg)
{
	// In einem Strip wurde Ausschnitt gendert
	if( msg->getType() != ViewAreaMdl::Update::Range )
		return;

	for( int l = 0; l < d_strips.size(); l++ )
		if( l != lane )
		{
			d_strips[ l ].d_viewer->getViewArea()->setRange( DimY, 
				d_strips[ lane ].d_viewer->getViewArea()->getRange( DimY ) );
			d_strips[ l ].d_viewer->redraw();
		}

	d_slice.d_viewer->getViewArea()->setRange( DimY, 
		d_strips[ lane ].d_viewer->getViewArea()->getRange( DimY ) );
	d_slice.d_viewer->redraw();
}

void StripScopeAgent2::updateStrip(int lane, CursorMdl::Update *msg)
{
	// In einem Strip wurde Cursor gendert

	PPM pos = msg->getY();

	bool change = d_curLane != lane;
	if( d_strips[ lane ].d_spec != 0 )
	{
		d_curLane = lane;
		if( change )
		{
			d_cursor[ DimX ] = d_strips[ lane ].d_spec->getOrigin()[ DimX ];
			d_cursor[ DimZ ] = d_strips[ lane ].d_spec->getOrigin()[ DimZ ];
		}
	}else
		d_curLane = -1;

	if( d_curLane >= 0 && d_autoCenter )
	{
		SpinPoint tuple = 
			d_strips[ lane ].d_tuples->getHit( d_cursor[ DimX ], pos );
		if( !tuple.isZero() )
		{
			pos = tuple[ DimY ]->getShift( d_spec ); 
			msg->override( pos, pos ); 
		}
	}

	d_cursor[ DimY ] = pos;
	if( d_cursorSync )
		GlobalCursor::setCursor( DimY, pos, d_spec->getColor( DimY ) );
	for( int l = 0; l < d_strips.size(); l++ )
		if( l != lane )
		{
			d_strips[ l ].d_cur->setCursor( (Dimension)DimY, d_cursor[ DimY ] );
		}
	d_slice.d_cur->setCursor( (Dimension)DimY, d_cursor[ DimY ] );
	if( change )
		d_slice.d_spec->setOrigin( d_cursor );
	notifyCursor();
}

void StripScopeAgent2::updateSlice( ViewAreaMdl::Update *msg)
{
	if( msg->getType() != ViewAreaMdl::Update::Range )
		return;

	for( int l = 0; l < d_strips.size(); l++ )
	{
		d_strips[ l ].d_viewer->getViewArea()->setRange( DimY, 
			d_slice.d_viewer->getViewArea()->getRange( DimY ) );
		d_strips[ l ].d_viewer->redraw();
	}
}

void StripScopeAgent2::updateSlice( CursorMdl::Update *msg)
{
	d_cursor[ DimY ] = msg->getX();
	if( d_cursorSync )
		GlobalCursor::setCursor( (Dimension)DimY, msg->getY(), d_spec->getColor( DimY ) );
	for( int l = 0; l < d_strips.size(); l++ )
		d_strips[ l ].d_cur->setCursor( (Dimension)DimY, d_cursor[ DimY ] );
	notifyCursor();
}

bool StripScopeAgent2::findSystem(Index id, SpinPoint & tuple)
{
	SpinSystem* sys = d_pro->getSpins()->getSystem( id );
	if( sys == 0 )
	{
		Lexi::ShowStatusMessage msg( "Unknown spin system" );
		traverseUp( msg );
		return false;
	}
	SpinSpace::Result tuples;
	d_mdl2D->find( tuples, sys );
	if( tuples.empty() )
	{
		Lexi::ShowStatusMessage msg( "System not represented in spectrum" );
		traverseUp( msg );
		return false;
	}else if( !tuples.hasOne() )
	{
		tuple = Dlg::selectTuple( MW, tuples, 2, "Select Spin Tuple" );
		if( tuple.isZero() )
			return false;
	}else
		tuple = tuples.first().d_point;
	return true;
}

void StripScopeAgent2::stepSpec(bool next)
{
	if( d_sort.size() < 2 )
		return;
	Sort::const_iterator p = d_sort.find( d_spec->getName() );
	if( p == d_sort.end() )
	{
		if( next )
			p = d_sort.begin();
		else
			--p;
	}else
	{
		if( next )
		{
			++p;
			if( p == d_sort.end() )
				p = d_sort.begin();
		}else if( p == d_sort.begin() )
		{
			p = d_sort.end();
			--p;
		}else
			--p;
	}
	assert( p != d_sort.end() );
	Lexi::Viewport::pushHourglass();
	setSpec( (*p).second );
	Lexi::Viewport::popCursor();
}

void StripScopeAgent2::setSpec(Spectrum * spec)
{
	//if( d_spec == spec )
	//	return;
	assert( spec );
	Spectrum* old = d_spec;
	d_spec = spec;
	d_query->setSpec( spec, false );

	if( d_popLabel )
	{
		d_mdl3D->setSpec( 0 );
		d_src3D->setSpec( spec );
		d_mdl3D->setSpec( d_spec );

		d_popLabel->purge();
		Gui::Menu::item( d_popLabel, this, "?", LabelSpin, true )->addParam( Root::Any() );
		Gui::Menu::item( d_popLabel, this, "?-1", LabelSpin, true )->addParam( new SpinLabelHolder( "?-1" ) );

		NmrExperiment* e = d_pro->getRepository()->getTypes()->inferExperiment3( d_spec->getType(), d_src3D->doPathsim() );
		assert( e );
		SpinLabelSet l;
		e->getColumn( d_spec->mapToType( DimY ), l );
		typedef std::map<QByteArray ,SpinLabel> Sort;
		Sort sort;
		SpinLabelSet::const_iterator p;
		for( p = l.begin(); p != l.end(); ++p )
			sort[ (*p).data() ] = (*p);
		Sort::const_iterator pp1;
		for( pp1 = sort.begin(); pp1 != sort.end(); ++pp1 )
		{
			Gui::Menu::item( d_popLabel, this, (*pp1).first.data(), 
				LabelSpin, true )->addParam( Root::Any( new SpinLabelHolder( (*pp1).second ) ) );
		}
	}

	fillPage( old == 0 || old->getColor( DimY ) != spec->getColor( DimY ) ||
		!old->getScale( DimY ).getRange().intersects( spec->getScale( DimY ).getRange() ) );
}

static void _allocate( PpmRange& r, PPM point, PPM width ) // TODO: Bug Release-Mode
{
	if( r.first <= r.second )
	{
		r.first = point - width / 2.0;
		r.second = point + width / 2.0;
	}else
	{
		r.first = point + width / 2.0;
		r.second = point - width / 2.0;
	}
}

void StripScopeAgent2::fillStrip( int i, const PpmPoint& a, 
		const SpinPoint& strip, bool hold )
{
	Spectrum* spec = d_spec;
	if( d_keep && hold )
	{
		spec = d_holdSpec;
		d_strips[ i ].d_mdl->setHost( d_mdl3DHold );
	}else
	{
		d_strips[ i ].d_mdl->setHost( d_mdl3D );
	}

	// SpecProj erstellen bei Anchors und an Strips zuweisen
	d_strips[ i ].d_spec = new SpecProjector( spec, DimX, DimY, a );
	d_strips[ i ].d_buf->setSpectrum( d_strips[ i ].d_spec );
	PpmRange rx = spec->getScale( DimX ).getRange();
	PPM w = d_pro->inferStripWidth( DimX, spec );
	_allocate( rx, a[ DimX ], w );
	d_strips[ i ].d_viewer->getViewArea()->setRange( DimX, rx );
	// Strip setzen
	d_strips[ i ].d_mdl->setSys( strip[ DimX ]->getSystem() );
    d_strips[ i ].d_mdl->setOrigThick( a[ DimZ ], spec->getScale( DimZ ).getDelta(), true ); // RISK
	w = d_pro->inferPeakWidth( DimZ, spec );
    d_strips[ i ].d_mdl->setGhostWidth( w );

	d_strips[ i ].d_tuple = strip;
	d_strips[ i ].d_tuples->setModel( d_strips[ i ].d_mdl );

	char buf[32];
	SpinPointView::formatLabel( buf, sizeof(buf), strip, 
		SpinPointView::PairIdLabelSysOrResi, DimY );

	d_strips[ i ].d_viewer->getViews()->replace( LABEL2,
		new Lexi::Label( buf, nil, (i==0 && hold)?g_clrHold:g_clrLabel, 
		Lexi::AlignLeft, Lexi::AlignBottom ) );

	if( i == 0 || ( i == 1 && d_keep && d_hold ) )
	{
		d_strips[ i ].d_viewer->getViews()->replace( LABEL1,
			new Lexi::Label( d_strips[ i ].d_spec->getName(), 
			nil, (hold)?g_clrHold:g_clrLabel, 
			Lexi::AlignLeft, Lexi::AlignTop ) );
	}else
		d_strips[ i ].d_viewer->getViews()->replace( LABEL1, 0 );

}

void StripScopeAgent2::fillPage(bool autoFit )
{
	Index offset = d_first;
	const StripQuery2::Result& strips = d_query->getResult();
	SpinPoint hold = d_query->getReference();
	SpinPoint strip;
	Index i;
	d_hold = false;
	PpmPoint a;
	a.assign( d_spec->getDimCount(), 0 );
	PpmRange rx;
	PpmRange ry = d_spec->getScale( DimY ).getRange();	// TODO: Y-Range konservieren
	ry.invert();
	QString str;

	for( i = 0; i + offset < strips.size() && i < d_strips.size(); i++ )
	{
		if( i == 0 && !hold.isZero() )	// Eine Referenz muss angezeigt werden.
		{
			if( strips.size() > 1 )
				offset--;
			strip = hold;
			d_hold = true;
		}else if( hold == strips[ i + offset ] ) // Der Strip wird bereits als Referenz angezeigt.
		{
			offset++;
			if( i + offset >= strips.size() )
				break;
			strip = strips[ i + offset ];
		}else
			strip = strips[ i + offset ];

		// Anchor berechnen und zu 3D ausdehnen
		a[ DimX ] = strip[ DimX ]->getShift( d_spec );
		a[ DimZ ] = strip[ DimY ]->getShift( d_spec );

		fillStrip( i, a, strip, strip == hold );

		if( autoFit )
			d_strips[ i ].d_viewer->getViewArea()->setRange( DimY, ry );
		d_strips[ i ].d_viewer->redraw();
	}
	while( i < d_strips.size() )
	{
		d_strips[ i ].d_spec = 0;
		d_strips[ i ].d_buf->setSpectrum( 0 );
		d_strips[ i ].d_tuples->setModel( 0 );
		d_strips[ i ].d_viewer->getViews()->replace( LABEL2, 0 );
		d_strips[ i ].d_viewer->getViews()->replace( LABEL1, 0 );
		i++;
	}
	// Slices anpassen
	if( d_slice.d_buf )
	{
		d_slice.d_spec = new SpecProjector( d_spec, DimY );
		d_slice.d_spec->setOrigin( d_cursor );
		d_slice.d_buf->setSpectrum( d_slice.d_spec );
		if( d_hold )
		{
			Spectrum* spec = d_spec;
			if( d_keep )
				spec = d_holdSpec;
			d_slice.d_specHold = new SpecProjector( spec, DimY );
			a[ DimX ] = hold[ DimX ]->getShift( spec );
			a[ DimZ ] = hold[ DimY ]->getShift( spec );
			d_slice.d_specHold->setOrigin( a );
			d_slice.d_bufHold->setSpectrum( d_slice.d_specHold );
		}else
		{
			d_slice.d_specHold = 0;
			d_slice.d_bufHold->setSpectrum( 0 );
		}
		if( autoFit )
			d_slice.d_viewer->getViewArea()->setRange( DimY, ry );
		d_slice.d_viewer->redraw();
	}
}

void StripScopeAgent2::notifyCursor()
{
	QString str;
    QTextStream ts( &str, QIODevice::WriteOnly );

	ts.setf( QTextStream::fixed );
	ts.precision( 3 );

	if( d_spec )
	{
		ts << d_spec->getName();
		ts <<  "  Cursor:  ";

		for( Dimension d = 0; d < 3; d++ )
		{
			ts << char( 'x' + d ) << ": ";
			ts << d_spec->getDimName( d ) << "=";
			ts << d_cursor[ d ];
			if( d_folding )
				ts << " (" << d_spec->getScale( d ).getRangeOffset( 
					d_cursor[ d ] ) << ")  ";
			else
				ts << "  ";
		}

		try
		{
			Amplitude val = d_spec->getAt( d_cursor, d_folding, d_folding );
			ts.setf( QTextStream::showpos );
			ts.precision( 0 );
			ts << "Level=" << val;
		}catch( ... )
		{
			ts << " Out of Spectrum";
		}
	}else
		ts << "No Spectrum";
	// ts << "No Current Strip";
	QByteArray  tmp;
	if( d_curLane != -1 && d_strips[ d_curLane ].d_tuples->
		formatSelection( tmp, SpinPointView::PairAll, 3 ) )
	{
		str += ",  ";
		str += tmp.data();
	}
	Lexi::ShowStatusMessage msg( str );
	traverseUp( msg );
}

bool StripScopeAgent2::findTuple(SpinSystem *sys, Spin *spin, SpinPoint& tuple )
{
	const Dimension dim = 2;
	SpinSpace::Result tuples;
	if( spin )
		d_mdl2D->find( tuples, spin );
	else
		d_mdl2D->find( tuples, sys );
	if( tuples.empty() )
	{
		Lexi::ShowStatusMessage msg( "Element not found in inferred base" );
		traverseUp( msg );
		return false;
	}
	Lexi::ShowStatusMessage msg( "One or more elements found in inferred base" );
	traverseUp( msg );
	if( tuples.hasOne() )
	{
		tuple = tuples.first().d_point;
	}else
	{
		tuple = Dlg::selectTuple( MW, tuples, dim, "Select Spin Tuple" );
		if( tuple.isZero() )
			return false;
	}
	return true;
}

void StripScopeAgent2::gotoTuple(SpinSystem *sys, Spin *spin)
{
	SpinPoint tuple;
	if( !findTuple( sys, spin, tuple ) )
		return;
	int focus = d_query->getIndex( tuple );
	if( focus == -1 )
	{
		StripQuery2::Result f;
		if( !d_query->getReference().isZero() )
			f.push_back( d_query->getReference() );
		f.push_back( tuple );
		d_first = 0;
		d_query->setManual( f, true );
	}else
		d_first = focus;
	fillPage( false );
}

void StripScopeAgent2::createContour(int i)
{
	ContourView* v = new ContourView( d_strips[ i ].d_buf, d_autoContour );
	d_strips[ i ].d_viewer->getViews()->replace( CONTOUR, v );
	if( d_autoContour )
	{
		v->createLevelsAuto( d_contourFactor, d_contourOption, d_gain );
	}else
		v->createLevelsMin( d_contourFactor, d_spec->getThreshold(), d_contourOption );
}

void StripScopeAgent2::handleContourParams(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	Dlg::ContourParams p;
	p.d_factor = d_contourFactor;
	p.d_threshold =	d_spec->getThreshold();
	p.d_option = d_contourOption;
	if( Dlg::setParams( MW, p ) )
	{
		d_spec->setThreshold( p.d_threshold );
		d_contourOption = p.d_option;
		d_contourFactor = p.d_factor;
		d_autoContour = false;
		for( int i = 0; i < d_strips.size(); i++ )
		{
			createContour( i );
			d_strips[ i ].d_viewer->redraw();
		}
	}
}

void StripScopeAgent2::handleAutoContour(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_autoContour );
	
	d_autoContour = !d_autoContour;
	for( int i = 0; i < d_strips.size(); i++ )
	{
		createContour( i );
		d_strips[ i ].d_viewer->redraw();
	}
}

void StripScopeAgent2::handleFitWindow(Action & a)
{
	ACTION_ENABLED_IF( a, true ); 

	PpmRange r = d_spec->getScale( DimY ).getRange();	
	r.invert();
	d_slice.d_viewer->getViewArea()->setRange( DimY, r );
	d_slice.d_viewer->redraw();
	for( int i = 0; i < d_strips.size(); i++ )
	{
		d_strips[ i ].d_viewer->getViewArea()->setRange( DimY, r );
		d_strips[ i ].d_viewer->redraw();
	}
}

void StripScopeAgent2::handleDeleteSpins(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		!d_strips[ d_curLane ].d_tuples->getSel().empty() &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPointView::Selection sel = d_strips[ d_curLane ].d_tuples->getSel();

	SpinPointView::Selection::const_iterator p;
	Root::Ref<Root::MakroCommand> cmd = new Root::MakroCommand( "Delete Spins" );
	std::set<Spin*> test;
	for( p = sel.begin(); p != sel.end(); ++p )
	{
		// qDebug( "Deleting adr. %x id %d", (*p)[ DimY ], (*p)[ DimY ]->getId() );
		if( test.count( (*p)[ DimY ] ) == 0 )
		{
			cmd->add( new DeleteSpinCmd( d_pro->getSpins(), (*p)[ DimY ] ) );
			test.insert( (*p)[ DimY ] );
		}
	}
	cmd->handle( this );
}

void StripScopeAgent2::handlePickSpin(Action & a)
{
	if( !d_src3D->showNulls() )
	{
		handlePickLabel( a );
		return;
	}
	ACTION_ENABLED_IF( a, d_curLane != -1 );

	Root::Ref<PickSystemSpinCmd> cmd = new PickSystemSpinCmd( d_pro->getSpins(), 
		d_strips[ d_curLane ].d_tuple[0]->getSystem(), 
		d_spec->getColor( DimY ), d_cursor[ DimY ], 0 ); 
		// Pick generisches Spektrum
	cmd->handle( this );
	d_strips[ d_curLane ].d_tuples->selectPeak( d_cursor[ DimX ], d_cursor[ DimY ] );
}

void StripScopeAgent2::handleMoveSpin(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();
	Root::Ref<MoveSpinCmd> cmd =
		new MoveSpinCmd( d_pro->getSpins(), tuple[ DimY ], d_cursor[ DimY ], 0 );
		// Move generisches Spektrum
	cmd->handle( this );
}

void StripScopeAgent2::handleMoveSpinAlias(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();
	Root::Ref<MoveSpinCmd> cmd =
		new MoveSpinCmd( d_pro->getSpins(), tuple[ DimY ], d_cursor[ DimY ], d_spec );
		// Move generisches Spektrum
	cmd->handle( this );
}

void StripScopeAgent2::handleLabelSpin(Action & a)
{
	if( d_curLane == -1 )
		return;
	SpinLabelHolder* lh = dynamic_cast<SpinLabelHolder*>( a.getParam( 0 ).getObject() );
	SpinLabel l;
	if( lh )
		l = lh->d_label;
	ACTION_ENABLED_IF( a, d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() &&
		d_strips[ d_curLane ].d_tuple[ DimX ]->getSystem()->isAcceptable( l ) );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();
	Root::Ref<LabelSpinCmd> cmd =
		new LabelSpinCmd( d_pro->getSpins(), tuple[ DimY ], l );
	cmd->handle( this );
	if( l.isNull() )
		d_src3D->showNulls( true );
}

void StripScopeAgent2::handleForceLabelSpin(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();

	bool ok	= FALSE;
	QString res;
	if( a.getParam( 0 ).isNull() )
	{
		res.sprintf( "Please enter a valid label (%s):", SpinLabel::s_syntax );
		res = QInputDialog::getText( "Force Spin Label", res, QLineEdit::Normal, 
			tuple[ DimY ]->getLabel().data(), &ok, MW );
		if( !ok )
			return;
	}else
		res = a.getParam( 0 ).getCStr();

	SpinLabel l;
	if( !SpinLabel::parse( res, l ) )
	{
		Root::ReportToUser::alert( this, "Force Spin Label", "Invalid spin label syntax!" );
		return;
	}

	Root::Ref<LabelSpinCmd> cmd =
		new LabelSpinCmd( d_pro->getSpins(), tuple[ DimY ], l );
	cmd->handle( this );
	if( l.isNull() )
		d_src3D->showNulls( true );
}

void StripScopeAgent2::handleShowAlignment(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 );
	
	SpinSystemString fra;
	if( d_hold && d_curLane > 0 )
	{
		SpinSystem* hold = d_strips[ 0 ].d_tuple[0]->getSystem();
		SpinSystem* cur = d_strips[ d_curLane ].d_tuple[0]->getSystem();
		SpinMatcher::Fit t;
		d_pro->getMatcher()->getStripMatch( hold, cur, t );
		Fitness ab = t.getStripFitness();
		d_pro->getMatcher()->getStripMatch( cur, hold, t );
		Fitness ba = t.getStripFitness();
		if( ab >= ba )
			d_pro->getSpins()->fillString( hold, cur, fra );
		else
			d_pro->getSpins()->fillString( cur, hold, fra );
	}else
		d_pro->getSpins()->fillString( 
			d_strips[ d_curLane ].d_tuple[0]->getSystem(), fra );

	FragmentAssignment* f = new FragmentAssignment( d_pro->getSpins(),
		d_pro->getMatcher(), fra );
	SingleAlignmentView* v = new SingleAlignmentView( this, f );
	v->show();
}

void StripScopeAgent2::handleSetResolution(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	bool ok	= FALSE;
	int res	= QInputDialog::getInteger( "Set Resolution", 
		"Please	enter the minimal number of pixels per sample:", 
		d_resol, 1, 20, 1,	&ok, MW );
	if( ok )
	{
		d_resol = res;
		d_lowResol = true;
		Viewport::pushHourglass();
		for( int i = 0; i < d_strips.size(); i++ )
		{
			d_strips[ i ].d_buf->setResolution( d_resol );
			d_strips[ i ].d_viewer->redraw();
		}
		Viewport::popCursor();
	}
}

void StripScopeAgent2::handleShowLowRes(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_lowResol );

	Viewport::pushHourglass();
	d_lowResol = !d_lowResol;
	if( d_lowResol )
	{
		for( int i = 0; i < d_strips.size(); i++ )
		{
			d_strips[ i ].d_buf->setResolution( d_resol );
			d_strips[ i ].d_viewer->redraw();
		}
	}else
	{
		for( int i = 0; i < d_strips.size(); i++ )
		{
			d_strips[ i ].d_buf->setScaling( false );
			d_strips[ i ].d_viewer->redraw();
		}
	}
	Viewport::popCursor();
}

void StripScopeAgent2::handleForwardStrip(Action & a)
{
	Index amount = d_strips.size();
	ACTION_ENABLED_IF( a, d_first < Index( d_query->getResult().size() - amount ) );

	d_curLane = -1;
	if( !d_query->getReference().isZero() )
		amount--;
	d_first += amount;
	if( d_first >= d_query->getResult().size() )
		d_first -= amount;
	if( d_first < 0 )
		d_first = 0;
	fillPage( false );
}

void StripScopeAgent2::handleBackwardStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_first > 0 );

	d_curLane = -1;
	d_first -= d_strips.size();
	if( d_first < 0 )
		d_first = 0;
	fillPage( false );
}

void StripScopeAgent2::handleSelectStrips(Action & a)
{
	if( a.getParamCount() == 0 )
		return;

	StripQuery2::Query q = (StripQuery2::Query) a.getParam( 0 ).getShort();

	int c = 0;
	if( !d_strips.empty() )
		c = d_strips[ 0 ].d_tuples->getSel().size();

	ACTION_CHECKED_IF( a, ( ( d_curLane != -1 )?
		d_query->canQuery( q, d_strips[ d_curLane ].d_tuple ):
		d_query->canQuery( q ) ) || 
		( d_hold && q == StripQuery2::SpinFit && c != 0 ),
		d_query->getQuery() == q );
	
	d_first = 0; // RISK
	if( q == StripQuery2::Manual )
	{
		bool ok;
		QString seq	= QInputDialog::getText( "Select Strips Manually", 
			"Please	enter a sequence of system numbers:", QLineEdit::Normal, 
			"", &ok, MW );
		if( !ok )
			return;
		QTextIStream in( &seq );
		StripQuery2::Result f;
		SpinSystem* sys;
		QString id;
		SpinSpace::Result tuples;
		SpinSpace::Result::const_iterator i;
		while( !in.atEnd() )
		{
			in >> id;
			sys = d_pro->getSpins()->findSystem( id );
			if( sys )
			{
				d_mdl2D->find( tuples, sys );
				if( tuples.empty() )
				{
					Lexi::ShowStatusMessage msg( 
						"Select Strips: System not represented in spectrum" );
					traverseUp( msg );
					return;
				}
				for( i = tuples.begin(); i != tuples.end(); ++i )
					f.push_back( (*i).d_point );
			}else
			{
				Lexi::ShowStatusMessage msg( "Select Strips: strip not found" );
				traverseUp( msg );
				return;
			}
		}
		d_query->setManual( f, d_mdl2D->doPathsim() );
	}else if( q == StripQuery2::SpinFit && !d_strips.empty() && d_hold )
	{
		StripQuery2::SpinList spins( c );
		SpinPointView::Selection sel = d_strips[ 0 ].d_tuples->getSel();
		SpinPointView::Selection::const_iterator i;
		int j = 0;
		for( i = sel.begin(); i != sel.end(); ++i, ++j )
			spins[ j ] = (*i)[ DimY ];
		updateSpecType();
		d_query->query( q, spins );
		d_strips[ 0 ].d_tuples->select( sel );
	}else if( d_curLane != -1 )
	{
		updateSpecType();
		d_query->query( q, d_strips[ d_curLane ].d_tuple );
	}else
	{
		updateSpecType();
		d_query->query( q );
	}
}

void StripScopeAgent2::handleHoldStrip(Action & a)
{
	ACTION_CHECKED_IF( a, 
		d_hold || !a.getParam( 0 ).isNull() || d_curLane != -1, d_hold );
	
	if( d_hold && a.getParam( 0 ).isNull() )
		d_query->setReference( SpinPoint(), false );
	else if( a.getParam( 0 ).isNull() )
	{
		if( d_curLane == -1 )
		{
			Lexi::ShowStatusMessage msg( "Hold Strips: No Current Strip" );
			traverseUp( msg );
			return;
		}
		d_query->setReference( d_strips[ d_curLane ].d_tuple, false );
		AdjustSelection m2( d_strips[ d_curLane ].d_tuple );
		traverseUp( m2 );
	}else
	{
		SpinPoint tuple;
		if( !findSystem( a.getParam( 0 ).getLong(), tuple ) )
			return;
		d_query->setReference( tuple, false );
		AdjustSelection m2( tuple );
		traverseUp( m2 );
	}
	fillPage( false );
}

void StripScopeAgent2::handleGotoStrip(Action & a)
{
	ACTION_ENABLED_IF( a, true );
	
	if( a.getParam( 0 ).isNull() )
	{
		bool ok	= FALSE;
		int res	= QInputDialog::getInteger( "Goto System", 
			"Please	enter the spin system id to display:", 
			0, 1, 99999999, 1, &ok, MW );
		if( ok )
		{
			SpinPoint tuple;
			if( !findSystem( res, tuple ) )
				return;

			int focus = d_query->getIndex( tuple );
			if( focus == -1 )
			{
				StripQuery2::Result f;
				if( !d_query->getReference().isZero() )
					f.push_back( d_query->getReference() );
				f.push_back( tuple );
				d_first = 0;
				d_query->setManual( f, true );
			}else
				d_first = focus;
		}else
			return;
	}else
	{
		SpinPoint tuple;
		if( !findSystem( a.getParam( 0 ).getLong(), tuple ) )
			return;
		int focus = d_query->getIndex( tuple );
		if( focus == -1 )
		{
			Lexi::ShowStatusMessage msg(  "Goto System: system not known or not part of current selection" );
			traverseUp( msg );
			return;
		}
		d_first = focus;
	}
	fillPage( false );
}

void StripScopeAgent2::handleLinkReference(Action & a)
{
	ACTION_ENABLED_IF( a, d_hold && d_curLane > 0 );

	SpinSystem* ref = d_strips[ 0 ].d_tuple[0]->getSystem();
	SpinSystem* cur = d_strips[ d_curLane ].d_tuple[0]->getSystem();

	QString str;
	if( d_query->isPred() )
	{
		SpinSystem* tmp = cur;
		cur = ref;
		ref = tmp;
	}else if( !d_query->isSucc() )
	{
		Dlg::StringList l( 2 );
		str.sprintf( "Pred. = %d / Succ.= %d", ref->getId(), cur->getId() );
		l[ 0 ] = str.toLatin1().data();
		str.sprintf( "Pred. = %d / Succ. = %d", cur->getId(), ref->getId() );
		l[ 1 ] = str.toLatin1().data();
		int res = Dlg::getOption( MW,
			l, "Select Direction" );
		if( res == -1 )
			return;
		else if( res == 1 )
		{
			SpinSystem* tmp = cur;
			cur = ref;
			ref = tmp;
		}
	}

	try
	{
		Root::Ref<LinkSystemCmd> cmd =
			new LinkSystemCmd( d_pro->getSpins(), ref, cur );
		if( cmd->handle( this ) )
		{
			str.sprintf( "Linked %d - %d", ref->getId(), cur->getId() );
			Lexi::ShowStatusMessage msg(  str );
			traverseUp( msg );
			return;
		}else
		{
			Lexi::ShowStatusMessage msg(  "Link to reference: unsuccessful" );
			traverseUp( msg );
			return;
		}
	}catch( Exception& e )
	{
		Root::ReportToUser::alert( this, "Link to Reference", e.what() );
	}
}

void StripScopeAgent2::handleUnlinkSucc(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuple[0]->getSystem()->getSucc() != 0 );

	SpinSystem* other = d_strips[ d_curLane ].d_tuple[0]->getSystem()->getSucc();
	Root::Ref<UnlinkSystemCmd> cmd =
		new UnlinkSystemCmd( d_pro->getSpins(), 
			d_strips[ d_curLane ].d_tuple[0]->getSystem(), other );
	if( cmd->handle( this ) )
	{
		QString str;
		str.sprintf( "Unlinked %d - %d", 
			d_strips[ d_curLane ].d_tuple[0]->getSystem()->getId(), other->getId() );
		Lexi::ShowStatusMessage msg(  str );
		traverseUp( msg );
		return;
	}else
	{
		Lexi::ShowStatusMessage msg(  "Unlink successor: unsuccessful" );
		traverseUp( msg );
		return;
	}
}

void StripScopeAgent2::handleUnlinkPred(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuple[0]->getSystem()->getPred() != 0 );

	SpinSystem* other = d_strips[ d_curLane ].d_tuple[0]->getSystem()->getPred();
	Root::Ref<UnlinkSystemCmd> cmd =
		new UnlinkSystemCmd( d_pro->getSpins(), other,
			d_strips[ d_curLane ].d_tuple[0]->getSystem() );
	if( cmd->handle( this ) )
	{
		QString str;
		str.sprintf( "Unlinked %d - %d", other->getId(),
			d_strips[ d_curLane ].d_tuple[0]->getSystem()->getId() );
		Lexi::ShowStatusMessage msg(  str );
		traverseUp( msg );
		return;
	}else
	{
		Lexi::ShowStatusMessage msg(  "Unlink predecessor: unsuccessful" );
		traverseUp( msg );
		return;
	}
}

void StripScopeAgent2::handleLinkStrip(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	int pred, succ;
	if( !a.getParam( 0 ).isNull() && !a.getParam( 1 ).isNull() )
	{
		pred = a.getParam( 0 ).getLong();
		succ = a.getParam( 1 ).getLong();
	}else if( !Dlg::getPredSucc( MW, pred, succ ) )
		return;

	SpinSystem* p = d_pro->getSpins()->getSystem( pred );
	SpinSystem* s = d_pro->getSpins()->getSystem( succ );
	if( p == 0 || s == 0 )
	{
		Lexi::ShowStatusMessage msg(  "Link strips: Unknown Strips" );
		traverseUp( msg );
		return;
	}

	try
	{
		Root::Ref<LinkSystemCmd> cmd =
			new LinkSystemCmd( d_pro->getSpins(), p, s );
		if( cmd->handle( this ) )
		{
			QString str;
			str.sprintf( "Linked %d - %d", p->getId(), s->getId() );
			Lexi::ShowStatusMessage msg(  str );
			traverseUp( msg );
			return;
		}else
		{
			Lexi::ShowStatusMessage msg(  "Link strips: unsuccessful" );
			traverseUp( msg );
			return;
		}
	}catch( Exception& e )
	{
		Root::ReportToUser::alert( this, "Link strips", e.what() );
	}
}

void StripScopeAgent2::handleAssignStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 );

	QString str;
	SpinSystem* sys = d_strips[ d_curLane ].d_tuple[0]->getSystem();
	Root::Index r = 0;

	if( !a.getParam( 0 ).isNull() )
		r = a.getParam( 0 ).getLong();
	else
	{
		bool ok;
		str.sprintf( "Assignment for spin system %d:", sys->getId() );
		r = QInputDialog::getInteger( "Assign Strips", 
			str, (sys->getAssig())?sys->getAssig()->getId():0, 
			-999999, 999999, 1, &ok, MW );
		if( !ok )
			return;
	}
	Residue* res = d_pro->getSequence()->getResidue( r );
	if( res == 0 )
	{
		Lexi::ShowStatusMessage msg( "Assign strip: Unknown residue" );
		traverseUp( msg );
		return;
	}

	Root::Ref<AssignSystemCmd> cmd =
		new AssignSystemCmd( d_pro->getSpins(), sys, res );
	if( cmd->handle( this ) )
	{
		str.sprintf( "Assigned spin system %d to residue %d", 
			sys->getId(), res->getId() );
		Lexi::ShowStatusMessage msg( str );
		traverseUp( msg );
	}else
	{
		Lexi::ShowStatusMessage msg( "Assign strip: unsuccessful" );
		traverseUp( msg );
	}
}

void StripScopeAgent2::handleAutoCenter(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_autoCenter );

	d_autoCenter = !d_autoCenter;
}

void StripScopeAgent2::handleUnassignStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 );

	SpinSystem* sys = d_strips[ d_curLane ].d_tuple[0]->getSystem();

	Root::Ref<UnassignSystemCmd> cmd =
		new UnassignSystemCmd( d_pro->getSpins(), sys );
	if( cmd->handle( this ) )
	{
		QString str;
		str.sprintf( "Unassigned spin system %d", sys->getId() );
		Lexi::ShowStatusMessage msg( str );
		traverseUp( msg );
	}else
	{
		Lexi::ShowStatusMessage msg( "Unassign strip: unsuccessful" );
		traverseUp( msg );
	}
}

void StripScopeAgent2::handleStrictStripMatch(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_query->isStrict() );

	d_query->setStrict( !d_query->isStrict(), true );
}

void StripScopeAgent2::handleUnlabeledStripMatch(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_query->isUnlabeled() );

	if( !d_query->isUnlabeled() )
		d_query->setStrict( false, false );
	d_query->setUnlabeled( !d_query->isUnlabeled(), true );
}

void StripScopeAgent2::handleShowFolded(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_folding );

	Viewport::pushHourglass();
	d_folding = !d_folding;
	for( int i = 0; i < d_strips.size(); i++ )
	{
		d_strips[ i ].d_buf->setFolding( d_folding );
		if( d_folding )
			d_strips[ i ].d_viewer->getViews()->replace( FOLDING, 
			new FoldingView( d_strips[ i ].d_buf ) );
		else
			d_strips[ i ].d_viewer->getViews()->replace( FOLDING, 0 );
		d_strips[ i ].d_viewer->redraw();
	}
	d_slice.d_buf->setFolding( d_folding );
	d_slice.d_bufHold->setFolding( d_folding );
	d_slice.d_viewer->redraw();
	Viewport::popCursor();
}

void StripScopeAgent2::handleSetCandidates(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 );

	SpinSystem* sys = d_strips[ d_curLane ].d_tuple[0]->getSystem();

	CandidateDlg dlg( MW, d_pro->getRepository() );
	dlg.setTitle( sys );
	if( dlg.exec() )
		d_pro->getSpins()->setCands( sys, dlg.d_cands );
}

void StripScopeAgent2::handleSetWidth(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	bool ok	= FALSE;
	QString res;
	res.sprintf( "%0.3f", d_pro->inferPeakWidth( DimX, d_strips[ d_curLane ].d_spec ) );
	res	= QInputDialog::getText( "Set Peak Width", 
		"Please	enter a positive PPM value:", QLineEdit::Normal, 
		res, &ok, MW );
	if( !ok )
		return;
	PPM w = res.toFloat( &ok );
	if( !ok || w < 0.0 )
	{
		QMessageBox::critical( MW, "Set Peak Width",
				"Invalid peak width!", "&Cancel" );
		return;
	}

	d_pro->setPeakWidth( DimX, w, d_strips[ d_curLane ].d_spec );
}

void StripScopeAgent2::handleSetTol(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	AtomType atom;
	atom = d_spec->getColor( DimY );

	bool ok	= FALSE;
	QString res;
	res.sprintf( "%0.3f", d_pro->getMatcher()->getTol( atom ) );
	res	= QInputDialog::getText( "Set Spin Tolerance", 
		"Please	enter a positive PPM value:", QLineEdit::Normal, 
		res, &ok, MW );
	if( !ok )
		return;
	PPM w = res.toFloat( &ok );
	if( !ok ) // RK 31.10.03: w <= 0 neu erlaubt
	{
		QMessageBox::critical( MW, "Set Spin Tolerance",
				"Invalid tolerance!", "&Cancel" );
		return;
	}

	d_pro->getMatcher()->setTol( atom, w );
	d_query->requery();
	d_pro->getStripper()->recalcAll();
}

void StripScopeAgent2::handleCreateReport(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	Dlg::ContourParams p;
	p.d_factor = d_contourFactor;
	p.d_threshold =	d_spec->getThreshold();
	p.d_option = d_contourOption;
	ReportViewer* rv = ReportViewer::getViewer( getParent()->getParent(), p, 
		d_gain, d_autoContour, d_folding );
	Root::Vector<ReportViewer::Param> params;
	params.assign( d_strips.size(), ReportViewer::Param() );
	for( int i = 0; i < d_strips.size(); i++ )
	{
		params[ i ].d_spec = d_strips[ i ].d_spec;
		params[ i ].d_area = d_strips[ i ].d_viewer->getViewArea();
		if( d_strips[ i ].d_spec )
		{
			params[ i ].d_tuple = d_strips[ i ].d_tuples->getModel();
			params[ i ].d_x = d_strips[ i ].d_tuple[ DimX ];
			params[ i ].d_y = d_strips[ i ].d_tuple[ DimY ];
		}else
		{
			params[ i ].d_tuple = 0;
			params[ i ].d_x = 0;
			params[ i ].d_y = 0;
		}
	}
	rv->showStrips( params ); 
}

void StripScopeAgent2::handleEditAtts(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();

	DynValueEditor::edit( MW, 
		d_pro->getRepository()->findObjectDef( Repository::keySpin ), tuple[ DimY ] );
}

void StripScopeAgent2::handleReplaceStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_lane != -1 && d_strips[ d_lane ].d_viewer->hasFocus() );

	int res	= 0;
	
	if( a.getParam( 0 ).isNull() )
	{
		bool ok	= false;
		if( d_strips[ d_lane ].d_spec )
			res = d_strips[ d_lane ].d_tuple[0]->getSystem()->getId();
		res = QInputDialog::getInteger( "Replace Strip", 
			"Please	enter the spin system id to display:", 
			res, 1, 99999999, 1, &ok, MW );
		if( !ok )
			return;
	}else
		res = a.getParam( 0 ).getLong();

	SpinPoint tuple;
	if( !findSystem( res, tuple ) )
		return;
	PpmPoint p;
	p.assign( d_spec->getDimCount(), 0 );
	p[ DimX ] = tuple[ DimX ]->getShift( d_spec );
	p[ DimZ ] = tuple[ DimY ]->getShift( d_spec );

	if( d_hold && d_lane == 0 )
	{
		d_query->setReference( tuple, true );
		AdjustSelection m2( tuple );
		traverseUp( m2 );
		fillPage( false );
	}else
	{
		fillStrip( d_lane, p, tuple, false );
		d_cursor[ DimX ] = p[ DimX ];
		d_cursor[ DimZ ] = p[ DimZ ];
		d_slice.d_spec->setOrigin( d_cursor );
		d_strips[ d_lane ].d_viewer->redraw();
	}
}

void StripScopeAgent2::handleCursorSync(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_cursorSync );
	
	d_cursorSync = !d_cursorSync;
	if( d_cursorSync )
		GlobalCursor::addObserver( this );	// TODO: preset Cursor
	else
		GlobalCursor::removeObserver( this );
}

void StripScopeAgent2::handleAutoGain(Action & a)
{
	ACTION_ENABLED_IF( a, !a.getParam( 0 ).isNull() );

	float g = a.getParam( 0 ).getFloat();
	if( g <= 0.0 )
	{
		Root::ReportToUser::alert( this, "Set Auto Gain", "Invalid Gain Value" );
		return;
	}
	d_gain = g;
	d_autoContour = true;
	for( int i = 0; i < d_strips.size(); i++ )
	{
		createContour( i );
		d_strips[ i ].d_viewer->redraw();
	}
}

void StripScopeAgent2::handleNextStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_first < Index( d_query->getResult().size() - 1 ) );

	d_curLane = -1;
	d_first += 1;
	if( d_first >= d_query->getResult().size() )
		d_first -= 1;
	if( d_first < 0 )
		d_first = 0;
	fillPage( false );
}

void StripScopeAgent2::handlePrevStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_first > 0 );

	d_curLane = -1;
	d_first -= 1;
	if( d_first < 0 )
		d_first = 0;
	fillPage( false );
}

void StripScopeAgent2::handleShowWithOff(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_src3D->showOffs() );
	
	d_src3D->showOffs( !d_src3D->showOffs() );
	d_src3DHold->showOffs( d_src3D->showOffs() );
}

void StripScopeAgent2::handleShowLinks(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_src3D->showLinks() );
	
	d_src3D->showLinks( !d_src3D->showLinks() );
	d_src3DHold->showLinks( d_src3D->showLinks() );
}

void StripScopeAgent2::handleShowInfered(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_src3D->showInferred() );
	
	d_src3D->showInferred( !d_src3D->showInferred() );
	d_src3DHold->showInferred( d_src3D->showInferred() );
}

void StripScopeAgent2::handleShowUnlabeled(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_src3D->showNulls() );
	
	d_src3D->showNulls( !d_src3D->showNulls() );
	d_src3DHold->showNulls( d_src3D->showNulls() );
}

void StripScopeAgent2::handleShowUnknown(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_src3D->showUnknown() );
	
	d_src3D->showUnknown( !d_src3D->showUnknown() );
	d_src3DHold->showUnknown( d_src3D->showUnknown() );
}

void StripScopeAgent2::handleDoPathSim(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_src3D->doPathsim() );
	
	d_src3D->doPathsim( !d_src3D->doPathsim() );
	d_src3DHold->doPathsim( d_src3D->doPathsim() );
}

void StripScopeAgent2::handleShowGhosts(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_strips[ 0 ].d_mdl->showGhosts() );

	d_strips[ 0 ].d_mdl->showGhosts( !d_strips[ 0 ].d_mdl->showGhosts() );
	for( int i = 0; i < d_strips.size(); i++ )
		d_strips[ i ].d_mdl->showGhosts( d_strips[ 0 ].d_mdl->showGhosts() );
}

void StripScopeAgent2::handleProposeSpin(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 && d_spec->isNoesy( DimY ) );

	Spin* orig = 0;
	if( d_spec->isNoesy( DimX, DimY ) )
		orig = d_strips[ d_curLane ].d_tuple[ DimX ];
	else
		orig = d_strips[ d_curLane ].d_tuple[ DimY ];

	ProposeSpinDlg dlg( MW, d_pro, d_spec->getColor( DimY ), d_cursor[ DimY ], 
		d_spec,	"Select Possible Partner" );
	dlg.setAnchor( DimX, d_strips[ d_curLane ].d_tuple[ DimX ] );
	dlg.setAnchor( DimZ, d_strips[ d_curLane ].d_tuple[ DimY ] );
	if( !dlg.exec() || dlg.getSpin() == 0 )
		return;

	if( orig->findLink( dlg.getSpin() ) == 0 ) // Ref == target zulssig wegen Diagonaler
	{
		Root::Ref<LinkSpinCmd> c1 = new LinkSpinCmd( d_pro->getSpins(), orig, dlg.getSpin() ); 
		if( c1->handle( this ) )
			d_strips[ d_curLane ].d_tuples->selectPeak( d_cursor[ DimX ], d_cursor[ DimY ] );
	}else
		Root::ReportToUser::alert( this, "Propose Spin", 
			"The selected spins are already linked!" );
}

void StripScopeAgent2::handlePickLabel(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 );

	const SpinPoint& p = d_strips[ d_curLane ].d_tuple; 
	SpinLabelSet ly = d_spec->getType()->getLabels( d_spec->mapToType( DimY ) );
	NmrExperiment* e = d_pro->getRepository()->getTypes()->
		inferExperiment2( d_spec->getType(), p[0]->getSystem(), d_spec );
	if( e )
	{
		// e->getColumn( d_spec->mapToType( DimY ), ly );
        PathTable::Path filter;
		filter[ d_spec->mapToType( DimX ) ] = p[ DimX ]->getLabel();
		filter[ d_spec->mapToType( DimZ ) ] = p[ DimY ]->getLabel();
		e->getColumn( filter, d_spec->mapToType( DimY ), ly );
	}
	if( p[0]->getSystem() )
		ly = p[0]->getSystem()->getAcceptables( ly );
	SpinLabel y;
	if( a.getParamCount() == 0 || a.getParam( 0 ).isNull() || 
		!SpinLabel::parse( a.getParam( 0 ).getCStr(), y ) )
		if( !Dlg::getLabel( MW, y, ly ) )
			return;
	if( !p[0]->getSystem()->isAcceptable( y ) )
	{
		Root::ReportToUser::alert( this, "Pick Label", "Label is not acceptable" );
		return;
	}

	Root::Ref<PickSystemSpinLabelCmd> cmd = new PickSystemSpinLabelCmd( d_pro->getSpins(), 
		p[0]->getSystem(), d_spec->getColor( DimY ), d_cursor[ DimY ], y, 0 ); 
	cmd->handle( this );

	if( y.isNull() )
		d_src3D->showNulls( true );

	d_strips[ d_curLane ].d_tuples->selectPeak( d_cursor[ DimX ], d_cursor[ DimY ] );
}

void StripScopeAgent2::handleSyncStrip(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 );

	AdjustSelection m2( d_strips[ d_curLane ].d_tuple );
	traverseUp( m2 );
}

void StripScopeAgent2::handleViewLabels(Action & a)
{
	if( a.getParamCount() == 0 )
		return;

	SpinPointView::Label q = (SpinPointView::Label) a.getParam( 0 ).getShort();
	if( q < SpinPointView::None || q >= SpinPointView::End )
		return;

	ACTION_CHECKED_IF( a, true,
		d_strips[ 0 ].d_tuples->getLabel() == q );
	
	for( int i = 0; i < d_strips.size(); i++ )
	{
		d_strips[ i ].d_tuples->setLabel( q, DimY );
		d_strips[ i ].d_viewer->redraw();
	}
}

void StripScopeAgent2::handleAnchPathSim(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_mdl2D->doPathsim() );
	
	d_mdl2D->doPathsim( !d_mdl2D->doPathsim() );
}

void StripScopeAgent2::handleGhostLabels(Action & a)
{
	ACTION_CHECKED_IF( a, true,
		d_strips[ 0 ].d_tuples->ghostLabel() );
	d_strips[ 0 ].d_tuples->ghostLabel( !d_strips[ 0 ].d_tuples->ghostLabel() );
	for( int i = 1; i < d_strips.size(); i++ )
		d_strips[ i ].d_tuples->ghostLabel( d_strips[ 0 ].d_tuples->ghostLabel() );
}

void StripScopeAgent2::handlePickBounds(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	try
	{
		Amplitude val = d_spec->getAt( d_cursor, d_folding, d_folding );
		Amplitude _min, _max;
		if( val > 0.0 )
		{
			_max = val;
			_min = 0;
		}else if( val < 0.0 )
		{
			_max = 0;
			_min = val;
		}else
			return;
		d_slice.d_slice->setMinMax( _min, _max );	
		d_slice.d_sliceHold->setMinMax( _min, _max );	
		d_slice.d_viewer->redraw();
	}catch( ... )
	{
	}
}

void StripScopeAgent2::handleSetBounds(Action & a)
{
	ACTION_ENABLED_IF( a, true );
	
	Amplitude _min, _max;
	_min = d_slice.d_slice->getMin();
	_max = d_slice.d_slice->getMax();
	if( Dlg::getBounds( MW, _min, _max ) )
	{
		d_slice.d_slice->setMinMax( _min, _max );	
		d_slice.d_sliceHold->setMinMax( _min, _max );	
		d_slice.d_viewer->redraw();
	}
}

void StripScopeAgent2::handleAutoSlice(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_slice.d_slice->isAutoScale() );

	bool on = !d_slice.d_slice->isAutoScale();
	d_slice.d_slice->setAutoScale( on );	
	d_slice.d_sliceHold->setAutoScale( on );	
	d_slice.d_viewer->redraw();
}

void StripScopeAgent2::handlePickBoundsSym(Action & a)
{
	ACTION_ENABLED_IF( a, true );

	try
	{
		Amplitude val = d_spec->getAt( d_cursor, d_folding, d_folding );
		Amplitude _min, _max;
		if( val > 0.0 )
		{
			_max = val;
			_min = -val;
		}else if( val < 0.0 )
		{
			_max = -val;
			_min = val;
		}else
			return;
		d_slice.d_slice->setMinMax( _min, _max );	
		d_slice.d_sliceHold->setMinMax( _min, _max );	
		d_slice.d_viewer->redraw();
	}catch( ... )
	{
	}
}

void StripScopeAgent2::handleDeleteLinks(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		!d_strips[ d_curLane ].d_tuples->getSel().empty() &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPointView::Selection sel = d_strips[ d_curLane ].d_tuples->getSel();

	SpinPointView::Selection::const_iterator p;
	Root::Ref<Root::MakroCommand> cmd = new Root::MakroCommand( "Delete Links" );
	SpinLink* l;
	std::set<SpinLink*> test;
	for( p = sel.begin(); p != sel.end(); ++p )
	{
		l = (*p)[ DimX ]->findLink( (*p)[ DimY ] );
		if( l && test.count( l ) == 0 )
		{
			cmd->add( new UnlinkSpinCmd( d_pro->getSpins(), (*p)[ DimX ], (*p)[ DimY ] ) );
			test.insert( l );
		}
	}
	cmd->handle( this );
}

void StripScopeAgent2::handleShowAllPeaks(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_mdl3D->showAll() );
	d_mdl3D->showAll( !d_mdl3D->showAll() );
	d_mdl3DHold->showAll( d_mdl3D->showAll() );
}

void StripScopeAgent2::handleHidePeak(Action & a)
{
	if( d_curLane == -1 || d_strips[ d_curLane ].d_tuples->getSel().size() != 1 )
		return;

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();
	SpinLink* link = tuple[ DimX ]->findLink( tuple[ DimY ] );
	ACTION_ENABLED_IF( a, link && d_strips[ d_curLane ].d_viewer->hasFocus() );
	Root::Ref<HideSpinLinkCmd> cmd = new HideSpinLinkCmd( d_pro->getSpins(), 
		link, d_spec );
	cmd->handle( this );
	// TODO: Plural
}

void StripScopeAgent2::handleKeepSpec(Action & a)
{
	ACTION_CHECKED_IF( a, true, d_keep );
	d_keep = !d_keep;

	if( d_keep )
	{
		d_holdSpec = d_spec;
		d_mdl3DHold->setSpec( 0 );
		d_src3DHold->setSpec( d_spec );
		d_mdl3DHold->setSpec( d_spec );
	}else
	{
		d_holdSpec = 0;
		d_mdl3DHold->setSpec( 0 );
		d_src3DHold->setSpec( 0, false );
		d_src3DHold->setSpecType( 0 );
	}
	fillPage( false );
	if( d_hold && !d_keep )
		d_strips[ 0 ].d_mdl->refill();
}

void StripScopeAgent2::handleEditAttsSys(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();

	DynValueEditor::edit( MW, 
		d_pro->getRepository()->findObjectDef( Repository::keySpinSystem ), 
		tuple[ DimY ]->getSystem() );
}

void StripScopeAgent2::handleEditAttsLink(Action & a)
{
	if( d_curLane == -1 ||
		d_strips[ d_curLane ].d_tuples->getSel().size() != 1 ||
		!d_strips[ d_curLane ].d_viewer->hasFocus() )
		return;
	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();
	SpinLink* l = tuple[ DimX ]->findLink( tuple[ DimY ] );
	ACTION_ENABLED_IF( a, l );

	DynValueEditor::edit( MW, 
		d_pro->getRepository()->findObjectDef( Repository::keyLink ), l );
}

void StripScopeAgent2::handleCalibrate(Action & a)
{
	ACTION_ENABLED_IF( a, d_curLane != -1 &&
		d_strips[ d_curLane ].d_tuples->getSel().size() == 1 &&
		d_strips[ d_curLane ].d_viewer->hasFocus() );

	SpinPoint tuple = *d_strips[ d_curLane ].d_tuples->getSel().begin();
	PpmPoint p( 0, 0, 0 );
	p[ DimY ] = tuple[ DimY ]->getShift( d_spec ) - d_cursor[ DimY ];

	Viewport::pushHourglass();
	Root::Ref<SpecCalibrateCmd> cmd = new SpecCalibrateCmd( d_spec, p );
	cmd->handle( this );
	Viewport::popCursor();
}



