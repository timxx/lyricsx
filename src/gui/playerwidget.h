#ifndef __PLAYER_WIDGET_H__
#define __PLAYER_WIDGET_H__

#include "config.h"

#include <player/player.h>
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

	QString getTitle() const;
	QString getArtist() const;
	QString getAlbum() const;
	QString getEditor() const;

	qint64 getCurrentPosition() const;

private Q_SLOTS:
	void onBtnOpen_Clicked();
	void onBtnPlayPause_Clicked();
	void onSliderDuration_Changed();

	void onPlayerStateChanged(Player::State state);
	void onPlayerDurationChanged(qint64 duration);
	void onPlayerPositionChanged(qint64 pos);

private:
	std::shared_ptr<Ui::PlayerWidget> m_ui;
	std::unique_ptr<Player> m_player;
};

LRCX_END_NS

#endif // __PLAYER_WIDGET_H__
