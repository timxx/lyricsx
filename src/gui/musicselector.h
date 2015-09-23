#ifndef __MUSIC_SELECTOR_H__
#define __MUSIC_SELECTOR_H__

#include "config.h"

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
	void initMpris();

private Q_SLOTS:
	void onRB_Local_Clicked(bool checked = false);
	void onRB_Mpris_Clicked(bool checked = false);

	void onBtn_Open_Clicked(bool checked = false);

private:
	std::shared_ptr<Ui::MusicSelector> m_ui;
};

LRCX_END_NS

#endif // __MUSIC_SELECTOR_H__
