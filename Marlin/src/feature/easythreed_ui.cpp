/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "easythreed_ui.h"
#include "../inc/MarlinConfig.h"
#include "../module/temperature.h"
#include "pause.h"
#include "../module/printcounter.h"
#include "../sd/cardreader.h"
#include "../gcode/queue.h"
#include "../module/motion.h"
#include "../module/planner.h"
#include "../MarlinCore.h"


EasythreedUI easythreed_ui;

void EasythreedUI::setPins() {
	#if ENABLED(EASYTHREED_ET4000PLUS)
		SET_INPUT_PULLUP(PRINT_HOME_PIN);
		SET_OUTPUT(HOME_GND_PIN);
		SET_INPUT_PULLUP(FEED_PIN);
		SET_INPUT_PULLUP(RETRACT_PIN);
		SET_OUTPUT(FEED_GND_PIN);
		SET_OUTPUT(RETRACT_GND_PIN);
		SET_INPUT_PULLUP(PRINTER_PIN);
		SET_OUTPUT(PRINT_LED_PIN);     
	#endif
  };

void EasythreedUI::UICheck(void)
{
	#if ENABLED(EASYTHREED_ET4000PLUS)
		NanoBlinkLed();
		NanoLoadFilament();
		NanoPrintOneKey();
	#endif
}

// EasyThreeD ET4000+ Mainboard for Nano / K7
#if ENABLED(EASYTHREED_ET4000PLUS)
	// Status LED on Start button
	uint16_t blink_time = 0;  
	enum LED_STATUS {
		LED_ON=4000,
		LED_BLINK_0=2500,
		LED_BLINK_1=1500,
		LED_BLINK_2=1000,
		LED_BLINK_3=800,
		LED_BLINK_4=500,
		LED_BLINK_5=300,
		LED_BLINK_6=150,
		LED_BLINK_7=50,
		LED_OFF = 0,
	};
	#define BLINK_LED(MS)  blink_time = MS

	void EasythreedUI::NanoBlinkLed(void) {
		static uint32_t blink_time_previous=0;
		static uint32_t blink_time_start=0;
		
		if(blink_time == 0)  { //OFF
			WRITE(PRINT_LED_PIN,1);
			return;
		}
		if(blink_time > 3000) { //ON
			WRITE(PRINT_LED_PIN,0);
			return;
		}
		if(blink_time_previous!=blink_time) {
			blink_time_previous = blink_time;
			blink_time_start = millis();
		}
		if(millis()<blink_time_start+blink_time) {
			WRITE(PRINT_LED_PIN,0);
		}
		else if(millis()<blink_time_start+2*blink_time) {
			WRITE(PRINT_LED_PIN,1);
		}
		else {
			blink_time_start = millis();
		}	
	}

	// Filament Load/Unload Switch
	static uint32_t filament_time = 0;
	static uint8_t filament_status = 0;

	void EasythreedUI::NanoLoadFilament(void) {       
		if(printingIsActive()) {
			return;
		}
		if(filament_status == 0) {  
			if((!READ(RETRACT_PIN))||(!READ(FEED_PIN))) {                                                                           
				filament_status++;                                                        
				filament_time = millis();                                                 
			}                                                                           
		} else if(filament_status == 1) {                            
			if(filament_time+20<millis()) {                                                                         
			if((!READ(RETRACT_PIN))||(!READ(FEED_PIN))) { 
				thermalManager.setTargetHotend(210, 0);  
				BLINK_LED(LED_BLINK_7);                             
				filament_status++;                                                    
			} else {                                                                       
				filament_status = 0;                                                  
			}                                                                       
			}	                                                                        
		} else if(filament_status == 2) {
			if( thermalManager.degHotend(0) >= float(180)) { 
				filament_status++;
				BLINK_LED(LED_BLINK_5);
			}
			if((READ(RETRACT_PIN))&&(READ(FEED_PIN))) {                         
			BLINK_LED(LED_ON);
			filament_status = 0;                                                          
			thermalManager.disable_all_heaters();                                         
			}                                                                             
		} else if(filament_status == 3) { 
			static uint8_t flag = 0; 
			if(!READ(RETRACT_PIN)) { 
			if(flag == 0) { 
				queue.inject_P("G91\nG0 E+10  F180\nG0 E-120 F180\nM104 S0");            
				BLINK_LED(LED_BLINK_5);
				flag = 1; 
			} 
			}
			if(!READ(FEED_PIN)) { 
			if(flag ==0) { 
				queue.inject_P("G91\nG0 E+100 F120\nM104 S0");
				BLINK_LED(LED_BLINK_5);
				flag = 1; 
			} 
			}
			if((READ(RETRACT_PIN))&&(READ(FEED_PIN))) { 
			flag = 0; 
			filament_status = 0;  
			quickstop_stepper();  
			planner.cleaning_buffer_counter=2;
			thermalManager.disable_all_heaters(); 
			} 
		} else { 
			filament_status = 0;  
		}	
	} //End of LoadFilament

	// Start Button
	uint8_t print_key_flag = 0; 
	uint8_t print_pause = 0;

	void EasythreedUI::NanoPrintOneKey(void)
	{
		static uint8_t key_status=0;
		static uint32_t key_time = 0;
		static uint8_t print_flag = 0;

		if(key_status == 0) {
			if(!READ(PRINTER_PIN)) {
				key_time = millis();
				key_status = 1;
			}
			if(print_flag!=0 && !printingIsActive()) {
				print_key_flag = 0;
				print_flag = 0;
				BLINK_LED(LED_ON); 
				}
			if(print_flag == 1) {
				BLINK_LED(LED_BLINK_5);
				}
		}
		else if(key_status == 1) {
			if(key_time+30<millis()) {
				if(!READ(PRINTER_PIN)) {
					key_time = millis();
					key_status = 2;
				}
				else {
					key_status = 0;
				}
			}	
		}
		else if(key_status == 2)  {
			if(READ(PRINTER_PIN)) {
				if(key_time + 1200 > millis()) { //short press print
					BLINK_LED(LED_BLINK_5);
					if(print_key_flag == 0) {
						if(!printingIsActive()) {
							print_flag = 1;
							card.mount();
							if(!card.isMounted) {
								BLINK_LED(LED_OFF); 
								key_status = 0;
								key_time = 0;
								print_flag = 0;
								return;
							}
							print_key_flag = 1; 
							card.ls(); 
							uint16_t filecnt = card.countFilesInWorkDir();
							if(filecnt==0) return;
							card.selectFileByIndex(filecnt);
							card.openAndPrintFile(card.filename);
						}
					}
					else {
						print_key_flag = 0;
					}							
				}
				else {
					BLINK_LED(LED_BLINK_5);
					if(print_key_flag==0) { //long press Z up 10mm
						queue.inject_P("G91\nG0 Z+10 F600\nG90");
					}
					else {	if(wait_for_heatup) {
						wait_for_heatup=false;
					}
					quickstop_stepper();  
					planner.cleaning_buffer_counter=2;
					thermalManager.disable_all_heaters(); 
					print_flag = 0;
					BLINK_LED(LED_ON);
					}
					planner.synchronize();
					void disableStepperDrivers();
					print_key_flag = 0;	
				}
				key_status = 0;
				key_time = 0;
			}	
		}
		else {
			key_status = 0;
			key_time = 0;
		}
	} // End Start Button
#endif // End EasyThreeD ET4000+
