#include "playerfactory.h"
#include "mediaplayer.h"
#include "mprisplayer.h"

LRCX_BEGIN_NS

std::vector<std::string> PlayerFactory::keys()
{
	static std::vector<std::string> s_keys =
	{
		"media",
		"mpris",
	};

	return s_keys;
}

std::unique_ptr<Player> PlayerFactory::create(const std::string &key)
{
	std::unique_ptr<Player> player;

	if (key == "media")
		player.reset(new MediaPlayer);
	else if (key == "mpris")
		player.reset(new MprisPlayer);

	return player;
}

LRCX_END_NS
