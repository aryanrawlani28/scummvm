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

#include "common/stream.h"
#include "engines/savestate.h"
#include "gui/saveload.h"

/**
 * Code in this file is shared between saveload.cpp & detection-static.cpp
 * The MetaEngine can query for MetaData for savefiles without launching the game,
 * since MetaEngines are now always built into the executable.
 */
namespace Drascula {

#define MAGIC_HEADER 0xD6A55A57		// (D)rascula (GA)me (S)cummVM (SA)ve (ST)ate
#define SAVEGAME_VERSION 1

SaveStateDescriptor loadMetaData(Common::ReadStream *s, int slot, bool setPlayTime) {
	uint32 sig = s->readUint32BE();
	byte version = s->readByte();

	SaveStateDescriptor desc(-1, "");	// init to an invalid save slot

	if (sig != MAGIC_HEADER || version > SAVEGAME_VERSION)
		return desc;

	// Save is valid, set its slot number
	desc.setSaveSlot(slot);

	Common::String name;
	byte size = s->readByte();
	for (int i = 0; i < size; ++i)
		name += s->readByte();
	desc.setDescription(name);

	uint32 saveDate = s->readUint32LE();
	int day = (saveDate >> 24) & 0xFF;
	int month = (saveDate >> 16) & 0xFF;
	int year = saveDate & 0xFFFF;
	desc.setSaveDate(year, month, day);

	uint16 saveTime = s->readUint16LE();
	int hour = (saveTime >> 8) & 0xFF;
	int minutes = saveTime & 0xFF;
	desc.setSaveTime(hour, minutes);

	uint32 playTime = s->readUint32LE();
	desc.setPlayTime(playTime * 1000);
	if (setPlayTime)
		g_engine->setTotalPlayTime(playTime * 1000);

	return desc;
}

} // End of namespace Drascula
