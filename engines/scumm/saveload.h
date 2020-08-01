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

#include "common/config-manager.h"
#include "common/memstream.h"
#include "common/savefile.h"
#include "common/serializer.h"
#include "common/system.h"
#include "common/zlib.h"
#include "common/ptr.h"

#include "backends/audiocd/audiocd.h"

#include "graphics/thumbnail.h"

//#include "scumm/common-engine-metaengine.h"

namespace Scumm {

#define SaveInfoSectionSize (4+4+4 + 4+4 + 4+2)

#define CURRENT_VER 99
#define INFOSECTION_VERSION 2

struct SaveGameHeader {
	uint32 type;
	uint32 size;
	uint32 ver;
	char name[32];
};

struct SaveInfoSection {
	uint32 type;
	uint32 version;
	uint32 size;

	uint32 timeTValue;  // Obsolete since version 2, but kept for compatibility
	uint32 playtime;

	uint32 date;
	uint16 time;
};

static bool loadSaveGameHeader(Common::SeekableReadStream *in, SaveGameHeader &hdr) {
	hdr.type = in->readUint32BE();
	hdr.size = in->readUint32LE();
	hdr.ver = in->readUint32LE();
	in->read(hdr.name, sizeof(hdr.name));
	return !in->err() && hdr.type == MKTAG('S','C','V','M');
}

} // End of namespace Scumm

namespace {

bool loadAndCheckSaveGameHeader(Common::InSaveFile *in, int heversion, Scumm::SaveGameHeader &hdr, Common::String *error = nullptr) {
	if (!Scumm::loadSaveGameHeader(in, hdr)) {
		if (error) {
			*error = "Invalid savegame";
		}
		return false;
	}

	if (hdr.ver > CURRENT_VER) {
		hdr.ver = TO_LE_32(hdr.ver);
	}

	if (hdr.ver < VER(7) || hdr.ver > CURRENT_VER) {
		if (error) {
			*error = "Invalid version";
		}
		return false;
	}

	// We (deliberately) broke HE savegame compatibility at some point.
	if (hdr.ver < VER(57) && heversion >= 60) {
		if (error) {
			*error = "Unsupported version";
		}
		return false;
	}

	hdr.name[sizeof(hdr.name) - 1] = 0;
	return true;
}

} // End of anonymous namespace

namespace Scumm {

bool getSavegameName(Common::InSaveFile *in, Common::String &desc, int heversion) {
	SaveGameHeader hdr;

	if (!loadAndCheckSaveGameHeader(in, heversion, hdr, &desc)) {
		return false;
	}

	desc = hdr.name;
	return true;
}

Common::String ScummEngine::makeSavegameName(const Common::String &target, int slot, bool temporary) {
	Common::String extension;
	extension = Common::String::format(".%c%02d", temporary ? 'c' : 's', slot);
	return target + extension;
}

bool ScummEngine::querySaveMetaInfos(const char *target, int slot, int heversion, Common::String &desc, Graphics::Surface *&thumbnail, SaveStateMetaInfos *&timeInfos) {
	if (slot < 0) {
		return false;
	}

	SaveGameHeader hdr;
	const Common::String filename = ScummEngine::makeSavegameName(target, slot, false);
	Common::ScopedPtr<Common::SeekableReadStream> in(g_system->getSavefileManager()->openForLoading(filename));

	if (!in) {
		return false;
	}

	if (!loadAndCheckSaveGameHeader(in.get(), heversion, hdr)) {
		return false;
	}

	desc = hdr.name;

	if (hdr.ver > VER(52)) {
		if (Graphics::checkThumbnailHeader(*in)) {
			if (!Graphics::loadThumbnail(*in, thumbnail)) {
				return false;
			}
		}

		if (hdr.ver > VER(57)) {
			if (!loadInfos(in.get(), timeInfos)) {
				return false;
			}
		} else {
			timeInfos = nullptr;
		}
	}

	return true;
}

bool ScummEngine::loadInfos(Common::SeekableReadStream *file, SaveStateMetaInfos *stuff) {
	memset(stuff, 0, sizeof(SaveStateMetaInfos));

	SaveInfoSection section;
	section.type = file->readUint32BE();
	if (section.type != MKTAG('I','N','F','O')) {
		return false;
	}

	section.version = file->readUint32BE();
	section.size = file->readUint32BE();

	// If we ever extend this we should add a table containing the sizes corresponding to each
	// version, so that we are able to properly verify their correctness.
	if (section.version == INFOSECTION_VERSION && section.size != SaveInfoSectionSize) {
		warning("Info section is corrupt");
		file->skip(section.size);
		return false;
	}

	section.timeTValue = file->readUint32BE();
	section.playtime = file->readUint32BE();

	// For header version 1, we load the data in with our old method
	if (section.version == 1) {
		//time_t tmp = section.timeTValue;
		//tm *curTime = localtime(&tmp);
		//stuff->date = (curTime->tm_mday & 0xFF) << 24 | ((curTime->tm_mon + 1) & 0xFF) << 16 | (curTime->tm_year + 1900) & 0xFFFF;
		//stuff->time = (curTime->tm_hour & 0xFF) << 8 | (curTime->tm_min) & 0xFF;
		stuff->date = 0;
		stuff->time = 0;
	}

	if (section.version >= 2) {
		section.date = file->readUint32BE();
		section.time = file->readUint16BE();

		stuff->date = section.date;
		stuff->time = section.time;
	}

	stuff->playtime = section.playtime;

	// Skip over the remaining (unsupported) data
	if (section.size > SaveInfoSectionSize)
		file->skip(section.size - SaveInfoSectionSize);

	return true;
}

}