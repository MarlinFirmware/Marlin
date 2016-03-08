///////////////////////////////////////////////////////////////////////////////
/// \file SDCache.h
///
/// \author Koldo Imanol de Miguel
///         Ruy Garcia
///
/// \brief SD file list cache.
///
/// Copyright (c) 2015 BQ - Mundo Reader S.L.
/// http://www.bq.com
///
/// This file is free software; you can redistribute it and/or modify
/// it under the terms of either the GNU General Public License version 2 or 
/// later or the GNU Lesser General Public License version 2.1 or later, both
/// as published by the Free Software Foundation.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
/// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING 
/// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
/// DEALINGS IN THE SOFTWARE.
///////////////////////////////////////////////////////////////////////////////

#ifndef SD_CACHE_H
#define SD_CACHE_H

#include "cardreader.h"

#ifndef SD_CACHE_SIZE
	#define SD_CACHE_SIZE LCD_HEIGHT+2
#endif //SD_CACHE_SIZE

typedef enum
{
	NOACTION = 0,
	BACK_ENTRY,
	UPDIR_ENTRY,
	FOLDER_ENTRY,
	FILE_ENTRY,
	INVALID_NAME,
} CacheEntryType_t;

typedef struct 
{
	CacheEntryType_t type;
	char filename[13];
	char longFilename[LONG_FILENAME_LENGTH];
} cache_entry;

class SDCache
{

	public:
		SDCache();
		~SDCache();
		
		void reloadCache();
		bool updateCachePosition(int16_t index);
		CacheEntryType_t press(uint16_t index);
		void returnToRoot();
		
		inline cache_entry const * getSelectedEntry() { return (m_cache + m_selected_file); };
		inline uint16_t getListLength() { return m_list_length; };
		inline uint8_t getWindowSize() { return m_window_size; };
		inline uint8_t getIndex() { return m_index; };
		inline bool getFolderIsRoot() { return (m_directory_depth == 0); };
		inline char * getDirectoryName() { return m_directory; };
		inline bool showingFirstItem() { return !m_window_min; };
		inline bool showingLastItem() { return m_window_max == m_list_length-1;};
		inline bool maxDirectoryReached() { return (m_directory_depth == MAX_DIR_DEPTH-1); };
		
		void setWindowCentered();
		inline void setWindowNotCentered() { m_window_is_centered = false; }
		
	private:
		void changeDir();
		void updateDirectoryName();

	public:
		//cache to show, based on partial iterators to cache
		const cache_entry * window_cache_begin;
		const cache_entry * window_cache_end;
		
	private:
		cache_entry m_cache[SD_CACHE_SIZE]; //MAX_CACHE_SIZE
		bool m_cache_update;
		uint16_t m_index;
		uint16_t m_list_length;
		uint8_t m_selected_file;

		char m_directory[20];
		uint8_t	m_directory_depth;
		
		uint8_t m_cache_size;
		uint16_t m_cache_min;
		uint16_t m_cache_max;
		uint8_t m_window_size;
		uint16_t m_window_min;
		uint16_t m_window_max;
		
		bool m_window_is_centered;
		uint8_t m_window_offset;
};

#endif //SD_CACHE_H
