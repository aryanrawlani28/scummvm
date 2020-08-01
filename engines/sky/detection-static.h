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

#include "common/translation.h"
#include "common/file.h"
#include "common/fs.h"
#include "common/savefile.h"
#include "common/system.h"

#include "backends/keymapper/keymap.h"

#include "engines/metaengine.h"
#include "engines/advancedDetector.h"


class SkyMetaEngine : public MetaEngine {
public:
	const char *getName() const override;
	const char *getOriginalCopyright() const override;
	const char *getEngineId() const override;

	bool hasFeature(MetaEngineFeature f) const override;
	PlainGameList getSupportedGames() const override;
	const ExtraGuiOptions getExtraGuiOptions(const Common::String &target) const override;
	PlainGameDescriptor findGame(const char *gameid) const override;
	DetectedGames detectGames(const Common::FSList &fslist) const override;
	Common::KeymapArray initKeymaps(const char *target) const override;

#if PLUGIN_ENABLED_STATIC(SKY)
	Common::Error createInstance(OSystem *syst, Engine **engine) const override;
#endif

	SaveStateList listSaves(const char *target) const override;
	int getMaximumSaveSlot() const override;
	void removeSaveState(const char *target, int slot) const override;
};
