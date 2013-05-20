#include "PaintWindow.h"
#include "PaintWidget.h"

#include <QIcon>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QButtonGroup>

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
	
	colorButtonGroup = new QButtonGroup( this );
	colorToolBar = new QToolBar( tr( "Colors" ), this );
	colorToolBar -> setMovable( false );
	createColorToolBar();
	
	connect( this, SIGNAL( changeTool( int ) ), paintWidget, SLOT( setSelectedTool( int ) ) );
	connect( this, SIGNAL( changePolygonSides( int ) ), paintWidget, SLOT( setNSides( int ) ) );
	connect( colorButtonGroup, SIGNAL( buttonClicked( int ) ), paintWidget, SLOT( setColor( int ) ) );
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

void PaintWindow::bucketSelected(){
	emit changeTool( PaintWindow::Bucket );
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
	
	bucketToolButton = new QToolButton( mainToolBar );
	bucketToolAction = new QAction( QIcon( "./Icons/FillIcon.png" ), tr( "Fill Bucket" ), this );
	bucketToolAction -> setStatusTip( tr( "Bucket tool" ) );
	bucketToolButton -> setDefaultAction( bucketToolAction );
	connect( bucketToolAction, SIGNAL( triggered() ), this, SLOT( bucketSelected() ) );
	mainToolBar -> addWidget( bucketToolButton );
	bucketToolButton -> setAutoRaise( false );
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

void PaintWindow::createColorToolBar(){
	addToolBar( Qt::BottomToolBarArea, colorToolBar );
	
	blackButton = new QToolButton( colorToolBar );
	blackButton -> setIcon( QIcon( "./Icons/BlackIcon.png" ) );
	colorToolBar -> addWidget( blackButton );
	blackButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( blackButton, BLACK );
	
	whiteButton = new QToolButton( colorToolBar );
	whiteButton -> setIcon( QIcon( "./Icons/WhiteIcon.png" ) );
	colorToolBar -> addWidget( whiteButton );
	whiteButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( whiteButton, WHITE );
	
	redButton = new QToolButton( colorToolBar );
	redButton -> setIcon( QIcon( "./Icons/RedIcon.png" ) );
	colorToolBar -> addWidget( redButton );
	redButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( redButton, RED );
	
	greenButton = new QToolButton( colorToolBar );
	greenButton -> setIcon( QIcon( "./Icons/GreenIcon.png" ) );
	colorToolBar -> addWidget( greenButton );
	greenButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( greenButton, GREEN );
	
	blueButton = new QToolButton( colorToolBar );
	blueButton -> setIcon( QIcon( "./Icons/BlueIcon.png" ) );
	colorToolBar -> addWidget( blueButton );
	blueButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( blueButton, BLUE );
	
	cyanButton = new QToolButton( colorToolBar );
	cyanButton -> setIcon( QIcon( "./Icons/CyanIcon.png" ) );
	colorToolBar -> addWidget( cyanButton );
	cyanButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( cyanButton, CYAN );
	
	magentaButton = new QToolButton( colorToolBar );
	magentaButton -> setIcon( QIcon( "./Icons/MagentaIcon.png" ) );
	colorToolBar -> addWidget( magentaButton );
	magentaButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( magentaButton, MAGENTA );
	
	yellowButton = new QToolButton( colorToolBar );
	yellowButton -> setIcon( QIcon( "./Icons/YellowIcon.png" ) );
	colorToolBar -> addWidget( yellowButton );
	yellowButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( yellowButton, YELLOW );
}
