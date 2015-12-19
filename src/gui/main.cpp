#include "application.h"
#include "mainwindow.h"

#include <QMessageBox>
#include <QFileInfo>

int main(int argc, char **argv)
{
	Application app(argc, argv);

	MainWindow window;
	window.show();

	if (argc > 1)
	{
		QString filePath = QString::fromLocal8Bit(argv[1]);

		if (!QFileInfo(filePath).exists())
		{
			QMessageBox::information(&window,
									 window.windowTitle(),
									 QObject::tr("The file you open doesn't exists."));
		}
		else
		{
			window.openFile(filePath);
		}
	}

	return app.exec();
}
