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

#include "common/system.h"

#include "pink/pink.h"
#include "pink/objects/module.h"
#include "pink/saveload.h" // For readsaveheader and generateSaveName.

namespace Pink {

Common::Error PinkEngine::loadGameState(int slot) {
	Common::SeekableReadStream *in = _saveFileMan->openForLoading(generateSaveName(slot, _targetName.c_str()));
	if (!in)
		return Common::kNoGameDataFoundError;

	SaveStateDescriptor desc;
	if (!readSaveHeader(*in, desc))
		return Common::kUnknownError;

	Archive archive(in);
	_variables.deserialize(archive);
	_nextModule = archive.readString();
	_nextPage = archive.readString();
	initModule(archive.readString(), "", &archive);
	setTotalPlayTime(desc.getPlayTimeMSecs());

	delete in;
	return Common::kNoError;
}

Common::Error PinkEngine::saveGameState(int slot, const Common::String &desc, bool isAutosave) {
	Common::OutSaveFile *out = _saveFileMan->openForSaving(generateSaveName(slot, _targetName.c_str()));
	if (!out)
		return Common::kUnknownError;

	Archive archive(out);

	out->writeUint32BE(MKTAG('p', 'i', 'n', 'k'));
	archive.writeString(desc);

	TimeDate curTime;
	_system->getTimeAndDate(curTime);

	out->writeUint32LE(((curTime.tm_mday & 0xFF) << 24) | (((curTime.tm_mon + 1) & 0xFF) << 16) | ((curTime.tm_year + 1900) & 0xFFFF));
	out->writeUint16LE(((curTime.tm_hour & 0xFF) << 8) | ((curTime.tm_min) & 0xFF));

	out->writeUint32LE(getTotalPlayTime() / 1000);

	if (!Graphics::saveThumbnail(*out))
		return Common::kUnknownError;

	_variables.serialize(archive);
	archive.writeString(_nextModule);
	archive.writeString(_nextPage);

	archive.writeString(_module->getName());
	_module->saveState(archive);

	delete out;

	return Common::kNoError;
}

}
