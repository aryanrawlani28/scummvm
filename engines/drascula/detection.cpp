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


#include "drascula/drascula.h"
#include "drascula/detection-static.h"

namespace Drascula {

uint32 DrasculaEngine::getFeatures() const {
	return _gameDescription->desc.flags;
}

Common::Language DrasculaEngine::getLanguage() const {
	return _gameDescription->desc.language;
}

void DrasculaEngine::loadArchives() {
	const ADGameFileDescription *ag;

	if (getFeatures() & GF_PACKED) {
		for (ag = _gameDescription->desc.filesDescriptions; ag->fileName; ag++) {
			if (!_archives.hasArchive(ag->fileName))
				_archives.registerArchive(ag->fileName, ag->fileType);
		}
	}

	_archives.enableFallback(true);
}

} // End of namespace Drascula

namespace Drascula {

bool DrasculaMetaEngine::createInstance(OSystem *syst, Engine **engine, const ADGameDescription *desc) const {
	const Drascula::DrasculaGameDescription *gd = (const Drascula::DrasculaGameDescription *)desc;
	if (gd) {
		*engine = new Drascula::DrasculaEngine(syst, gd);
	}
	return gd != 0;
}

} // End of namespace Drascula

#if PLUGIN_ENABLED_DYNAMIC(DRASCULA)
	REGISTER_PLUGIN_DYNAMIC(DRASCULA, PLUGIN_TYPE_ENGINE, Drascula::DrasculaMetaEngine);
#else
	REGISTER_PLUGIN_STATIC(DRASCULA, PLUGIN_TYPE_ENGINE, Drascula::DrasculaMetaEngine);
#endif
