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
 * This file is not header guarded, it is meant to be included once by the main
 * engine file - dreamweb.h and once by detection-static.cpp.
 */
#include "common/endian.h"
#include "common/rect.h"

#define SCUMMVM_HEADER MKTAG('S', 'C', 'V', 'M')
#define SCUMMVM_BLOCK_MAGIC_SIZE 0x1234
#define SAVEGAME_VERSION 1

namespace DreamWeb {

struct FileHeader {
	char _desc[50];
	uint16 _len[20];
	uint8 _padding[6];

	uint16 len(unsigned int i) const {
		assert(i < 20);
		return READ_LE_UINT16(&_len[i]);
	}
	void setLen(unsigned int i, uint16 length) {
		assert(i < 20);
		WRITE_LE_UINT16(&_len[i], length);
	}
} PACKED_STRUCT;

}
