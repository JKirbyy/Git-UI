#pragma once

#include <QMainWindow>

class QLabel;
class QWidget;

class MainWindow: public QMainWindow {
public:
	MainWindow();
	void SetStatusBar(QString string);
private:
	void createWindowContent();
	void createMenuAndStatusBar();

private: // widgets we own
	QLabel* _label;
	QWidget* _mainWidget;
};
