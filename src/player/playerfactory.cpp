/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "playerfactory.h"
#include "mediaplayer.h"
#ifdef LRCX_HAVE_DBUS
#  include "mprisplayer.h"
#endif

LRCX_BEGIN_NS

std::vector<std::string> PlayerFactory::keys()
{
	static std::vector<std::string> s_keys =
	{
		"media",
#ifdef LRCX_HAVE_DBUS
		"mpris",
#endif
	};

	return s_keys;
}

std::unique_ptr<Player> PlayerFactory::create(const std::string &key)
{
	std::unique_ptr<Player> player;

	if (key == "media")
		player.reset(new MediaPlayer);
#ifdef LRCX_HAVE_DBUS
	else if (key == "mpris")
		player.reset(new MprisPlayer);
#endif

	return player;
}

LRCX_END_NS
