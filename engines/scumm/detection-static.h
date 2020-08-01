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

#include "engines/metaengine.h"

#include "audio/mididrv.h"

#include "scumm/detection.h"
#include "scumm/detection_tables.h"

#include "common/fs.h"
#include "common/list.h"
#include "common/md5.h"
#include "common/savefile.h"
#include "common/system.h"

//using namespace Scumm;

class ScummMetaEngine : public MetaEngine {
public:
	const char *getEngineId() const override;
	const char *getName() const override;
	const char *getOriginalCopyright() const override;

	bool hasFeature(MetaEngineFeature f) const override;
	PlainGameList getSupportedGames() const override;
	PlainGameDescriptor findGame(const char *gameid) const override;
	DetectedGames detectGames(const Common::FSList &fslist) const override;

	int getMaximumSaveSlot() const override;
	const ExtraGuiOptions getExtraGuiOptions(const Common::String &target) const override;
};

class ScummMetaEngineDynamic : public MetaEngineDynamic {
#if PLUGIN_ENABLED_STATIC(SCUMM)
	Common::Error createInstance(OSystem *syst, Engine **engine) const override;
	SaveStateList listSaves(const char *target) const override;
	SaveStateDescriptor querySaveMetaInfos(const char *target, int slot) const override;
	void removeSaveState(const char *target, int slot) const override;
#endif
};
