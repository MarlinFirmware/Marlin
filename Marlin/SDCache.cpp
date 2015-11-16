#include "SDCache.h"

#include "cardreader.h"

//review for all data
SDCache::SDCache()
	: m_cache_update(true)
	, m_index(0)
	, m_cache_size(0)
	, m_list_length(0)
	, m_selected_file(0)
	, m_window_size(0) 
	, m_directory_depth(0)
	, m_window_is_centered(false)
	, m_window_offset(0)
{ 
	card.initsd();
}

SDCache::~SDCache()
{
}

void SDCache::reloadCache()
{
	card.reloadDir();
	
	m_index = 0;
	m_selected_file = 0;
	m_list_length = card.getnrfilenames() + 1;

	if (m_list_length < LCD_HEIGHT) 
	{
		m_window_size = m_list_length;
		m_cache_size = m_list_length;
	} 
	else if (m_list_length < SD_CACHE_SIZE) 
	{
		m_window_size = LCD_HEIGHT;
		m_cache_size = m_list_length;
	} 
	else
	{
		m_window_size = LCD_HEIGHT;
		m_cache_size = SD_CACHE_SIZE;
	}

	window_cache_end = window_cache_begin + m_window_size;

	// Set the edges of the windows
	m_window_min = 0;
	m_window_max = m_window_size - 1;
	m_cache_min = 0;
	m_cache_max = m_cache_size - 1;

	// The content of the cache must be updated
	m_cache_update = true;
	updateCachePosition(0);
}

bool SDCache::updateCachePosition(int16_t index)
{	
	//return_value is set to true if window has moved
    bool return_value = false;
    bool update_window = false;
    
    int16_t upper_index = index;
    int16_t lower_index = index;
    
    if(m_window_is_centered)
    {
		upper_index = index + m_window_offset;
		lower_index = index - m_window_offset;
	}
    
	if(lower_index < 0)
	{
		lower_index = 0;
	}
	else if(upper_index >= m_list_length -1)
	{
		upper_index = m_list_length - 1;
	}
	
	if(index != m_index)
	{
		update_window = true;
		if(lower_index < m_cache_min)
		{
			if(lower_index + m_window_size - m_cache_size + 1 > 0)
			{
				m_cache_max = lower_index + m_window_size - 1;
				m_cache_min = m_cache_max - m_cache_size + 1;
				return_value = true;
			}
			else
			{
				m_cache_min = 0;
                m_cache_max = m_cache_size - 1;
                return_value = true;
			}
			m_cache_update = true;			
		}
		else if(upper_index > m_cache_max)
		{
			m_cache_min = upper_index - m_window_size + 1;
            m_cache_max = m_cache_min + m_cache_size - 1;
            if (m_cache_max >= m_list_length - 1) {
                m_cache_max = m_list_length - 1;
                m_cache_min = m_list_length - m_cache_size;
            }
			m_cache_update = true;
		}
		
		if(m_window_is_centered)
		{
			//cases to be changed
			if(lower_index < m_window_min)
			{
				if(lower_index > 0)
				{
					m_window_min = lower_index;
				} 
				else
				{
					m_window_min = 0;
				}
								
				m_window_max = m_window_min + m_window_size - 1;
			}
			else if(upper_index > m_window_max)
			{
				m_window_max = upper_index;
				if(m_window_max >= m_list_length - 1)
				{
					m_window_max = m_list_length - 1;
				}
				m_window_min = m_window_max - m_window_size + 1;
			}
		}
		else
		{
			if(index < m_window_min)
			{
				m_window_min = index;
				m_window_max = m_window_min + m_window_size - 1;
			}
			else if(index > m_window_max)
			{
				m_window_max = index;
				m_window_min = m_window_max - m_window_size + 1;
			}
		}
		m_selected_file = index - m_window_min;
		
		window_cache_begin = m_cache + (m_window_min - m_cache_min);
		window_cache_end = window_cache_begin + m_window_size;
		
		m_index = index;
	}
	
	if(m_cache_update == true)
	{		
		// Clears the cache content
        memset(m_cache, 0, sizeof(m_cache));
	
        uint8_t i = 0;
        int8_t offset = -1; 
        
        // Cache entry for either "Back" or .. option
        if ((m_cache_min + i) == 0)
        {
			if(getFolderIsRoot())
			{
				m_cache[i].type = BACK_ENTRY;
				strcpy(m_cache[i].longFilename, "Back");
				strcpy(m_cache[i].filename, "Back");
			}
			else
			{
				m_cache[i].type = UPDIR_ENTRY;
				strcpy(m_cache[i].longFilename, "..");
				strcpy(m_cache[i].filename, "..");
			}
			
			offset++;
			i++;
		}

		// Cache entries for the files in the current directory
		int8_t j = 0;
		while (i < m_cache_size)
		{
			card.getfilename(m_cache_min + j + offset);
			
			strcpy(m_cache[i].filename, card.filename);
			if (card.filenameIsDir) 
			{
				m_cache[i].type = FOLDER_ENTRY;
				if(strlen(card.longFilename) == 0)
				{
					strcpy(m_cache[i].longFilename, card.filename);
				}
				else
				{
					strcpy(m_cache[i].longFilename, card.longFilename);
				}
			} 
			else 
			{
				m_cache[i].type = FILE_ENTRY;
				strcpy(m_cache[i].longFilename, card.longFilename);
			}

			i++;
			j++;
		}
	}
	
	if(update_window || m_cache_update)
	{
		m_cache_update = false;
		
		window_cache_begin = m_cache + (m_window_min - m_cache_min);
		window_cache_end = window_cache_begin + m_window_size;

		// The content of the cache is up-to-date
		m_cache_update = false;
	}
	return return_value;
}

