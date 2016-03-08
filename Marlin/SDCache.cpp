///////////////////////////////////////////////////////////////////////////////
/// \file SDCache.cpp
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

#include "SDCache.h"
#include "cardreader.h"

SDCache::SDCache()
	: m_cache_update(true)
	, m_index(0)
	, m_cache_size(0)
	, m_list_length(0)
	, m_selected_file(0)
	, m_window_size(0) 
	, m_directory_depth(0)
	, m_window_is_centered(false)
	, m_window_offset(3)
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
    
    if(m_window_is_centered == true)
	{
		m_window_min = index - m_window_offset;
		m_window_max = index + m_window_offset;
	}
	else
	{
		if(index > m_window_max)
		{
			m_window_max = index;
			m_window_min = m_window_max - (m_window_size-1);
		}
		else if(index < m_window_min)
		{
			m_window_min = index;
			m_window_max = m_window_min + (m_window_size-1);
		}
	}
	
	if( (m_window_is_centered == true && index < m_window_offset)
	|| (m_window_is_centered == false && m_window_min < 0) )
	{
		m_window_min = 0;
		m_window_max = m_window_size - 1;
	}
	else if ( (m_window_is_centered == true && index + m_window_offset > m_list_length-2)
	|| (m_window_is_centered == false && m_window_max > m_list_length - 2))
	{
		m_window_max = m_list_length - 1;
		m_window_min = m_window_max - (m_window_size - 1);
	}
	
	if(index != m_index)
	{
		update_window = true;
		
		if (m_window_min < m_cache_min)
		{
			if(m_window_max >= m_cache_size)
			{
				m_cache_max = m_window_max;
				m_cache_min = m_cache_max - (m_cache_size - 1);
				return_value = true;
				m_cache_update = true;
			}
			else
			{
				m_cache_min = 0;
				m_cache_max = m_cache_min + (m_cache_size - 1);
				return_value = true;
				m_cache_update = true;	
			}
			
		}
		else if(m_window_max > m_cache_max)
		{
			if(m_window_min <= m_list_length - m_cache_size)
			{
				m_cache_min = m_window_min;
				m_cache_max = m_cache_min + (m_cache_size - 1);
				return_value = true;
				m_cache_update = true;
			}
			else
			{
				m_cache_max = m_list_length-1;
				m_cache_min = m_cache_max - (m_cache_size - 1);
				return_value = true;
				m_cache_update = true;
			}
		}
		
		window_cache_begin = m_cache + (m_window_min - m_cache_min);
		window_cache_end = window_cache_begin + m_window_size;
		
		m_selected_file = index - m_cache_min;
		m_index = index;
		
	}
	
	if(m_cache_update == true)
	{		
		memset(m_cache, 0, sizeof(m_cache));
		
		int8_t cache_position = m_cache_size;
		uint16_t file_to_read = m_list_length - m_cache_max - 1; // + 2;
		
		while(cache_position > 0)
		{
			if(file_to_read == m_list_length-1)
			{
				if(getFolderIsRoot())
				{
					m_cache[0].type = BACK_ENTRY;
					strcpy(m_cache[0].longFilename, "Back");
					strcpy(m_cache[0].filename, "Back");
				}
				else
				{
					m_cache[0].type = UPDIR_ENTRY;
					strcpy(m_cache[0].longFilename, "..");
					strcpy(m_cache[0].filename, "..");
				}
				
				break;
			}			
			
			card.getfilename(file_to_read);
			
			
			if (card.filenameIsDir) 
			{
				m_cache[cache_position-1].type = FOLDER_ENTRY;				
			}
			else 
			{
				m_cache[cache_position-1].type = FILE_ENTRY;
			}
			
			strcpy(m_cache[cache_position-1].filename, card.filename);
			if(strlen(card.longFilename) == 0)
			{
				strcpy(m_cache[cache_position-1].longFilename, card.filename);
			}
			else
			{
				strcpy(m_cache[cache_position-1].longFilename, card.longFilename);
			}
						
			++file_to_read;
			--cache_position;
			
			
			
		}
	}
	
	if(update_window || m_cache_update)
	{		
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

	char* c;
	char cmd[LONG_FILENAME_LENGTH];
	strcpy(cmd, getSelectedEntry()->longFilename);
	
	//check first for name validity
	for (c = &cmd[0]; *c; c++)
	{
		if ((uint8_t)*c > 127)
		{
			return CacheEntryType_t::INVALID_NAME;
		}
	}
		
	switch(getSelectedEntry()->type)
	{
		//cases handled outside of SDCache
		case FILE_ENTRY:
			card.getfilename(m_list_length - m_index - 1);
		case BACK_ENTRY:
			return getSelectedEntry()->type; 
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
	if(getSelectedEntry()->type == FOLDER_ENTRY)
	{
		if(m_directory_depth < MAX_DIR_DEPTH-1)
		{
			++m_directory_depth;
			if(strlen(getSelectedEntry()->longFilename) != 0)
			{
				strncpy(m_directory, getSelectedEntry()->longFilename, 19);
			}
			else
			{
				strcpy(m_directory, getSelectedEntry()->filename);
			}
			card.chdir(getSelectedEntry()->filename);
			
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
	
	SERIAL_ECHOLNPGM("folder not found");
}
