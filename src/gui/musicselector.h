#ifndef __MUSIC_SELECTOR_H__
#define __MUSIC_SELECTOR_H__

#include "config.h"
#ifdef LRCX_HAVE_DBUS
#  include <player/mprisplayerhelper.h>
#endif

#include <QDialog>

namespace Ui {
class MusicSelector;
}

LRCX_BEGIN_NS

class Player;

class MusicSelector : public QDialog
{
	Q_OBJECT
public:
	MusicSelector(QWidget *parent = 0);
	~MusicSelector();

	static std::unique_ptr<Player> select();

private:
#ifdef LRCX_HAVE_DBUS
	void initMpris();
#endif

private Q_SLOTS:
	void onRB_Local_Clicked(bool checked = false);
	void onRB_Mpris_Clicked(bool checked = false);

	void onBtn_Open_Clicked(bool checked = false);

#ifdef LRCX_HAVE_DBUS
	void onMprisPlayerAdded(const MprisPlayerInfo &player);
	void onMprisPlayerClosed(const MprisPlayerInfo &player);
#endif

private:
	std::shared_ptr<Ui::MusicSelector> m_ui;

#ifdef LRCX_HAVE_DBUS
	MprisPlayerHelper *m_mprisPlayer;
#endif
};

LRCX_END_NS

#endif // __MUSIC_SELECTOR_H__