CacheEntryType_t SDCache::press(uint16_t index)
{
	//first a check for possible update
	updateCachePosition(index);

	switch(window_cache_begin[m_selected_file].type)
	{
		//cases handled outside of SDCache
		case FILE_ENTRY:
			card.getfilename(m_index-1);
		case BACK_ENTRY:		
			return window_cache_begin[m_selected_file].type; 
			break;
		
		//Cases handled internally
		case UPDIR_ENTRY: 
		case FOLDER_ENTRY: 
				changeDir();
				return CacheEntryType_t::NOACTION; 
				break;
	}
}

void SDCache::returnToRoot()
{
	card.initsd();
	
	m_cache_update = true;
	m_index = 0;
	m_directory_depth = 0;
	
	reloadCache();
}

void SDCache::setWindowCentered()
{
#ifdef DOGLCD 
	m_window_is_centered = true; 
	m_window_offset = 2; 
#endif
}

void SDCache::changeDir()
{
	if(window_cache_begin[m_selected_file].type == FOLDER_ENTRY)
	{
		if(m_directory_depth < MAX_DIR_DEPTH-1)
		{
			++m_directory_depth;
			if(strlen(card.longFilename) != 0)
			{
				strncpy(m_directory, getSelectedEntry()->longFilename, 19);
			}
			else
			{
				strcpy(m_directory, getSelectedEntry()->filename);
			}
			card.chdir(window_cache_begin[m_selected_file].filename);
			
			reloadCache();
		}
	}
	else //return to previous directory
	{
		--m_directory_depth;
		card.updir();
		updateDirectoryName();
		reloadCache();
	}
}

void SDCache::updateDirectoryName()
{
	char curDir[13];
	card.getCurrentDirName();
	strcpy(curDir, card.filename);
	
	if(card.filename[0] == '/')
	{
		m_directory_depth = 0;
		m_directory[0] = '/';
		return;
	}
	
	card.updir();
	uint8_t folderLength = card.getnrfilenames();
	uint8_t i = 0;
	
	while(i < folderLength)
	{
		card.getfilename(i);
		SERIAL_ECHOLN(card.filename);
		if(strcmp(curDir, card.filename) == 0)
		{
			if(strlen(card.longFilename) != 0)
			{
				strncpy(m_directory, card.longFilename, 19);
				m_directory[19] = '\0';
			}
			else
			{
				strcpy(m_directory, card.filename);
			}

			card.chdir(curDir);
			return;
		}
		
		i++;
	}
	
	SERIAL_ECHOLNPGM("dir not found");
}
