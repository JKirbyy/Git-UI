// Window subclassing example - main program
// (NDE, 2016-09-08)

#include "window.h"
#include "globals.h"

#include <QApplication>
#include <iostream>
#include <unistd.h>

int argc=0;
char* argv[] = {NULL};

int main(int argc, char *argv[]) {
	QApplication app(argc, argv);
	MainWindow window;
	//sending the main window as a pointer for global use
	main_globals.SetWindow(&window);
	window.setMinimumSize(800, 400);
	window.show();//starting main application GIT GUI
	app.exec();

	// avoid segfault in QtWidget destructors.
	// _exit(0);
}
