#include "PaintWidget.h"
#include "PaintWindow.h"

#include <QMouseEvent>
#include <QPoint>
#include <QColor>
#include <QPainter>
#include <QPen>
#include <QTimer>
#include <QString>

#include <cstdio>
using std::fwrite;
#include <cmath>
using std::abs;
using std::pow;
using std::sqrt;
using std::atan2;
using std::sin;
using std::cos;
#include <cstdlib>
using std::srand;
using std::rand;
#include <cstring>
using std::memset;
#include <ctime>

#include <GL/glu.h>

#define PIX_COMPONENTS 3

typedef struct bmpInfoHeader{
	unsigned long infoHeaderSize; 		//40
	long width; 						//500
	long height; 						//500
	unsigned short nPlanes; 			//1
	unsigned short colorDepth; 			//24
	unsigned long compressionMethod; 	//0
	unsigned long imageSize; 			//500 * 500 * 3
	long hResolution;					//0
	long vResolution;					//0
	unsigned long nColors;				//0
	unsigned long nImportantColors;		//0	
}bmpInfoHeader;

typedef struct bmpHeader{
	short signature;
	long fileSize;
	long reservedField;
	long offset;
}bmpHeader;

typedef struct PixelInfo{
	unsigned char info[PIX_COMPONENTS];
	
	bool operator==( PixelInfo& p ){
		return ( info[0] == p.info[0] && info[1] == p.info[1] && info[2] == p.info[2] );
	}
	
	bool operator!=( PixelInfo& p ){
		return !( *this == p );
	}
	
}PixelInfo;

