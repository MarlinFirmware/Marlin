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
#include "cardreader.h"
#include "Language.h"

namespace screen
{
	uint8_t ScreenList::directory_index = 0;
	uint8_t ScreenList::directory_array[10] = { 0 };
	bool ScreenList::from_updir = false;

	ScreenList::ScreenList(const char * title, Subject<bool> * model)
		: Screen(title, LIST)
		, Observer<bool>(model)
		, m_index(0)
		, m_icon_index(0)
		, m_num_list(0)
		, m_num_item_added(0)
	{
		memset(m_directory, 0, sizeof(m_directory));
		m_directory_is_root = false;
		m_previous_time = millis();
	}

	ScreenList::~ScreenList()
	{ }

	void ScreenList::init(uint16_t index)
	{
		if(SDManager::single::instance().isInserted() == false)
		{
			ViewManager::getInstance().activeView(m_back_screen);
		}
		else 
		{
			m_num_list = card.getnrfilenames();
			m_index = 0;

			card.getWorkDirName();
			strncpy(m_directory, card.filename, 19);
			m_directory[19] = '\0';

			if (card.filename[0] != '/')
			{
				if(from_updir)
				{
					if(directory_index > 0)
					{
						char prev_folder[20];
						strncpy(prev_folder, card.filename, 19);
						card.updir();
						card.getfilename(directory_array[directory_index-1]-1);
						if ( (card.longFilename != NULL) && (strlen(card.longFilename) > 0) )
						{
							strncpy(m_directory, card.longFilename, sizeof(m_directory));
						}
						else
						{
							strncpy(m_directory, card.filename, sizeof(m_directory));
						}
						card.chdir(prev_folder);
						painter.print(m_directory);
						from_updir = false;
					}
				}
				else
				{
					if ( (card.folderName != NULL) && (strlen(card.folderName) > 0) )
					{
						strncpy(m_directory, card.folderName, 19);
					}
					else
					{
						strncpy(m_directory, card.filename, 19);
					}
				}
				m_directory_is_root = false;
			}
			else
			{
				m_directory_is_root = true;
			}

			m_num_list++;
			m_scroll_size = (float) 47 / m_num_list;
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
		if ( m_index == (m_num_list -1) )
		{
			m_index = m_num_list -1;
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
		painter.firstPage();
		do
		{
			// Draw title
			uint8_t x_init = painter.coordinateXInit();
			uint8_t y_init = painter.coordinateYInit();
			uint8_t x_end = painter.coordinateXEnd();

			if (m_directory_is_root == true)
			{
				painter.setColorIndex(1);
				painter.setFont(u8g_font_6x9);
				painter.setPrintPos(x_init, y_init + 3);
				painter.print("/");
				painter.setPrintPos(x_init + 6, y_init + 3);
				painter.print_P(m_title);
				memset(directory_array,0,sizeof(directory_array));
				directory_index = 0;
			}
			else
			{
				painter.setColorIndex(1);
				painter.setFont(u8g_font_6x9);
				painter.drawBitmap(x_init, y_init + 3, little_icon_width, little_icon_height, bits_updir_small);
				painter.setPrintPos(x_init + 6, y_init + 3);
				painter.print("/");
				painter.setPrintPos(x_init + 12, y_init + 3);
				painter.print(m_directory);
			}

			//Draw line separator
			painter.drawLine(x_init, y_init + 13, x_end, y_init + 13);
			painter.coordinateYInit(14);

			// Draw list
			uint8_t window_size = 50 / (max_font_height + 1);
			uint8_t window_selector = window_size / 2;
			if (window_size % 2 == 0)
			{
				window_selector--;
			}

			for (uint8_t i = 0; i < window_size; i++)
			{
				if (i == window_selector)
				{
					painter.setColorIndex(1);
					painter.drawBox(painter.coordinateXInit(), painter.coordinateYInit() + i * (max_font_height + 1), 128, max_font_height);
					painter.setColorIndex(0);
				}
				else
				{
					painter.setColorIndex(1);
				}

				if ((int)(m_index + i - window_selector) < 0 || (m_index + i - window_selector) >= m_num_list)
				{
					continue;
				}

				if ((m_index + i - window_selector) == 0)
				{
					if(m_directory_is_root)
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
					card.getfilename(m_index + i - window_selector - 1);

					if (card.filenameIsDir == true)
					{
						painter.drawBitmap(painter.coordinateXInit() + 1, painter.coordinateYInit() + i * (max_font_height + 1), little_icon_width, little_icon_height, bits_folder_small);
					}
					painter.setPrintPos(painter.coordinateXInit() + 9, painter.coordinateYInit() + i * (max_font_height + 1));
					if(strcmp(card.longFilename,"") != 0)
					{
						if (i == window_selector)
						{
							m_current_time = millis();
							if (m_current_time > m_previous_time + 1200)
							{
								painter.animate(card.longFilename, 18, 100);
							}
							else
							{
								painter.print(card.longFilename);
							}
						}
						else
						{
							painter.print(card.longFilename);
						}
					}
					else
					{
						if (i == window_selector)
						{
							m_current_time = millis();
							if (m_current_time > m_previous_time + 1200)
							{
								painter.animate(card.filename, 18, 100);
							}
							else
							{
								painter.print(card.filename);
							}
						}
						else
						{
							painter.print(card.filename);
						}
					}
				}
			}

			// Draw scroll bar
			painter.setColorIndex(1);
			painter.drawBox(122, 13, 6, 51);
			painter.setColorIndex(0);
			painter.drawBox(123, 14, 4, 49);

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
		if (m_index == 0)
		{
			if (m_directory_is_root == true)
			{
				directory_index = 0;
				ViewManager::getInstance().activeView(m_back_screen);
				return;
			}
			else
			{
				directory_index--;
				from_updir = true;

				card.updir();
				ViewManager::getInstance().activeView(screen_SD_list);
				return;
			}
		}
		else
		{
			card.getfilename(m_index - 1);
			if (card.filenameIsDir == true)
			{
				if(directory_index < 9)
				{
					directory_array[directory_index] = m_index;
					directory_index++;
					from_updir = false;
				}
				card.chdir(card.filename);
				ViewManager::getInstance().activeView(screen_SD_list);
				return;
			}
			ViewManager::getInstance().activeView(m_next_screen);
		}
	}

	void ScreenList::add(ScreenIndex_t const & view)
	{
		if (m_num_item_added % 2)
		{
			m_next_screen = view;
		}
		else
		{
			m_back_screen = view;
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
