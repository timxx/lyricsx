#include "application.h"
#include "mainwindow.h"

int main(int argc, char **argv)
{
	Application app(argc, argv);

	MainWindow window;
	window.show();

	return app.exec();
}