PaintWidget::PaintWidget( QWidget* parent ) : QGLWidget( parent ), clickPoint( 0, 0 ), curPoint( 0, 0 ), lowPoint( 0, 0 ),
	color( 0, 0, 0 ), bgColor( 255, 255, 255 ){
	
	width_ = PaintWindow::width();
	height_ = PaintWindow::height();
	rowSize_ = width_ * 3;
	rWidth = 0;
	rHeight = 0;
	while( rowSize_ % 4 != 0 ){
		rowSize_++;
	}
	setMinimumSize( width_, height_ );
	srand( std::time( 0 ) );
	firstDone = false;
	pencilActive = false;
	eraserActive = false;
	sprayActive = false;
	correctClick = false;
	recentlyCleared = false;
	nClicks = 0;
	polygonAngle = 0.0;
	selectedTool_ = PaintWindow::Line;
	pixelInfo = new PixelInfo[ width_ * height_ ];
	tempInfo = 0;
	bufferInfo = 0;
	splinePoints = new QPoint[4];
	timer = new QTimer( this );
	connect( timer, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
}

PaintWidget::~PaintWidget(){
	if( pixelInfo != 0 ){
		delete[] pixelInfo;
	}
	if( splinePoints != 0 ){
		delete[] splinePoints;
	}
	if( tempInfo != 0 ){
		delete[] tempInfo;
	}
}

int PaintWidget::selectedTool() const{
	return selectedTool_;
}

void PaintWidget::setSelectedTool( int t ){
	selectedTool_ = t;
	nClicks = 0;
}

int PaintWidget::nSides() const{
	return nSides_;
}

void PaintWidget::setNSides( int n ){
	nSides_ = n;
}

int PaintWidget::width() const{
	return width_;
}

int PaintWidget::height() const{
	return height_;
}

int PaintWidget::rowSize() const{
	return rowSize_;
}

void PaintWidget::setWidth( int w ){
	width_ = w;
}

void PaintWidget::setHeight( int h ){
	height_ = h;
}

void PaintWidget::setRowSize( int r ){
	rowSize_ = r;
	while( rowSize_ % 4 != 0 ){
		rowSize_++;
	}
}

void PaintWidget::setColor( int c ){
	int r, g, b;
	
	switch( c ){
		case BLACK: case WHITE: case RED: case GREEN: case BLUE: case CYAN: case MAGENTA: case YELLOW:
		case PINK: case LIME: case ORANGE: case NAVY: case GREY: case PURPLE: case VIOLET: case SKYBLUE:
			r = ( c & 0xFF0000 ) >> 16;
			g = ( c & 0x00FF00 ) >> 8;
			b = c & 0x0000FF;
			color.setRgb( r, g, b );
			break;
		default:
			break;
	}
}

void PaintWidget::putPixel( int x, int y, const QColor& c ){
	glColor3f( c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0 );
	glBegin( GL_POINTS );
	glVertex2i( x, y );
	glEnd();
}

void PaintWidget::putSquare( int x, int y, const QColor& c ){
	glColor3f( c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0 );
	glBegin( GL_QUADS );
	glVertex2i( x - 5, y - 5 );
	glVertex2i( x - 5, y + 5 );
	glVertex2i( x + 5, y - 5 );
	glVertex2i( x + 5, y + 5 );
	glEnd();
}

void PaintWidget::drawLine( int x1, int y1, int x2, int y2 ){
	int dx = abs( x2 - x1 );
	int dy = abs( y2 - y1 );
	int p;
	int x, y;
	int addX = ( x1 < x2 )? 1 : ( x1 > x2 )? -1 : 0;
	int addY = ( y1 < y2 )? 1 : ( y1 > y2 )? -1 : 0;
	
	x = x1;
	y = y1;
	if( dx >= dy ){
		p = 2 * dy - dx;
		( selectedTool_ != PaintWindow::Eraser )? putPixel( x, y, color ) : putSquare( x, y, bgColor );
		while( x != x2 ){
			x += addX;
			if( p < 0 ){
				p += 2 * dy;
			}
			else{
				y += addY;
				p += 2 * ( dy - dx );
			}
			( selectedTool_ != PaintWindow::Eraser )? putPixel( x, y, color ) : putSquare( x, y, bgColor );
		}
	}
	else{
		p = 2 * dx - dy;
		( selectedTool_ != PaintWindow::Eraser )? putPixel( x, y, color ) : putSquare( x, y, bgColor );
		while( y != y2 ){
			y += addY;
			if( p < 0 ){
				p += 2 * dx;
			}
			else{
				x += addX;
				p += 2 * ( dx - dy );
			}
			( selectedTool_ != PaintWindow::Eraser )? putPixel( x, y, color ) : putSquare( x, y, bgColor );
		}
	}
}

void PaintWidget::dotLine( int x1, int y1, int x2, int y2 ){
	int dx = abs( x2 - x1 );
	int dy = abs( y2 - y1 );
	int p;
	int x, y;
	int addX = ( x1 < x2 )? 1 : ( x1 > x2 )? -1 : 0;
	int addY = ( y1 < y2 )? 1 : ( y1 > y2 )? -1 : 0;
	bool write = true;
	int count = 0;
	
	x = x1;
	y = y1;
	if( dx >= dy ){
		p = 2 * dy - dx;
		putPixel( x, y, QColor( 0, 0, 0 ) );
		count++;
		while( x != x2 ){
			x += addX;
			if( p < 0 ){
				p += 2 * dy;
			}
			else{
				y += addY;
				p += 2 * ( dy - dx );
			}
			if( write ){
				putPixel( x, y, QColor( 0, 0, 0 ) );
			}
			count++;
			if( count % 5 == 0 ){
				write = !write;
			}
		}
	}
	else{
		p = 2 * dx - dy;
		putPixel( x, y, QColor( 0, 0, 0 ) );
		count++;
		while( y != y2 ){
			y += addY;
			if( p < 0 ){
				p += 2 * dx;
			}
			else{
				x += addX;
				p += 2 * ( dx - dy );
			}
			if( write ){
				putPixel( x, y, QColor( 0, 0, 0 ) );
			}
			count++;
			if( count % 5 == 0 ){
				write = !write;
			}
		}
	}
}

void PaintWidget::drawCircle( int xC, int yC, int r ){
	int x, y;
	int p = 1 - r;
	
	x = 0;
	y = r;
	putCirclePixels( x, y, xC, yC );
	while( x < y ){
		x++;
		if( p < 0 ){
			p += 2 * x + 1;
		}
		else{
			y--;
			p += 2 * ( x - y ) + 1;
		}
		putCirclePixels( x, y, xC, yC );
	}
}

void PaintWidget::drawEllipse( int xC, int yC, int rX, int rY ){
	long long x, y;
	long long rX2 = ( rX * rX );
	long long rY2 = ( rY * rY );
	long long p = ceil( rY2 - rX2 * rY + rX2 * 0.25 );
	long long auxX, auxY;
	
	x = 0;
	y = rY;
	putEllipsePixels( x, y, xC, yC );
	auxX = 0;
	auxY = 2 * rX2 * y;
	while( auxX < auxY ){
		x++;
		auxX = 2 * rY2 * x;
		if( p < 0 ){
			p += auxX + rY2;
		}
		else{
			y--;
			auxY = 2 * rX2 * y;
			p += auxX - auxY + rY2;
		}
		putEllipsePixels( x, y, xC, yC );
	}
	
	p = ceil( rY2 * ( ( x + 0.5 ) * ( x + 0.5 ) ) + rX2 * ( ( y - 1 ) * ( y - 1 ) ) - rX2 * rY2 );
	while( y > 0 ){
		y--;
		auxY = 2 * rX2 * y;
		if( p > 0 ){
			p += -auxY + rX2;
		}
		else{
			x++;
			auxX = 2 * rY2 * x;
			p += auxX - auxY + rX2;
		}
		putEllipsePixels( x, y, xC, yC );
	}
}

void PaintWidget::drawSpline( QPoint* points ){
	double time;
	int x, y;
	
	for( time = 0; time <= 1.0; time += 0.001 ){
		x = pow( 1 - time, 3 ) * points[0].x() + 
			3 * time * pow( 1 - time, 2 ) * points[1].x() +
			3 * pow( time, 2 ) * ( 1- time ) * points[2].x() +
			pow( time, 3 ) * points[3].x();
		y = pow( 1 - time, 3 ) * points[0].y() +
			3 * time * pow( 1 - time, 2 ) * points[1].y() +
			3 * pow( time, 2 ) * ( 1 - time ) * points[2].y() +
			pow( time, 3 ) * points[3].y();
		putPixel( x, y, color );
	}
}

void PaintWidget::sprayPixels( int x, int y ){
	putPixel( x + ( -10 + rand() % 21 ), y + ( -10 + rand() % 21 ), color );
	putPixel( x + ( -10 + rand() % 21 ), y + ( -10 + rand() % 21 ), color );
	putPixel( x + ( -10 + rand() % 21 ), y + ( -10 + rand() % 21 ), color );
	putPixel( x + ( -10 + rand() % 21 ), y + ( -10 + rand() % 21 ), color );
}

void PaintWidget::drawPolygon( int xC, int yC, int r, float curAngle, int sides ){
	int angle;
	int i;
	QPoint** vertexes = new QPoint*[sides];
	
	angle = 360 / sides;
	for( i = 0; i < sides; i++ ){
		vertexes[i] = getVertex( xC, yC, r, curAngle );
		curAngle += angle;
	}
	for( i = 0; i < sides; i++ ){
		drawLine( vertexes[i] -> x(), vertexes[i] -> y(), vertexes[(i+1)%sides] -> x(), vertexes[(i+1)%sides] -> y() );
	}
	for( i = 0; i < sides; i++ ){
		delete vertexes[i];
	}
	delete[] vertexes;
}

void PaintWidget::drawRectangle( int x1, int y1, int x2, int y2, bool dotted ){
	if( dotted ){
		if( x2 > x1 && y2 > y1 ){
			lowPoint.setX( x1 );
			lowPoint.setY( y2 );
		}
		else if( x2 < x1 && y2 > y1 ){
			lowPoint.setX( x2 );
			lowPoint.setY( y2 );
		}
		else if( x2 < x1 && y2 < y1 ){
			lowPoint.setX( x2 );
			lowPoint.setY( y1 );
		}
		else{
			lowPoint.setX( x1 );
			lowPoint.setY( y1 );
		}
		rWidth = abs( x2 - x1 ) - 1;
		rHeight = abs( y2 - y1 ) - 1;
		
		dotLine( x1, y1, x1, y2 );
		dotLine( x1, y1, x2, y1 );
		dotLine( x1, y2, x2, y2 );
		dotLine( x2, y1, x2, y2 );
	}
	else{
		drawLine( x1, y1, x1, y2 );
		drawLine( x1, y1, x2, y1 );
		drawLine( x1, y2, x2, y2 );
		drawLine( x2, y1, x2, y2 );
	}
}

void PaintWidget::fillArea( int x, int y, PixelInfo bgcolor, PixelInfo fillcolor ){
	QColor c( fillcolor.info[0] & 0xFF, fillcolor.info[1] & 0xFF, fillcolor.info[2] & 0xFF );
	PixelInfo pix = pixelInfo[ ( height_ - y ) * width_ + x ];
	int i = x;
	int left, right;
	
	if( pix == bgcolor && pix != fillcolor ){
		while( i >= 0 && pix == bgcolor && pix != fillcolor ){
			pixelInfo[ ( height_ - y ) * width_ + i ] = fillcolor;
			i--;
			pix = pixelInfo[ ( height_ - y ) * width_ + i ];
		}
		left = i + 1;

		i = x + 1;
		pix = pixelInfo[ ( height_ - y ) * width_ + i ];
		while( i <= width_ && pix == bgcolor && pix != fillcolor ){
			pixelInfo[ ( height_ - y ) * width_ + i ] = fillcolor;
			i++;
			pix = pixelInfo[ ( height_ - y ) * width_ + i ];
		}
		right = i - 1;
	
		for( i = left; i <= right; i++ ){
			fillArea( i, y + 1, bgcolor, fillcolor );
			fillArea( i, y - 1, bgcolor, fillcolor );
		}
	}
}

void PaintWidget::clear(){
	glClearColor( 1, 1, 1, 0.0 );
	glClear( GL_COLOR_BUFFER_BIT );
	glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
	recentlyCleared = true;
	updateGL();
}

void PaintWidget::saveToFile( const QString& filePath ){
	FILE* file;
	bmpHeader h1;
	bmpInfoHeader h2;
	PixelInfo* bitmap;
	
	file = fopen( filePath.toStdString().c_str(), "wb" );
	if( file != NULL ){
		bitmap = new PixelInfo[ rowSize_ * height_ ];
		glPixelStorei( GL_PACK_ALIGNMENT, 4 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
		glReadPixels( 0, 0, width_, height_, GL_BGR, GL_UNSIGNED_BYTE, bitmap );
		glPixelStorei( GL_PACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		
		h2.infoHeaderSize = 40;
		h2.width = width_;
		h2.height = height_;
		h2.nPlanes = 1;
		h2.colorDepth = 24;
		h2.compressionMethod = 0;
		h2.imageSize = width_ * 3;
		while( h2.imageSize % 4 != 0 ){
			h2.imageSize++;
		}
		h2.imageSize *= height_;
		h2.hResolution = 0;
		h2.vResolution = 0;
		h2.nColors = 0;
		h2.nImportantColors = 0;
		
		h1.signature = 0x4D42;
		h1.fileSize = 14 + sizeof( bmpInfoHeader ) + h2.imageSize;
		h1.reservedField = 0;
		h1.offset = 54;
		
		fwrite( &h1.signature, sizeof( short ), 1, file );
		fwrite( &h1.fileSize, sizeof( long ), 1, file );
		fwrite( &h1.reservedField, sizeof( long ), 1, file );
		fwrite( &h1.offset, sizeof( long ), 1, file );
		fwrite( &h2, sizeof( bmpInfoHeader ), 1, file );
		fwrite( bitmap, h2.imageSize, 1, file );
		
		fclose( file );
		delete bitmap;
	}
}

void PaintWidget::loadFromFile( const QString& filePath ){
	FILE* file;
	bmpHeader h1;
	bmpInfoHeader h2;
	PixelInfo* bitmap;
	int r;
	
	file = fopen( filePath.toStdString().c_str(), "rb" );
	if( file != NULL ){
		r = fread( &h1.signature, sizeof( short ), 1, file );
		r = fread( &h1.fileSize, sizeof( long ), 1, file );
		r = fread( &h1.reservedField, sizeof( long ), 1, file );
		r = fread( &h1.offset, sizeof( long ), 1, file );
		r = fread( &h2, sizeof( bmpInfoHeader ), 1, file );
		
		if( h1.signature == 0x4D42 && h2.compressionMethod == 0 && h2.colorDepth == 24 ){
			bitmap = new PixelInfo[h2.imageSize];
			r = fread( bitmap, h2.imageSize, 1, file );
		
			resize( h2.width, h2.height );
			glPixelStorei( GL_PACK_ALIGNMENT, 4 );
			glPixelStorei( GL_UNPACK_ALIGNMENT, 4 );
			glDrawPixels( h2.width, h2.height, GL_BGR, GL_UNSIGNED_BYTE, bitmap );
			glPixelStorei( GL_PACK_ALIGNMENT, 1 );
			glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
		
			fclose( file );
			delete bitmap;
			glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
			recentlyCleared = true;
			updateGL();
		}
		else{
			fclose( file );
		}
	}
}

void PaintWidget::initializeGL(){
	if( !firstDone ){
		glClearColor( 1, 1, 1, 0.0 );
		glMatrixMode( GL_PROJECTION );
		gluOrtho2D( 0, width_, 0, height_ );
		glClear( GL_COLOR_BUFFER_BIT );
		glPixelStorei( GL_PACK_ALIGNMENT, 1 );
		glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	}
	else{
		glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
	}
}

void PaintWidget::resizeGL( int w, int h ){
	fixPixelInfo( w, h );
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	gluOrtho2D( 0, w, 0, h );
	PaintWindow::setWidth( w );
	PaintWindow::setHeight( h );
	setWidth( w );
	setHeight( h );
	setRowSize( width_ * 3 );
}

void PaintWidget::paintGL(){
	PixelInfo bg, fill;
	
	if( firstDone ){
		if( !pencilActive && !eraserActive && !sprayActive ){
			painter.begin( this );
			painter.beginNativePainting();
			glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
		}
		if( !recentlyCleared ){
			switch( selectedTool_ ){
				case PaintWindow::Line:
					drawLine( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y() );
					break;
				case PaintWindow::Circle:
					drawCircle( clickPoint.x(), clickPoint.y(), radius );
					break;
				case PaintWindow::Ellipse:
					drawEllipse( clickPoint.x(), clickPoint.y(), rx, ry );
					break;
				case PaintWindow::Spline:
					if( nClicks < 2 ){
						glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
					}
					drawSpline( splinePoints );
					break;
				case PaintWindow::Pencil:
					drawLine( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y() );
					clickPoint.setX( curPoint.x() );
					clickPoint.setY( curPoint.y() );
					pencilActive = true;
					break;
				case PaintWindow::Eraser:
					drawLine( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y() );
					clickPoint.setX( curPoint.x() );
					clickPoint.setY( curPoint.y() );
					eraserActive = true;
					break;
				case PaintWindow::Spray:
					sprayPixels( curPoint.x(), curPoint.y() );
					sprayActive = true;
					break;
				case PaintWindow::Polygon:
					drawPolygon( clickPoint.x(), clickPoint.y(), radius, polygonAngle, nSides_ );
					break;
				case PaintWindow::Rectangle:
					drawRectangle( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y() );
					break;
				case PaintWindow::Bucket:
					bg = pixelInfo[ ( height_ - clickPoint.y() ) * width_ + clickPoint.x() ];
					fill.info[0] = color.red();
					fill.info[1] = color.green();
					fill.info[2] = color.blue();
					fillArea( clickPoint.x(), clickPoint.y(), bg, fill );
					glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
					break;
				case PaintWindow::Copy:
					if( nClicks < 1 ){
						glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
						drawRectangle( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y(), true );
					}
					else if( correctClick ){
						glWindowPos2fMESAemulate( lowPoint.x() + ( curPoint.x() - clickPoint.x() ),
												  height_ - ( lowPoint.y() + ( curPoint.y() - clickPoint.y() ) ) );
						glDrawPixels( rWidth, rHeight, GL_RGB, GL_UNSIGNED_BYTE, bufferInfo );
						glWindowPos2fMESAemulate( 0, 0 );
					}
					break;
				case PaintWindow::Cut:
					if( nClicks < 1 ){
						glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
						drawRectangle( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y(), true );
					}
					else if( correctClick ){
						glWindowPos2fMESAemulate( lowPoint.x() + ( curPoint.x() - clickPoint.x() ),
												  height_ - ( lowPoint.y() + ( curPoint.y() - clickPoint.y() ) ) );
						glDrawPixels( rWidth, rHeight, GL_RGB, GL_UNSIGNED_BYTE, bufferInfo );
						glWindowPos2fMESAemulate( 0, 0 );
					}
					break;
			}
		}
		else{
			recentlyCleared = false;
		}
		if( !pencilActive && !eraserActive && !sprayActive ){
			glFlush();
			painter.endNativePainting();
			painter.end();
		}
	}
}

void PaintWidget::mousePressEvent( QMouseEvent* event ){
	if( event -> button() == Qt::LeftButton ){
		clicked = true;
		firstDone = true;
		clickPoint.setX( event -> x() );
		clickPoint.setY( event -> y() );
		switch( selectedTool_ ){
			case PaintWindow::Spline:
				if( tempInfo == 0 ){
					tempInfo = new PixelInfo[ width_ * height_ ];
				}
				switch( nClicks ){
					case 0:
						glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
						for( int i = 0; i < 4; i++ ){
							splinePoints[i].setX( event -> x() );
							splinePoints[i].setY( event -> y() );
						}
						break;
					case 1:
						for( int i = 1; i < 3; i++ ){
							splinePoints[i].setX( event -> x() );
							splinePoints[i].setY( event -> y() );
						}
						updateGL();
						glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
						break;
					case 2:
						splinePoints[2].setX( event -> x() );
						splinePoints[2].setY( event -> y() );
						updateGL();
						break;
				}
				glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
				break;
			case PaintWindow::Pencil: case PaintWindow::Eraser:
				glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
				curPoint.setX( event -> x() );
				curPoint.setY( event -> y() );
				updateGL();
				break;
			case PaintWindow::Spray:
				glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
				curPoint.setX( event -> x() );
				curPoint.setY( event -> y() );
				setMouseTracking( true );
				timer -> start( 50 );
				break;
			case PaintWindow::Bucket:
				glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
				updateGL();
				break;
			case PaintWindow::Copy:
				if( tempInfo == 0 ){
					tempInfo = new PixelInfo[ width_ * height_ ];
				}
				switch( nClicks ){
					case 0:
						glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
						glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
						break;
					case 1:
						if( checkPoint( clickPoint.x(), clickPoint.y(), lowPoint.x(), lowPoint.y(),
							lowPoint.x() + rWidth, lowPoint.y() - rHeight ) ){
							if( bufferInfo == 0 ){
								bufferInfo = new PixelInfo[ rWidth * rHeight ];
							}
							glReadPixels( lowPoint.x() + 1, height_ - lowPoint.y() + 1, rWidth, rHeight,
										  GL_RGB, GL_UNSIGNED_BYTE, bufferInfo );
							correctClick = true;
						}
						else{
							updateGL();
						}
						break;
				}
				break;
			case PaintWindow::Cut:
				if( tempInfo == 0 ){
					tempInfo = new PixelInfo[ width_ * height_ ];
				}
				switch( nClicks ){
					case 0:
						glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
						glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
						break;
					case 1:
						if( checkPoint( clickPoint.x(), clickPoint.y(), lowPoint.x(), lowPoint.y(),
							lowPoint.x() + rWidth, lowPoint.y() - rHeight ) ){
							if( bufferInfo == 0 ){
								bufferInfo = new PixelInfo[ rWidth * rHeight ];
							}
							glReadPixels( lowPoint.x() + 1, height_ - lowPoint.y() + 1, rWidth, rHeight,
										  GL_RGB, GL_UNSIGNED_BYTE, bufferInfo );
							correctClick = true;
							glDrawPixels( width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
							clearArea( lowPoint.x() + 1, height_ - lowPoint.y() + 1, rWidth, rHeight );
						}
						else{
							updateGL();
						}
						break;
				}
				break;
			default:
				glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
				break;
		}
	}
}

void PaintWidget::mouseReleaseEvent( QMouseEvent* event ){
	if( event -> button() == Qt::LeftButton ){
		clicked = false;
		switch( selectedTool_ ){
			case PaintWindow::Spline:
				nClicks = ( nClicks + 1 ) % 3;
				if( nClicks == 0 ){
					delete tempInfo;
					tempInfo = 0;
					glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
				}
				break;
			case PaintWindow::Pencil:
				pencilActive = false;
				glFlush();
				painter.endNativePainting();
				painter.end();
				break;
			case PaintWindow::Eraser:
				eraserActive = false;
				glFlush();
				painter.endNativePainting();
				painter.end();
				break;
			case PaintWindow::Spray:
				sprayActive = false;
				setMouseTracking( false );
				timer -> stop();
				glFlush();
				painter.endNativePainting();
				painter.end();
				break;
			case PaintWindow::Copy:
				nClicks = ( nClicks + 1 ) % 2;
				if( nClicks == 0 ){
					delete tempInfo;
					tempInfo = 0;
					delete bufferInfo;
					bufferInfo = 0;
					glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
					correctClick = false;
				}
				break;
			case PaintWindow::Cut:
				nClicks = ( nClicks + 1 ) % 2;
				if( nClicks == 0 ){
					delete tempInfo;
					tempInfo = 0;
					delete bufferInfo;
					bufferInfo = 0;
					glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
					correctClick = false;
				}
				break;
			default:
				break;
		}
	}
}

void PaintWidget::mouseMoveEvent( QMouseEvent* event ){
	curPoint.setX( event -> x() );
	curPoint.setY( event -> y() );
	switch( selectedTool_ ){
		case PaintWindow::Circle:
			radius = sqrt( pow( ( clickPoint.x() - curPoint.x() ), 2 ) + pow( clickPoint.y() - curPoint.y(), 2 ) );
			break;
		case PaintWindow::Ellipse:
			rx = abs( clickPoint.x() - curPoint.x() );
			ry = abs( clickPoint.y() - curPoint.y() );
			break;
		case PaintWindow::Spline:
			switch( nClicks ){
				case 0:
					splinePoints[3].setX( event -> x() );
					splinePoints[3].setY( event -> y() );
					break;
				case 1:
					splinePoints[1].setX( event -> x() );
					splinePoints[1].setY( event -> y() );
					break;
				case 2:
					splinePoints[2].setX( event -> x() );
					splinePoints[2].setY( event -> y() );
					break;
			}
			break;
		case PaintWindow::Polygon:
			radius = sqrt( pow( ( clickPoint.x() - curPoint.x() ), 2 ) + pow( clickPoint.y() - curPoint.y(), 2 ) );
			polygonAngle = ( atan2( clickPoint.y() - curPoint.y(), clickPoint.x() - curPoint.x() ) * 180 ) / M_PI;
			break;
	}
	updateGL();
}

void PaintWidget::paintEvent( QPaintEvent* event ){
	painter.begin( this );
	painter.beginNativePainting();
	glInit();
	painter.endNativePainting();
	painter.end();
}

void PaintWidget::putCirclePixels( int x, int y, int xC, int yC ){
	putPixel( x + xC, y + yC, color );
	putPixel( y + xC, x + yC, color );
	putPixel( y + xC, -x + yC, color );
	putPixel( x + xC, -y + yC, color );
	putPixel( -x + xC, -y + yC, color );
	putPixel( -y + xC, -x + yC, color );
	putPixel( -y + xC, x + yC, color );
	putPixel( -x + xC, y + yC, color );
}

void PaintWidget::putEllipsePixels( int x, int y, int xC, int yC ){
	putPixel( x + xC, y + yC, color );
	putPixel( x + xC, -y + yC, color );
	putPixel( -x + xC, -y + yC, color );
	putPixel( -x + xC, y + yC, color );
}

QPoint* PaintWidget::getVertex( int xC, int yC, int r, float angle ){
	float rads;
	QPoint* newPoint = new QPoint;
	
	rads = 180 / M_PI;
	newPoint -> setX( xC + ceil( r * cos( angle / rads ) ) );
	newPoint -> setY( yC + ceil( r * sin( angle / rads ) ) );
	
	return newPoint;
}

void PaintWidget::fixPixelInfo( int newWidth, int newHeight ){
	PixelInfo* temp = new (std::nothrow) PixelInfo[ newWidth * newHeight ];
	
	if( temp != 0 ){
		for( int i = 0; i < newHeight; i++ ){
			for( int j = 0; j < newWidth; j++ ){
				temp[ i * newWidth + j ].info[0] = temp[ i * newWidth + j ].info[1] = temp[ i * newWidth + j ].info[2] = 255;
			}
		}
	
		delete pixelInfo;
		pixelInfo = temp;
	}
	else{
		std::printf( "ERROR\n" );
	}
}

bool PaintWidget::checkPoint( int x, int y, int lx1, int ly1, int lx2, int ly2 ){
	return ( x > lx1 && x < lx2 && y > ly2 && y < ly1 );
}

void PaintWidget::clearArea( int x, int y, int w, int h ){
	PixelInfo* clearBuffer = new PixelInfo[ w * h ];
	
	for( int i = 0; i < w * h; i++ ){
		clearBuffer[i].info[0] = clearBuffer[i].info[1] = clearBuffer[i].info[2] = 255;
	}
	
	glWindowPos2fMESAemulate( x, y );
	glDrawPixels( rWidth, rHeight, GL_RGB, GL_UNSIGNED_BYTE, clearBuffer );
	glWindowPos2fMESAemulate( 0, 0 );
	glReadPixels( 0, 0, width_, height_, GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
	
	delete[] clearBuffer;
}

void PaintWidget::glWindowPos4fMESAemulate( GLfloat x, GLfloat y, GLfloat z, GLfloat w ){
	GLfloat fx, fy;
	
	/* Push current matrix mode and viewport attributes. */
	glPushAttrib( GL_TRANSFORM_BIT | GL_VIEWPORT_BIT );
	/* Setup projection parameters. */
	glMatrixMode( GL_PROJECTION );
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();
	glDepthRange( z, z );
	glViewport( (int)x - 1, (int)y - 1, 2, 2);

	/* Set the raster (window) position. */
	fx = x - (int) x;
	fy = y - (int) y;
	glRasterPos4f(fx, fy, 0.0, w);

	/* Restore matrices, viewport and matrix mode. */
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glPopAttrib();
}

void PaintWidget::glWindowPos2fMESAemulate( GLfloat x, GLfloat y ){
	glWindowPos4fMESAemulate(x, y, 0, 1);
}
