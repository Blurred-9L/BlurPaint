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
			Ellipse
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
		
	private:
		PaintWidget* paintWidget;
		QToolBar* mainToolBar;
		QToolButton* lineToolButton;
		QToolButton* circleToolButton;
		QToolButton* ellipseToolButton;
		QAction* lineToolAction;
		QAction* circleToolAction;
		QAction* ellipseToolAction;
		static int width_;
		static int height_;
		
		void createMainToolBar();
		
	signals:
		void changeTool( int );
};

#endif //PAINT_WINDOW_H
