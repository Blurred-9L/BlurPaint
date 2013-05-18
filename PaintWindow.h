#ifndef PAINT_WINDOW_H
#define PAINT_WINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QAction>

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
			Spray
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
		
	private:
		PaintWidget* paintWidget;
		QToolBar* mainToolBar;
		QToolButton* lineToolButton;
		QToolButton* circleToolButton;
		QToolButton* ellipseToolButton;
		QToolButton* splineToolButton;
		QToolButton* pencilToolButton;
		QToolButton* eraserToolButton;
		QToolButton* sprayToolButton;
		QAction* lineToolAction;
		QAction* circleToolAction;
		QAction* ellipseToolAction;
		QAction* splineToolAction;
		QAction* pencilToolAction;
		QAction* eraserToolAction;
		QAction* sprayToolAction;
		static int width_;
		static int height_;
		
		void createMainToolBar();
		
	signals:
		void changeTool( int );
};

#endif //PAINT_WINDOW_H
