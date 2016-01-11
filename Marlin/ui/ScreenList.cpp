///////////////////////////////////////////////////////////////////////////////
/// \file ScreenList.cpp
///
/// \author Ivan Galvez Junquera
///         Ruy Garcia
///         Victor Andueza 
///         Joaquin Herrero
///
/// \brief Implementation of SD screen list class.
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

#include "ScreenList.h"
#include "Language.h"


namespace ui
{
	ScreenList::ScreenList(const char * title, Subject<bool> * model)
		: Screen(title, LIST)
		, Observer<bool>(model)
		, m_index(0)
		, m_num_item_added(0)
		, m_browsing_cache(0)
	{
		m_previous_time = millis();
	}

	ScreenList::~ScreenList()
	{ 
		delete m_browsing_cache;
	}

	void ScreenList::init(uint16_t index)
	{
		if(SDManager::single::instance().isInserted() == false)
		{
			ViewManager::getInstance().activeView(m_back_screen);
		}
		else 
		{
			m_browsing_cache = new SDCache();
			m_browsing_cache->setWindowCentered();
			m_browsing_cache->reloadCache();
		}
	}

	void ScreenList::left()
	{
		if (m_index == 0)
		{
			m_index = 0;
		}
		else
		{
			--m_index;
		}
		painter.animationReset(2000);
		m_previous_time = millis();
	}

	void ScreenList::right()
	{
		if ( m_index == (m_browsing_cache->getListLength() -1) )
		{
			m_index = m_browsing_cache->getListLength() -1;
		}
		else
		{
			++m_index;
		}
		painter.animationReset(2000);
		m_previous_time = millis();
	}

