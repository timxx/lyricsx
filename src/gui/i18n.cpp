/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "i18n.h"

#include <QCoreApplication>

namespace lrcxns
{
	namespace i18n
	{
		QString tr(const char * key,
				   const char * disambiguation = 0,
				   int n = -1)
		{
			return QCoreApplication::translate("lrcxns::i18n", key, disambiguation, n);
		}

		QString appName()
		{
			return tr("LyricsX");
		}

		QString filterAllFiles()
		{
			return tr("All files (*)");
		}

		QString filterLRCFiles()
		{
			return tr("LRC files (*.lrc)");
		}

		QString filterMP3Files()
		{
			return tr("MP3 files (*.mp3)");
		}

		QString openFileTitle()
		{
			return tr("Open File");
		}

		QString saveFileTitle()
		{
			return tr("Save File");
		}
	}
}
