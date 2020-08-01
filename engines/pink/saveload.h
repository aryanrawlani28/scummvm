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
 * Common functions between engines and metaengines, related to reading savefiles.
 * Not meant to be included by anything apart from saveload.cpp & detection-static.cpp.
 */

#include "graphics/thumbnail.h"

namespace Pink {

WARN_UNUSED_RESULT bool readSaveHeader(Common::InSaveFile &in, SaveStateDescriptor &desc, bool skipThumbnail = true);
Common::String generateSaveName(int slot, const char *gameId);

Common::String generateSaveName(int slot, const char *gameId) {
	return Common::String::format("%s.s%02d", gameId, slot);
}

WARN_UNUSED_RESULT bool readSaveHeader(Common::InSaveFile &in, SaveStateDescriptor &desc, bool skipThumbnail) {
	if (in.readUint32BE() != MKTAG('p', 'i', 'n', 'k'))
		return false;

	const Common::String description = in.readPascalString();
	uint32 date = in.readUint32LE();
	uint16 time = in.readUint16LE();
	uint32 playTime = in.readUint32LE();

	Graphics::Surface *thumbnail = nullptr;
	if (!Graphics::loadThumbnail(in, thumbnail, skipThumbnail))
		return false;

	int day = (date >> 24) & 0xFF;
	int month = (date >> 16) & 0xFF;
	int year = date & 0xFFFF;

	int hour = (time >> 8) & 0xFF;
	int minutes = time & 0xFF;

	desc.setSaveDate(year, month, day);
	desc.setSaveTime(hour, minutes);
	desc.setPlayTime(playTime * 1000);
	desc.setDescription(description);
	desc.setThumbnail(thumbnail);

	return true;
}

}