#include "PaintWindow.h"
#include "PaintWidget.h"

#include <QIcon>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QButtonGroup>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QFileDialog>

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
	
	createFileMenu();
	
	connect( this, SIGNAL( changeTool( int ) ), paintWidget, SLOT( setSelectedTool( int ) ) );
	connect( this, SIGNAL( changePolygonSides( int ) ), paintWidget, SLOT( setNSides( int ) ) );
	connect( colorButtonGroup, SIGNAL( buttonClicked( int ) ), paintWidget, SLOT( setColor( int ) ) );
	connect( saveFileAs, SIGNAL( triggered() ), this, SLOT( getFilePath() ) );
	connect( this, SIGNAL( sendFilePath( const QString& ) ), paintWidget, SLOT( saveToFile( const QString& ) ) );
	connect( closeWindow, SIGNAL( triggered() ), this, SLOT( close() ) );
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

void PaintWindow::rectangleSelected(){
	emit changeTool( PaintWindow::Rectangle );
}

void PaintWindow::bucketSelected(){
	emit changeTool( PaintWindow::Bucket );
}

void PaintWindow::copySelected(){
	emit changeTool( PaintWindow::Copy );
}

void PaintWindow::cutSelected(){
	emit changeTool( PaintWindow::Cut );
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

void PaintWindow::getFilePath(){
	QString filePath;
	
	filePath = QFileDialog::getSaveFileName( this, tr( "Save file" ), ".", tr( "Bitmap (*.bmp)" ) );
	
	if( !filePath.isEmpty() ){
		emit sendFilePath( filePath );
	}
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
	
	copyToolButton = new QToolButton( mainToolBar );
	copyToolAction = new QAction( QIcon( "./Icons/CopyIcon.png" ), tr( "Copy" ), this );
	copyToolAction -> setStatusTip( tr( "Copy tool" ) );
	copyToolButton -> setDefaultAction( copyToolAction );
	connect( copyToolAction, SIGNAL( triggered() ), this, SLOT( copySelected() ) );
	mainToolBar -> addWidget( copyToolButton );
	copyToolButton -> setAutoRaise( false );
	
	cutToolButton = new QToolButton( mainToolBar );
	cutToolAction = new QAction( QIcon( "./Icons/CutIcon.png" ), tr( "Cut" ), this );
	cutToolAction -> setStatusTip( tr( "Cut selection tool" ) );
	cutToolButton -> setDefaultAction( cutToolAction );
	connect( cutToolAction, SIGNAL( triggered() ), this, SLOT( cutSelected() ) );
	mainToolBar -> addWidget( cutToolButton );
	cutToolButton -> setAutoRaise( false );
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
	
	rectangleToolButton = new QToolButton( polygonToolBar );
	rectangleToolAction = new QAction( QIcon( "./Icons/RectangleIcon.png" ), tr( "Rectangle" ), this );
	rectangleToolAction -> setStatusTip( tr( "Rectangle Tool" ) );
	rectangleToolButton -> setDefaultAction( rectangleToolAction );
	connect( rectangleToolAction, SIGNAL( triggered() ), this, SLOT( rectangleSelected() ) );
	polygonToolBar -> addWidget( rectangleToolButton );
	rectangleToolButton -> setAutoRaise( false );
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
	
	pinkButton = new QToolButton( colorToolBar ),
	pinkButton -> setIcon( QIcon( "./Icons/PinkIcon.png" ) );
	colorToolBar -> addWidget( pinkButton );
	pinkButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( pinkButton, PINK );
	
	limeButton = new QToolButton( colorToolBar );
	limeButton -> setIcon( QIcon( "./Icons/LimeIcon.png" ) );
	colorToolBar -> addWidget( limeButton );
	limeButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( limeButton, LIME );
	
	orangeButton = new QToolButton( colorToolBar );
	orangeButton -> setIcon( QIcon( "./Icons/OrangeIcon.png" ) );
	colorToolBar -> addWidget( orangeButton );
	orangeButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( orangeButton, ORANGE );
	
	navyButton = new QToolButton( colorToolBar );
	navyButton -> setIcon( QIcon( "./Icons/NavyIcon.png" ) );
	colorToolBar -> addWidget( navyButton );
	navyButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( navyButton, NAVY );
	
	greyButton = new QToolButton( colorToolBar );
	greyButton -> setIcon( QIcon( "./Icons/GreyIcon.png" ) );
	colorToolBar -> addWidget( greyButton );
	greyButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( greyButton, GREY );
	
	purpleButton = new QToolButton( colorToolBar );
	purpleButton -> setIcon( QIcon( "./Icons/PurpleIcon.png" ) );
	colorToolBar -> addWidget( purpleButton );
	purpleButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( purpleButton, PURPLE );
	
	violetButton = new QToolButton( colorToolBar );
	violetButton -> setIcon( QIcon( "./Icons/VioletIcon.png" ) );
	colorToolBar -> addWidget( violetButton );
	violetButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( violetButton, VIOLET );
	
	skyBlueButton = new QToolButton( colorToolBar );
	skyBlueButton -> setIcon( QIcon( "./Icons/SkyBlueIcon.png" ) );
	colorToolBar -> addWidget( skyBlueButton );
	skyBlueButton -> setAutoRaise( false );
	colorButtonGroup -> addButton( skyBlueButton, SKYBLUE );
}

void PaintWindow::createFileMenu(){
	fileMenu = menuBar() -> addMenu( tr( "File" ) );
	
	openFile = new QAction( tr( "Open" ), this );
	openFile -> setStatusTip( tr( "Open file" ) );
	fileMenu -> addAction( openFile );
	
	saveFile = new QAction( tr( "Save" ), this );
	saveFile -> setStatusTip( tr( "Save file" ) );
	fileMenu -> addAction( saveFile );
	
	saveFileAs = new QAction( tr( "Save as..." ), this );
	saveFileAs -> setStatusTip( tr( "Save file as..." ) );
	fileMenu -> addAction( saveFileAs );
	
	closeWindow = new QAction( tr( "Quit" ), this );
	closeWindow -> setStatusTip( tr( "Close application" ) );
	fileMenu -> addAction( closeWindow );
}
