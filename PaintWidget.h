#ifndef PAINT_WIDGET_H
#define PAINT_WIDGET_H

#include <QGLWidget>
#include <QPoint>
#include <QColor>
#include <QPainter>
#include <QTimer>

class PaintWindow;
struct PixelInfo;

class PaintWidget : public QGLWidget{
	Q_OBJECT

	public:
		PaintWidget( QWidget* parent = 0 );
		~PaintWidget();
		int selectedTool() const;
		int nSides() const;
		void putPixel( int x, int y, const QColor& c );
		void putSquare( int x, int y, const QColor& c );
		void drawLine( int x1, int y1, int x2, int y2 );
		void drawCircle( int xC, int yC, int r );
		void drawEllipse( int xC, int yC, int rX, int rY );
		void drawSpline( QPoint* points );
		void sprayPixels( int x, int y );
		void drawPolygon( int xC, int yC, int r, float curAngle, int sides );
		
	public slots:
		void setSelectedTool( int t );
		void setNSides( int n );

	protected:
		void initializeGL();
		void resizeGL( int w, int h );
		void paintGL();
		void mousePressEvent( QMouseEvent* event );
		void mouseReleaseEvent( QMouseEvent* event );
		void mouseMoveEvent( QMouseEvent* event );
		void paintEvent( QPaintEvent* event );
		
	private:
		int selectedTool_;
		int nSides_;
		int radius;
		int rx;
		int ry;
		int nClicks;
		double polygonAngle;
		bool firstDone;
		bool pencilActive;
		bool eraserActive;
		bool sprayActive;
		bool clicked;
		PixelInfo* pixelInfo;
		PixelInfo* tempInfo;
		QPoint clickPoint;
		QPoint curPoint;
		QPoint* splinePoints;
		QColor color;
		QColor bgColor;
		QPainter painter;
		QTimer* timer;
		
		void putCirclePixels( int x, int y, int xC, int yC );
		void putEllipsePixels( int x, int y, int xC, int yC );
		QPoint* getVertex( int xC, int yC, int r, float angle );
};

#endif //PAINT_WIDGET_H
