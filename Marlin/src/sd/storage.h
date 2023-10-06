/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Marlin Storage Architecture:
 *
 * DiskIODriver:
 *  Do all low level IO with the underline hardware or block device: SPI, SDIO, OTG
 *
 * FilesystemDriver:
 *  Handle the filesystem format / implementation. Uses the io driver to read and write data.
 *  Sd2Card is the very first and current filesystem implementation on Marlin, supporting FAT.
 *  FatFS - Work in progress.
 *
 *
 * Marlin Abstractions:
 *
 *   Using this names allow us to isolate filesystem driver code, keeping all Marlin code agnostic.
 *
 * MediaFilesystem:
 *  Abstraction of systemwide filesystem operation.
 *
 * MarlinVolume:
 *  Abstraction of a filesystem volume.
 *
 * MediaFile:
 *  Abstraction of a generic file. Using this name allow us to isolate filesystem driver code,
 *  keeping all Marlin code agnostic.
 *
 * PrintFromStorage:
 *  Class to handle printing from any attached storage.
 *
 */

/*

Interface definition. Doesn't need to be compiled, as we use duck typing,
allowing drivers to just use type alias.

Class MarlinVolume {
public:
};

Class MediaFile {
public:
};

Class MediaFilesystem {
public:
    static void init();
    static MarlinVolume* openVolume(const char *);
};

*/
