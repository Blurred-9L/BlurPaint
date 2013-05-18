#include "PaintWidget.h"
#include "PaintWindow.h"

#include <QMouseEvent>
#include <QPoint>
#include <QColor>
#include <QPainter>

#include <cstdio>
#include <cmath>
using std::abs;
using std::pow;
using std::sqrt;

#include <GL/glu.h>

#define PIX_COMPONENTS 3

typedef struct PixelInfo{
	char info[PIX_COMPONENTS];
}PixelInfo;

PaintWidget::PaintWidget( QWidget* parent ) : QGLWidget( parent ), clickPoint( 0, 0 ), curPoint( 0, 0 ), color( 0, 0, 0 ){
	//setMouseTracking( true );
	firstDone = false;
	pencilActive = false;
	nClicks = 0;
	selectedTool_ = PaintWindow::Line;
	pixelInfo = new PixelInfo[ PaintWindow::width() * PaintWindow::height() ];
	tempInfo = new PixelInfo[ PaintWindow::width() * PaintWindow::height() ];
	splinePoints = new QPoint[4];
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
		painter.drawPoint( x, y );
		while( x != x2 ){
			x += addX;
			if( p < 0 ){
				p += 2 * dy;
			}
			else{
				y += addY;
				p += 2 * ( dy - dx );
			}
			painter.drawPoint( x, y );
		}
	}
	else{
		p = 2 * dx - dy;
		painter.drawPoint( x, y );
		while( y != y2 ){
			y += addY;
			if( p < 0 ){
				p += 2 * dx;
			}
			else{
				x += addX;
				p += 2 * ( dx - dy );
			}
			painter.drawPoint( x, y );
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
		painter.drawPoint( x, y );
	}
}

void PaintWidget::initializeGL(){
	glClearColor( 1, 1, 1, 0.0 );
	glMatrixMode( GL_PROJECTION );
	gluOrtho2D( 0, PaintWindow::width(), 0, PaintWindow::height() );
	glClear( GL_COLOR_BUFFER_BIT );
}

void PaintWidget::resizeGL( int w, int h ){
	glViewport( 0, 0, w, h );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( 0, w, 0, h );
	glLoadIdentity();
	glClear( GL_COLOR_BUFFER_BIT );
	PaintWindow::setWidth( w );
	PaintWindow::setHeight( h );
}

void PaintWidget::paintGL(){
	if( firstDone ){
		if( !pencilActive ){
			painter.begin( this );
			painter.setPen( color );
			glDrawPixels( PaintWindow::width(), PaintWindow::height(), GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
		}
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
					glDrawPixels( PaintWindow::width(), PaintWindow::height(), GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
				}
				drawSpline( splinePoints );
				break;
			case PaintWindow::Pencil:
				drawLine( clickPoint.x(), clickPoint.y(), curPoint.x(), curPoint.y() );
				clickPoint.setX( curPoint.x() );
				clickPoint.setY( curPoint.y() );
				pencilActive = true;
				break;
		}
		if( selectedTool_ != PaintWindow::Pencil ){
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
				switch( nClicks ){
					case 0:
						glReadPixels( 0, 0, PaintWindow::width(), PaintWindow::height(), GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
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
						glDrawPixels( PaintWindow::width(), PaintWindow::height(), GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
						break;
					case 2:
						splinePoints[2].setX( event -> x() );
						splinePoints[2].setY( event -> y() );
						updateGL();
						break;
				}
				glReadPixels( 0, 0, PaintWindow::width(), PaintWindow::height(), GL_RGB, GL_UNSIGNED_BYTE, tempInfo );
				break;
			default:
				glReadPixels( 0, 0, PaintWindow::width(), PaintWindow::height(), GL_RGB, GL_UNSIGNED_BYTE, pixelInfo );
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
				break;
			case PaintWindow::Pencil:
				pencilActive = false;
				painter.end();
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
	}
	updateGL();
}

void PaintWidget::paintEvent( QPaintEvent* event ){
	painter.begin( this );
	painter.setPen( color );
	if( not firstDone ){
		glInit();
	}
	painter.end();
}

void PaintWidget::putCirclePixels( int x, int y, int xC, int yC ){
	painter.drawPoint( x + xC, y + yC );
	painter.drawPoint( y + xC, x + yC );
	painter.drawPoint( y + xC, -x + yC );
	painter.drawPoint( x + xC, -y + yC );
	painter.drawPoint( -x + xC, -y + yC );
	painter.drawPoint( -y + xC, -x + yC );
	painter.drawPoint( -y + xC, x + yC );
	painter.drawPoint( -x + xC, y + yC );
}

void PaintWidget::putEllipsePixels( int x, int y, int xC, int yC ){
	painter.drawPoint( x + xC, y + yC );
	painter.drawPoint( x + xC, -y + yC );
	painter.drawPoint( -x + xC, -y + yC );
	painter.drawPoint( -x + xC, y + yC );
}