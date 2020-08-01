/* ScummVM - Graphic Adventure Engine
 *
 * ScummVM is the legal property of its developers, whose names
 * are too numerous to list here. Please refer to the COPYRIGHT
 * file distributed with this source distribution.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */

/**
 * Some common code for Engine & MetaEngine.
 */

#ifndef SKY_COMMON_ENGINE_METAENGINE_H
#define SKY_COMMON_ENGINE_METAENGINE_H

#define MAX_SAVE_GAMES  999
#define MAX_TEXT_LEN     80

namespace Sky {

enum SkyAction {
	kSkyActionNone,
	kSkyActionToggleFastMode,
	kSkyActionToggleReallyFastMode,
	kSkyActionOpenControlPanel,
	kSkyActionConfirm,
	kSkyActionSkip,
	kSkyActionSkipLine,
	kSkyActionPause
};

// This was orignally a class member for the SkyEngine.
static const char *shortcutsKeymapId = "sky-shortcuts";

} // End of namespace Sky

#endif
