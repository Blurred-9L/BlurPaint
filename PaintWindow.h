#ifndef PAINT_WINDOW_H
#define PAINT_WINDOW_H

#include <QMainWindow>
#include <QToolBar>
#include <QToolButton>
#include <QAction>
#include <QButtonGroup>
#include <QMenu>
#include <QMenuBar>
#include <QString>
#include <QColor>

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
			Rectangle,
			Bucket,
			Copy,
			Cut
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
		void rectangleSelected();
		void bucketSelected();
		void copySelected();
		void cutSelected();
		void getSaveFilePath();
		void getOpenFilePath();
		void getUsedFilePath();
		void resetFilePath();
		void pickColor();
		void getInfo();
		
	private:
		QString lastFile;
		PaintWidget* paintWidget;
		QMenu* fileMenu;
		QMenu* toolMenu;
		QMenu* polygonMenu;
		QMenu* colorMenu;
		QMenu* helpMenu;
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
		QToolButton* rectangleToolButton;
		QToolButton* bucketToolButton;
		QToolButton* copyToolButton;
		QToolButton* cutToolButton;
		QToolButton* blackButton;
		QToolButton* whiteButton;
		QToolButton* redButton;
		QToolButton* greenButton;
		QToolButton* blueButton;
		QToolButton* cyanButton;
		QToolButton* magentaButton;
		QToolButton* yellowButton;
		QToolButton* pinkButton;
		QToolButton* limeButton;
		QToolButton* orangeButton;
		QToolButton* navyButton;
		QToolButton* greyButton;
		QToolButton* purpleButton;
		QToolButton* violetButton;
		QToolButton* skyBlueButton;
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
		QAction* rectangleToolAction;
		QAction* bucketToolAction;
		QAction* copyToolAction;
		QAction* cutToolAction;
		QAction* newFile;
		QAction* openFile;
		QAction* saveFile;
		QAction* saveFileAs;
		QAction* closeWindow;
		QAction* pickColorAction;
		QAction* aboutAction;
		QButtonGroup* colorButtonGroup;
		static int width_;
		static int height_;
		
		void createMainToolBar();
		void createPolygonToolBar();
		void createColorToolBar();
		void createFileMenu();
		void createToolMenu();
		void createPolygonMenu();
		
	signals:
		void changeTool( int );
		void changePolygonSides( int );
		void sendSaveFilePath( const QString& );
		void sendOpenFilePath( const QString& );
		void sendColor( const QColor& );
};

#endif //PAINT_WINDOW_H
