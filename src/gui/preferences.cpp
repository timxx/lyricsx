/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "preferences.h"
#include "ui_preferences.h"
#include "application.h"
#include "i18n.h"

#include <QFontDatabase>

LRCX_BEGIN_NS

Preferences::Preferences(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::Preferences)
	, m_fontDatabase(new QFontDatabase)
{
	m_ui->setupUi(this);

	connect(m_ui->cb_Font, SIGNAL(currentFontChanged(const QFont &)), this, SLOT(onFontFamilyChanged(const QFont&)));
	connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));

	initSettings();
}

void Preferences::initSettings()
{
	QString family = xApp->settings(Application::AS_EditorFontFamily).toString();
	if (family.isEmpty() || !m_fontDatabase->hasFamily(family))
		family = QFont().family();

	m_ui->cb_Font->setCurrentFont(family);

	QVariant editor = xApp->settings(Application::AS_LrcEditor);
	if (editor.isValid())
		m_ui->line_Editor->setText(editor.toString());
	else
		m_ui->line_Editor->setText(i18n::appName());
}

void Preferences::saveSettings()
{
	xApp->setSetting(Application::AS_EditorFontFamily,
					 m_ui->cb_Font->currentFont().family());
	xApp->setSetting(Application::AS_EditorFontStyle,
					 m_ui->cb_Style->currentText());
	xApp->setSetting(Application::AS_EditorFontSize,
					 m_ui->cb_Size->itemData(m_ui->cb_Size->currentIndex()));

	xApp->setSetting(Application::AS_LrcEditor,
					 m_ui->line_Editor->text());
}

void Preferences::onFontFamilyChanged(const QFont &font)
{
	updateFontFamily(font.family());
}

void Preferences::updateFontFamily(const QString &family)
{
	QString style = xApp->settings(Application::AS_EditorFontStyle).toString();
	if (style.isEmpty())
		style = QFontInfo(QFont(family)).styleName();

	const QStringList styles = m_fontDatabase->styles(family);

	m_ui->cb_Style->setCurrentIndex(-1);
	m_ui->cb_Style->clear();

	int styleIndex = -1;
	foreach (const QString &s, styles)
	{
		int count = m_ui->cb_Style->count();
		m_ui->cb_Style->addItem(s);
		if (s == style || (styleIndex == -1 && (s == QLatin1String("Normal") || s == QLatin1String("Regular"))))
			styleIndex = count;
	}

	m_ui->cb_Style->setEnabled(m_ui->cb_Style->count() > 0);
	m_ui->cb_Style->setCurrentIndex(styleIndex == -1 ? 0 : styleIndex);

	updateFontSize(family, style);
}

void Preferences::updateFontSize(const QString &family, const QString &style)
{
	QList<int> pointSizes = m_fontDatabase->pointSizes(family, style);
	if (pointSizes.empty())
		pointSizes = QFontDatabase::standardSizes();

	qSort(pointSizes);

	m_ui->cb_Size->setCurrentIndex(-1);
	m_ui->cb_Size->clear();

	int size = xApp->settings(Application::AS_EditorFontSize).toInt();
	if (size < 1)
		size = QFont().pointSize();

	foreach (int pointSize, pointSizes)
		m_ui->cb_Size->addItem(QString::number(pointSize), QVariant(pointSize));

	int index = 0;
	// find the closet one
	for (int i = 0 ; i < pointSizes.count(); ++i)
	{
		if (pointSizes[i] >= size)
		{
			if (i > 0 && ((size - pointSizes[i - 1]) < (pointSizes[i] - size)))
				index = i - 1;
			else
				index = i;
			break;
		}
	}

	m_ui->cb_Size->setCurrentIndex(index);
}

LRCX_END_NS
