#ifndef __PLAYER_WIDGET_H__
#define __PLAYER_WIDGET_H__

#include "config.h"

#include <QDockWidget>

namespace Ui {
	class PlayerWidget;
}

LRCX_BEGIN_NS

class PlayerWidget : public QDockWidget
{
	Q_OBJECT
public:
	PlayerWidget(QWidget *parent = 0);
	~PlayerWidget();

private Q_SLOTS:
	void onBtnOpen_Clicked();
	void onBtnPlayPause_Clicked();
	void onSliderDuration_Changed(int value);

private:
	std::shared_ptr<Ui::PlayerWidget> m_ui;
};

LRCX_END_NS

#endif // __PLAYER_WIDGET_H__
