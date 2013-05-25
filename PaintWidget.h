#ifndef PAINT_WIDGET_H
#define PAINT_WIDGET_H

#include <QGLWidget>
#include <QPoint>
#include <QColor>
#include <QPainter>
#include <QTimer>
#include <QString>

#define BLACK   0x000000
#define WHITE   0xFFFFFF
#define RED     0xFF0000
#define GREEN   0x00FF00
#define BLUE    0x0000FF
#define CYAN    0x00FFFF
#define MAGENTA 0xFF00FF
#define YELLOW  0xFFFF00

class PaintWindow;
struct PixelInfo;

class PaintWidget : public QGLWidget{
	Q_OBJECT

	public:	
		PaintWidget( QWidget* parent = 0 );
		~PaintWidget();
		int selectedTool() const;
		int nSides() const;
		int width() const;
		int height() const;
		int rowSize() const;
		void setWidth( int w );
		void setHeight( int h );
		void setRowSize( int r );
		void putPixel( int x, int y, const QColor& c );
		void putSquare( int x, int y, const QColor& c );
		void drawLine( int x1, int y1, int x2, int y2 );
		void dotLine( int x1, int y1, int x2, int y2 );
		void drawCircle( int xC, int yC, int r );
		void drawEllipse( int xC, int yC, int rX, int rY );
		void drawSpline( QPoint* points );
		void sprayPixels( int x, int y );
		void drawPolygon( int xC, int yC, int r, float curAngle, int sides );
		void drawRectangle( int x1, int y1, int x2, int y2, bool dotted = false );
		void fillArea( int x, int y, PixelInfo bgcolor, PixelInfo fillcolor );
		
	public slots:
		void setSelectedTool( int t );
		void setNSides( int n );
		void setColor( int c );
		void saveToFile( const QString& filePath );

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
		int width_;
		int height_;
		int rowSize_;
		int rWidth;
		int rHeight;
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
		bool correctClick;
		PixelInfo* pixelInfo;
		PixelInfo* tempInfo;
		PixelInfo* bufferInfo;
		QPoint clickPoint;
		QPoint curPoint;
		QPoint lowPoint;
		QPoint* splinePoints;
		QColor color;
		QColor bgColor;
		QPainter painter;
		QTimer* timer;
		
		void putCirclePixels( int x, int y, int xC, int yC );
		void putEllipsePixels( int x, int y, int xC, int yC );
		QPoint* getVertex( int xC, int yC, int r, float angle );
		void fixPixelInfo( int newWidth, int newHeight );
		bool checkPoint( int x, int y, int x1, int y1, int x2, int y2 );
		void clearArea( int x, int y, int w, int h );
		
		void glWindowPos4fMESAemulate( GLfloat x, GLfloat y, GLfloat z, GLfloat w );
		void glWindowPos2fMESAemulate( GLfloat x, GLfloat y );
};

#endif //PAINT_WIDGET_H