	void ScreenList::draw()
	{
		m_browsing_cache->updateCachePosition(m_index);
		uint16_t list_length = m_browsing_cache->getListLength();
		painter.firstPage();
		do
		{
			// Draw title
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t x_end = painter.coordinateXEnd();			
			
			if (m_browsing_cache->getFolderIsRoot() == true)
			{
				painter.setColorIndex(1);
				painter.setFont(FontType_t::BODY_FONT);
				painter.setPrintPos(x_init, y_init + 3);
				painter.print("/");
				painter.setPrintPos(x_init + 6, y_init + 3);
				painter.print_P(m_title);
			}
			else
			{
				painter.setColorIndex(1);
				painter.setFont(FontType_t::BODY_FONT);
				painter.drawBitmap(x_init, y_init + 3, little_icon_width, little_icon_height, bits_updir_small);
				painter.setPrintPos(x_init + 6, y_init + 3);
				painter.print("/");
				painter.setPrintPos(x_init + 12, y_init + 3);
				painter.print(m_browsing_cache->getDirectoryName());
			}

			//Draw line separator
			painter.drawLine(x_init, y_init + 13, x_end, y_init + 13);
			painter.coordinateYInit(14);

			int8_t i = 0;
			uint8_t screen_end = m_browsing_cache->getWindowSize();
			const cache_entry * entry = m_browsing_cache->window_cache_begin;	
			
			if(m_browsing_cache->showingFirstItem())
			{
				i += 2 - m_browsing_cache->getIndex();
				screen_end += i;
			}
			else if(m_browsing_cache->showingLastItem()) 
			{
				i -= 3 - (list_length - m_browsing_cache->getIndex());
			}
			
			for(int8_t j = 0 ; (i != screen_end) && (entry != m_browsing_cache->window_cache_end) ; i++, j++, entry++)
			{
				//Skip entries left outside window drawing space
				if(i < 0)
				{
					continue;
				}
				
				if (entry == m_browsing_cache->getSelectedEntry())
				{
					painter.setColorIndex(1);
					painter.drawBox(painter.coordinateXInit(), painter.coordinateYInit() + i * (max_font_height + 1), 128, max_font_height);
					painter.setColorIndex(0);
				}
				else
				{
					painter.setColorIndex(1);
				}
				
				if (m_browsing_cache->showingFirstItem() && j == 0)
				{
					if(entry->type == CacheEntryType_t::BACK_ENTRY)
					{
						painter.drawBitmap(painter.coordinateXInit() + 1, painter.coordinateYInit() + i * (max_font_height + 1), little_icon_width, little_icon_height, bits_back_small);
						painter.setPrintPos(painter.coordinateXInit() + 9, painter.coordinateYInit() + i * (max_font_height + 1));
						painter.print_P(MSG_SCREEN_SD_LIST_BACK());
					}
					else
					{
						painter.drawBitmap(painter.coordinateXInit() + 1, painter.coordinateYInit() + i * (max_font_height + 1), little_icon_width, little_icon_height, bits_updir_small);
						painter.setPrintPos(painter.coordinateXInit() + 9, painter.coordinateYInit() + i * (max_font_height + 1));
						painter.print_P(MSG_SCREEN_SD_LIST_PREV());
					}
				}
				else 
				{
					if (entry->type == FOLDER_ENTRY)
					{
						painter.drawBitmap(painter.coordinateXInit() + 1, painter.coordinateYInit() + i * (max_font_height + 1), little_icon_width, little_icon_height, bits_folder_small);
					}
					painter.setPrintPos(painter.coordinateXInit() + 9, painter.coordinateYInit() + i * (max_font_height + 1));
					if (entry == m_browsing_cache->getSelectedEntry())
					{
						m_current_time = millis();
						if (m_current_time > m_previous_time + 1200)
						{
							painter.animate(entry->longFilename, 18, 300);
						}
						else
						{
							painter.print(entry->longFilename);
						}
					}
					else
					{
						painter.print(entry->longFilename);
					}
				}
			}

			// Draw scroll bar
			painter.setColorIndex(1);
			painter.drawBox(122, 13, 6, 51);
			painter.setColorIndex(0);
			painter.drawBox(123, 14, 4, 49);
			
			m_scroll_size = (float) 47 / m_browsing_cache->getListLength();

			int8_t scroll_bottom_bar = (m_index + 1) * m_scroll_size;
			if (scroll_bottom_bar < 1)
			{
				scroll_bottom_bar = 1;
			}
			painter.setColorIndex(1);
			painter.drawBox(124, 15, 2, scroll_bottom_bar);

			int8_t scroll_upper_bar = scroll_bottom_bar - m_scroll_size;
			if (scroll_upper_bar > 0)
			{
				painter.setColorIndex(0);
				painter.drawBox(124, 15, 2, scroll_upper_bar);
			}
		} while( painter.nextPage() );
	}

	void ScreenList::press()
	{
		bool maxDirReached = m_browsing_cache->maxDirectoryReached();
		CacheEntryType_t type = m_browsing_cache->press(m_index);
		
		if(type == CacheEntryType_t::INVALID_NAME)
		{
			ViewManager::getInstance().activeView(m_error_screen);
		}
		else if(type == CacheEntryType_t::BACK_ENTRY)
		{
			ViewManager::getInstance().activeView(m_back_screen);
		}
		else if(type == CacheEntryType_t::FILE_ENTRY)
		{
			ViewManager::getInstance().activeView(m_next_screen);
		}
		else if(!maxDirReached)
		{
			m_index = 0;
		}
	}

	void ScreenList::add(ScreenIndex_t const & view)
	{
		if (m_num_item_added == 0)
		{
			m_back_screen = view;
		}
		else if(m_num_item_added == 1)
		{
			m_next_screen = view;
		}
		else if(m_num_item_added == 2)
		{
			m_error_screen = view;
		}
		m_num_item_added++;
	}

	void ScreenList::update(bool is_inserted)
	{
		if(is_inserted == false)
		{
			card.release();
			ViewManager::getInstance().activeView(m_back_screen);
		}
	}
}
