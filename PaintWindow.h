#ifndef PAINT_WINDOW_H
#define PAINT_WINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QButtonGroup>

class PaintWidget;

class PaintWindow : public QMainWindow{
	Q_OBJECT
	
	public:
		enum ToolNum{
			Line,
			Circle,
			Ellipse,
			Spline,
			Pencil,
			Eraser,
			Spray,
			Polygon,
			Bucket
		};
	
		PaintWindow();
		static int width();
		static int height();
		static void setWidth( int w );
		static void setHeight( int h );
		
	public slots:
		void lineSelected();
		void circleSelected();
		void ellipseSelected();
		void splineSelected();
		void pencilSelected();
		void eraserSelected();
		void spraySelected();
		void polygonSelected();
		void triangleSelected();
		void squareSelected();
		void pentagonSelected();
		void hexagonSelected();
		void heptagonSelected();
		void bucketSelected();
		
	private:
		PaintWidget* paintWidget;
		QToolBar* mainToolBar;
		QToolBar* polygonToolBar;
		QToolBar* colorToolBar;
		QToolButton* lineToolButton;
		QToolButton* circleToolButton;
		QToolButton* ellipseToolButton;
		QToolButton* splineToolButton;
		QToolButton* pencilToolButton;
		QToolButton* eraserToolButton;
		QToolButton* sprayToolButton;
		QToolButton* triangleToolButton;
		QToolButton* squareToolButton;
		QToolButton* pentagonToolButton;
		QToolButton* hexagonToolButton;
		QToolButton* heptagonToolButton;
		QToolButton* bucketToolButton;
		QToolButton* blackButton;
		QToolButton* whiteButton;
		QToolButton* redButton;
		QToolButton* greenButton;
		QToolButton* blueButton;
		QToolButton* cyanButton;
		QToolButton* magentaButton;
		QToolButton* yellowButton;
		QAction* lineToolAction;
		QAction* circleToolAction;
		QAction* ellipseToolAction;
		QAction* splineToolAction;
		QAction* pencilToolAction;
		QAction* eraserToolAction;
		QAction* sprayToolAction;
		QAction* triangleToolAction;
		QAction* squareToolAction;
		QAction* pentagonToolAction;
		QAction* hexagonToolAction;
		QAction* heptagonToolAction;
		QAction* bucketToolAction;
		QButtonGroup* colorButtonGroup;
		static int width_;
		static int height_;
		
		void createMainToolBar();
		void createPolygonToolBar();
		void createColorToolBar();
		
	signals:
		void changeTool( int );
		void changePolygonSides( int );
};

#endif //PAINT_WINDOW_H
