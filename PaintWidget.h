#ifndef PAINT_WIDGET_H
#define PAINT_WIDGET_H

#include <QGLWidget>
#include <QPoint>
#include <QColor>
#include <QPainter>

class PaintWindow;

class PaintWidget : public QGLWidget{
	Q_OBJECT

	public:
		PaintWidget( QWidget* parent = 0 );
		~PaintWidget();
		int selectedTool() const;
		void drawLine( int x1, int y1, int x2, int y2 );
		void drawCircle( int xC, int yC, int r );
		void drawEllipse( int xC, int yC, int rX, int rY );
		
	public slots:
		void setSelectedTool( int t );

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
		int radius;
		int rx;
		int ry;
		bool firstDone;
		bool clicked;
		unsigned int* pixelInfo;
		QPoint clickPoint;
		QPoint curPoint;
		QColor color;
		QPainter painter;
		
		void putCirclePixels( int x, int y, int xC, int yC );
		void putEllipsePixels( int x, int y, int xC, int yC );
};

#endif //PAINT_WIDGET_H
