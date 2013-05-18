#include "PaintWindow.h"
#include "PaintWidget.h"

#include <QIcon>
#include <QToolBar>
#include <QToolButton>
#include <QAction>

int PaintWindow::width_ = 500;
int PaintWindow::height_ = 500;

PaintWindow::PaintWindow(){
	paintWidget = new PaintWidget;
	
	setCentralWidget( paintWidget );
	
	mainToolBar = new QToolBar( tr( "Drawing Tools" ), this );
	mainToolBar -> setMovable( false );
	createMainToolBar();
	
	connect( this, SIGNAL( changeTool( int ) ), paintWidget, SLOT( setSelectedTool( int ) ) );
	
	resize( width_, height_ );
}

int PaintWindow::width(){
	return width_;
}

int PaintWindow::height(){
	return height_;
}

void PaintWindow::setWidth( int w ){
	width_ = w;
}

void PaintWindow::setHeight( int h ){
	height_ = h;
}

void PaintWindow::lineSelected(){
	emit changeTool( PaintWindow::Line );
}

void PaintWindow::circleSelected(){
	emit changeTool( PaintWindow::Circle );
}

void PaintWindow::ellipseSelected(){
	emit changeTool( PaintWindow::Ellipse );
}

void PaintWindow::splineSelected(){
	emit changeTool( PaintWindow::Spline );
}

void PaintWindow::pencilSelected(){
	emit changeTool( PaintWindow::Pencil );
}

void PaintWindow::createMainToolBar(){
	addToolBar( Qt::LeftToolBarArea, mainToolBar );
	
	lineToolButton = new QToolButton( mainToolBar );
	lineToolAction = new QAction( QIcon( "./Icons/LineIcon.png" ), tr( "Line" ), this );
	lineToolAction -> setStatusTip( tr( "Draw line" ) );
	lineToolButton -> setDefaultAction( lineToolAction );
	connect( lineToolAction, SIGNAL( triggered() ), this, SLOT( lineSelected() ) );
	mainToolBar -> addWidget( lineToolButton );
	lineToolButton -> setAutoRaise( false );
	
	circleToolButton = new QToolButton( mainToolBar );
	circleToolAction = new QAction( QIcon( "./Icons/CircleIcon.png" ), tr( "Circle" ), this );
	circleToolAction -> setStatusTip( tr( "Draw circle" ) );
	circleToolButton -> setDefaultAction( circleToolAction );
	connect( circleToolAction, SIGNAL( triggered() ), this, SLOT( circleSelected() ) );
	mainToolBar -> addWidget( circleToolButton );
	circleToolButton -> setAutoRaise( false );
	
	ellipseToolButton = new QToolButton( mainToolBar );
	ellipseToolAction = new QAction( QIcon( "./Icons/EllipseIcon.png" ), tr( "Ellipse" ), this );
	ellipseToolAction -> setStatusTip( tr( "Draw ellipse" ) );
	ellipseToolButton -> setDefaultAction( ellipseToolAction );
	connect( ellipseToolAction, SIGNAL( triggered() ), this, SLOT( ellipseSelected() ) );
	mainToolBar -> addWidget( ellipseToolButton );
	ellipseToolButton -> setAutoRaise( false );
	
	splineToolButton = new QToolButton( mainToolBar );
	splineToolAction = new QAction( QIcon( "./Icons/SplineIcon.png" ), tr( "Spline" ), this );
	splineToolAction -> setStatusTip( tr( "Draw spline" ) );
	splineToolButton -> setDefaultAction( splineToolAction );
	connect( splineToolAction, SIGNAL( triggered() ), this, SLOT( splineSelected() ) );
	mainToolBar -> addWidget( splineToolButton );
	splineToolButton -> setAutoRaise( false );
	
	pencilToolButton = new QToolButton( mainToolBar );
	pencilToolAction = new QAction( QIcon( "./Icons/PencilIcon.png" ), tr( "Pencil" ), this );
	pencilToolAction -> setStatusTip( tr( "Pencil tool" ) );
	pencilToolButton -> setDefaultAction( pencilToolAction );
	connect( pencilToolAction, SIGNAL( triggered() ), this, SLOT( pencilSelected() ) );
	mainToolBar -> addWidget( pencilToolButton );
	pencilToolButton -> setAutoRaise( false );
}
