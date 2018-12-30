#include "window.h"
#include "tabs.h"

#include <QHBoxLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QApplication>


MainWindow::MainWindow() {
	createWindowContent();
	createMenuAndStatusBar();
}

void MainWindow::SetStatusBar(QString string) {
	statusBar()->showMessage(string);
}

void MainWindow::createWindowContent() {
	// Create layout

	TabDialog* tabs=new TabDialog();

	QHBoxLayout* layout=new QHBoxLayout();
	layout->addStretch();

	// transfer ownership?
	layout->addWidget(tabs, 1);
	layout->addStretch();

	// Put layout in the middle of the window

	_mainWidget = new QWidget();
	_mainWidget->setLayout(layout);

	setCentralWidget(_mainWidget);
}


void MainWindow::createMenuAndStatusBar() {
	statusBar()->showMessage("⚠ No Repository Set!");

	QMenu* fileMenu = menuBar()->addMenu("&File");

	QAction* open = fileMenu->addAction("&Open");
	open->setShortcuts(QKeySequence::Open);
	open->setStatusTip("Open an existing file");

	QAction* quit = fileMenu->addAction("Quit");
	quit->setShortcuts(QKeySequence::Quit);
	quit->setStatusTip("Quit the application");

	connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}
