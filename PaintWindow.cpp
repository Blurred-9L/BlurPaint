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
	
	polygonToolBar = new QToolBar( tr( "Polygons" ), this );
	polygonToolBar -> setMovable( false );
	createPolygonToolBar();
	
	connect( this, SIGNAL( changeTool( int ) ), paintWidget, SLOT( setSelectedTool( int ) ) );
	connect( this, SIGNAL( changePolygonSides( int ) ), paintWidget, SLOT( setNSides( int ) ) );
	
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

void PaintWindow::eraserSelected(){
	emit changeTool( PaintWindow::Eraser );
}

void PaintWindow::spraySelected(){
	emit changeTool( PaintWindow::Spray );
}

void PaintWindow::polygonSelected(){
	emit changeTool( PaintWindow::Polygon );
}

void PaintWindow::triangleSelected(){
	emit changePolygonSides( 3 );
}

void PaintWindow::squareSelected(){
	emit changePolygonSides( 4 );
}

void PaintWindow::pentagonSelected(){
	emit changePolygonSides( 5 );
}

void PaintWindow::hexagonSelected(){
	emit changePolygonSides( 6 );
}

void PaintWindow::heptagonSelected(){
	emit changePolygonSides( 7 );
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
	
	eraserToolButton = new QToolButton( mainToolBar );
	eraserToolAction = new QAction( QIcon( "./Icons/EraserIcon.png" ), tr( "Eraser" ), this );
	eraserToolAction -> setStatusTip( tr( "Eraser tool" ) );
	eraserToolButton -> setDefaultAction( eraserToolAction );
	connect( eraserToolAction, SIGNAL( triggered() ), this, SLOT( eraserSelected() ) );
	mainToolBar -> addWidget( eraserToolButton );
	eraserToolButton -> setAutoRaise( false );
	
	sprayToolButton = new QToolButton( mainToolBar );
	sprayToolAction = new QAction( QIcon( "./Icons/SprayIcon.png" ), tr( "Spray" ), this );
	sprayToolAction -> setStatusTip( tr( "Spray tool" ) );
	sprayToolButton -> setDefaultAction( sprayToolAction );
	connect( sprayToolAction, SIGNAL( triggered() ), this, SLOT( spraySelected() ) );
	mainToolBar -> addWidget( sprayToolButton );
	sprayToolButton -> setAutoRaise( false );
}

void PaintWindow::createPolygonToolBar(){
	addToolBar( Qt::RightToolBarArea, polygonToolBar );
	
	triangleToolButton = new QToolButton( polygonToolBar );
	triangleToolAction = new QAction( QIcon( "./Icons/TriangleIcon.png" ), tr( "Triangle" ), this );
	triangleToolAction -> setStatusTip( tr( "Triangle tool" ) );
	triangleToolButton -> setDefaultAction( triangleToolAction );
	connect( triangleToolAction, SIGNAL( triggered() ), this, SLOT( polygonSelected() ) );
	connect( triangleToolAction, SIGNAL( triggered() ), this, SLOT( triangleSelected() ) );
	polygonToolBar -> addWidget( triangleToolButton );
	triangleToolButton -> setAutoRaise( false );
	
	squareToolButton = new QToolButton( polygonToolBar );
	squareToolAction = new QAction( QIcon( "./Icons/SquareIcon.png" ), tr( "Square" ), this );
	squareToolAction -> setStatusTip( tr( "Square tool" ) );
	squareToolButton -> setDefaultAction( squareToolAction );
	connect( squareToolAction, SIGNAL( triggered() ), this, SLOT( polygonSelected() ) );
	connect( squareToolAction, SIGNAL( triggered() ), this, SLOT( squareSelected() ) );
	polygonToolBar -> addWidget( squareToolButton );
	squareToolButton -> setAutoRaise( false );
	
	pentagonToolButton = new QToolButton( polygonToolBar );
	pentagonToolAction = new QAction( QIcon( "./Icons/PentagonIcon.png" ), tr( "Pentagon" ), this );
	pentagonToolAction -> setStatusTip( tr( "Pentagon tool" ) );
	pentagonToolButton -> setDefaultAction( pentagonToolAction );
	connect( pentagonToolAction, SIGNAL( triggered() ), this, SLOT( polygonSelected() ) );
	connect( pentagonToolAction, SIGNAL( triggered() ), this, SLOT( pentagonSelected() ) );
	polygonToolBar -> addWidget( pentagonToolButton );
	pentagonToolButton -> setAutoRaise( false );
	
	hexagonToolButton = new QToolButton( polygonToolBar );
	hexagonToolAction = new QAction( QIcon( "./Icons/HexagonIcon.png" ), tr( "Hexagon" ), this );
	hexagonToolAction -> setStatusTip( tr( "Hexagon tool" ) );
	hexagonToolButton -> setDefaultAction( hexagonToolAction );
	connect( hexagonToolAction, SIGNAL( triggered() ), this, SLOT( polygonSelected() ) );
	connect( hexagonToolAction, SIGNAL( triggered() ), this, SLOT( hexagonSelected() ) );
	polygonToolBar -> addWidget( hexagonToolButton );
	hexagonToolButton -> setAutoRaise( false );
	
	heptagonToolButton = new QToolButton( polygonToolBar );
	heptagonToolAction = new QAction( QIcon( "./Icons/HeptagonIcon.png" ), tr( "Heptagon" ), this );
	heptagonToolAction -> setStatusTip( tr( "Heptagon tool" ) );
	heptagonToolButton -> setDefaultAction( heptagonToolAction );
	connect( heptagonToolAction, SIGNAL( triggered() ), this, SLOT( polygonSelected() ) );
	connect( heptagonToolAction, SIGNAL( triggered() ), this, SLOT( heptagonSelected() ) );
	polygonToolBar -> addWidget( heptagonToolButton );
	heptagonToolButton -> setAutoRaise( false );
}
