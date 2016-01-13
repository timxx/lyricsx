/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "application.h"
#include "mainwindow.h"
#include "i18n.h"

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
									 i18n::fileNotExists());
		}
		else
		{
			window.openFile(filePath);
		}
	}

	return app.exec();
}
