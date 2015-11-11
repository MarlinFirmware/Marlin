#ifndef SD_CACHE_H
#define SD_CACHE_H

#include "Singleton.h"
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
} CacheEntryType_t;

typedef struct 
{
	CacheEntryType_t type;
	void * action;		//Used in 1st gen, probably can be removed
	char filename[13];
	char longFilename[LONG_FILENAME_LENGTH];
} cache_entry;

class SDCache
{
	public:
	
		typedef Singleton<SDCache> single;

	public:
		SDCache();
		
		void reloadCache();
		void updateCachePosition(int16_t index);
		CacheEntryType_t press(uint16_t index);
		
		inline cache_entry const * getSelectedEntry() { return window_cache_begin + m_selected_file; };
		inline uint16_t getListLenght() { return m_list_length; };
		inline bool getFolderIsRoot() { return (m_directory_depth == 0); };
		inline char * getDirectoryName() { return m_directory; };
		inline bool showingFirstItem() { return !m_window_min; };
		inline bool maxDirectoryReached() { return (m_directory_depth == MAX_DIR_DEPTH-1); };
		
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
};

#endif //SD_CACHE_H
