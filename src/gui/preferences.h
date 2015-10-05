#ifndef __PREFERENCES_H__
#define __PREFERENCES_H__

#include "config.h"

#include <QDialog>

namespace Ui {
class Preferences;
}

class QFontDatabase;

LRCX_BEGIN_NS

class Preferences : public QDialog
{
	Q_OBJECT
public:
	Preferences(QWidget *parent = 0);

private:
	void initSettings();

	void updateFontFamily(const QString &family);
	void updateFontSize(const QString &family, const QString &style);

private Q_SLOTS:
	void onFontFamilyChanged(const QFont &font);
	void saveSettings();

private:
	std::shared_ptr<Ui::Preferences> m_ui;
	std::shared_ptr<QFontDatabase> m_fontDatabase;
};

LRCX_END_NS

#endif // __PREFERENCES_H__
