**** Pins Gen7 v1.4
----------------------------------------
**** _Bootscreen.h / _Statusscreen.h
----------------------------------------
**** M606.cpp
----------------------------------------
**** Config.h / Config.adv
----------------------------------------
/** Marlin/src/lcd/ultralcd_impl_DOGM.h */ 
ln:323 
  void lcd_bootscreen() {
    #if ENABLED(SHOW_CUSTOM_BOOTSCREEN)
      lcd_custom_bootscreen();
      return;
    #endif
----------------------------------------
/** Marlin/src/feature/fwretract.h */ 
class FWRetract {
  #if ENABLED(BUCKET_FEATURE) 
    public:
  #else
    private:			
  #endif
		
  #if EXTRUDERS > 1
    static bool retracted_swap[EXTRUDERS];         // Which extruders are swap-retracted
  #endif
----------------------------------------
/** Marlin/src/feature/pause.h */ 
#if ENABLED(BUCKET_FEATURE)
  extern int tool_migration_last_target; // Define if tool migration enabled and the last extruder to reach
  extern float bucket_purge_length , bucket_purge_feedrate , bucket_retract ;
  #if DISABLED(FWRETRACT)
    extern float bucket_retract_feedrate
               , bucket_recover_feedrate;
  #endif
  extern int bucket_fan_dwell;
#endif
----------------------------------------
/** Marlin/src/feature/runout.h */ 
ln 39
#if ENABLED(BUCKET_FEATURE)
  #include "../feature/pause.h"
#endif
ln 55
        filament_ran_out = true;
        
        #if ENABLED(BUCKET_FEATURE)
          //If last migration tool enabled
          if (active_extruder < tool_migration_last_target ) {																						
            enqueue_and_echo_commands_P(PSTR("M606"));											
            return;
          }
        #endif
        
        enqueue_and_echo_commands_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
----------------------------------------
/** Marlin/src/gcode/gcode.cpp */ 
ln 643
      #if ENABLED(BUCKET_FEATURE)
        case 606: M606(); break;  // M606: Migrate extruder to another
      #endif
----------------------------------------
/** Marlin/src/gcode/gcode.h */
ln 701
  #if ENABLED(BUCKET_FEATURE)
    static void M606();
  #endif
----------------------------------------
/** Marlin/src/lcd/language/language_en.h */ 
ln 735
#if ENABLED(BUCKET_FEATURE)
  #ifndef MSG_BUCKET_FEATURE
    #define MSG_BUCKET_FEATURE                         _UxGT("Purge Bucket")
  #endif
  #ifndef MSG_BUCKET_FEATURE
    #define MSG_BUCKET_TOOL_MIGRATION                  _UxGT("Tool Migration")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_LAST
    #define MSG_BUCKET_TOOL_MIGRATION_LAST             _UxGT("Final Extruder")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_SWAP
    #define MSG_BUCKET_TOOL_MIGRATION_SWAP             _UxGT("Migrate to ")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_ON
    #define MSG_BUCKET_TOOL_MIGRATION_ON               _UxGT("Auto ON")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_TOOL_MIGRATION_OFF              _UxGT("Auto OFF")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_PURGE_LENGTH                    _UxGT("Purge")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_PURGE_F                         _UxGT("Purge V")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_RETRACT                         _UxGT("Retract")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_RETRACT_F                       _UxGT("Retract V")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_RECOVER_F                       _UxGT("UnRet V")
  #endif
  #ifndef MSG_BUCKET_TOOL_MIGRATION_OFF 
    #define MSG_BUCKET_FAN_DWELL                       _UxGT("Fan dwell")
  #endif
#endif
----------------------------------------
/** Marlin/src/lcd/language/language_fr.h */ ln 275
/**  Marlin/src/lcd/language/language_fr_utf8.h */ ln 275
#if ENABLED(BUCKET_FEATURE)
  #define MSG_BUCKET_FEATURE                _UxGT("Purge Bucket")
  #define MSG_BUCKET_TOOL_MIGRATION         _UxGT("Migration Extrudeur")
  #define MSG_BUCKET_TOOL_MIGRATION_LAST    _UxGT("Dernier Extrudeur")
  #define MSG_BUCKET_TOOL_MIGRATION_SWAP    _UxGT("Migrer vers ")
  #define MSG_BUCKET_TOOL_MIGRATION_ON      _UxGT("Auto ON")
  #define MSG_BUCKET_TOOL_MIGRATION_OFF     _UxGT("Auto OFF")
  #define MSG_BUCKET_PURGE_LENGTH           _UxGT("Purge")
  #define MSG_BUCKET_PURGE_F                _UxGT("Purge V")
  #define MSG_BUCKET_RETRACT                _UxGT("Retrait")
  #define MSG_BUCKET_RETRACT_F              _UxGT("Retrait V")
  #define MSG_BUCKET_RECOVER_F              _UxGT("Rappel V")
  #define MSG_BUCKET_FAN_DWELL              _UxGT("Ventilation")
#endif
----------------------------------------
/** Marlin/src/lcd/ultralcd.cpp */
ln 46
#if ENABLED(BUCKET_FEATURE)
  #include "../feature/pause.h"
  #define BUCKET_FEATURE_COMMAND "M606 T"
  void bucket_feature_submenu() ;	
  void tool_migration_gcode(const char * const cmd);
#endif

ln 235
    void lcd_advanced_pause_resume_message();
  #endif
		
  #if ENABLED (LCD_FILAMENT_LOAD_UNLOAD_MENU)
    void lcd_filament_load_unload();
    void lcd_filament_load_unload_menu();
  #endif

ln 1160
    void _lcd_babystep(const AxisEnum axis, const char* msg) {
      if (use_click()) { return lcd_goto_previous_menu_no_defer(); }
      ENCODER_DIRECTION_NORMAL();
      if (encoderPosition) {
        //const int16_t babystep_increment = (int32_t)encoderPosition * (BABYSTEP_MULTIPLICATOR);
								const int16_t babystep_increment = (int32_t)encoderPosition;
        encoderPosition = 0;
        lcdDrawUpdate = LCDVIEW_REDRAW_NOW;
        thermalManager.babystep_axis(axis, babystep_increment);
        babysteps_done += ( babystep_increment * ( axis == Z_AXIS ? BABYSTEP_MULTIPLICATOR : 1) ) ;
      }
      
ln 1331
        #endif // EXTRUDERS > 4
      #endif // EXTRUDERS > 3
    #endif // EXTRUDERS > 2
  #endif // EXTRUDERS > 1
		
  /**
   * LCD Filament Load/Unload menu
   */
  #if ENABLED (LCD_FILAMENT_LOAD_UNLOAD_MENU)
    void lcd_filament_load_unload(
      const uint8_t extruder
    ) {
      set_destination_from_current();
      const float old_feedrate_mm_s = feedrate_mm_s;

      // Toolchange
      #if  EXTRUDERS > 1
        if (extruder >=40 && (extruder-40) != active_extruder) {
         char *command;
         switch (extruder-40) {
           case 0: command = PSTR("T0"); break;
           case 1: command = PSTR("T1"); break;
           #if EXTRUDERS > 2
             case 2: command = PSTR("T2"); break;
             #if EXTRUDERS > 3
               case 3: command = PSTR("T3"); break;
               #if EXTRUDERS > 4
                 case 4: command = PSTR("T4"); break;
               #endif // EXTRUDERS > 4
             #endif // EXTRUDERS > 3
           #endif // EXTRUDERS > 2
         }
         enqueue_and_echo_commands_P(command);
         lcdDrawUpdate = LCDVIEW_CALL_REDRAW_NEXT;
         }
       #endif //EXTRUDERS > 1

       // Extrude
       if (extruder ==3) {
        current_position[E_AXIS] -=  LCD_LOAD_MENU_EXTRUDE_L / planner.e_factor[active_extruder];
        feedrate_mm_s = LCD_LOAD_MENU_EXTRUDE_F;
        sync_plan_position_e();
        prepare_move_to_destination();
        feedrate_mm_s = old_feedrate_mm_s;
       }
       else
        // Load
        if (extruder ==2) {
        current_position[E_AXIS] -=  LCD_LOAD_MENU_FEEDING_L  / planner.e_factor[active_extruder];
        feedrate_mm_s = LCD_LOAD_MENU_FEEDING_F;
        sync_plan_position_e();
        prepare_move_to_destination();
        feedrate_mm_s = old_feedrate_mm_s;
        }
        else
         // Unload
         if (extruder ==1) {
         current_position[E_AXIS] +=  LCD_LOAD_MENU_UNFEEDING_L / planner.e_factor[active_extruder];
         feedrate_mm_s = LCD_LOAD_MENU_UNFEEDING_F;
         sync_plan_position_e();
         prepare_move_to_destination();
         feedrate_mm_s = old_feedrate_mm_s;
         }
     }	//lcd_filament_load_unload

     void lcd_unload_extruder(){ lcd_filament_load_unload(1); }
     void lcd_load_extruder(){ lcd_filament_load_unload(2); }
     void lcd_extrude_extruder(){ lcd_filament_load_unload(3); }
     #if EXTRUDERS > 1
       void lcd_toolchange_extruder_e0() { lcd_filament_load_unload(40); }
       void lcd_toolchange_extruder_e1() { lcd_filament_load_unload(41); }
       void lcd_toolchange_extruder_e2() { lcd_filament_load_unload(42); }
       void lcd_toolchange_extruder_e3() { lcd_filament_load_unload(43); }
       void lcd_toolchange_extruder_e4() { lcd_filament_load_unload(44); }
     #endif

     void lcd_filament_load_unload_menu() {
       START_MENU();
       MENU_BACK(MSG_MAIN);
       if (!thermalManager.tooColdToExtrude(active_extruder)) {
         #if EXTRUDERS > 1
           switch (active_extruder) {
             case 0: STATIC_ITEM(MSG_MOVE_E " " MSG_E1, true); break;
             case 1: STATIC_ITEM(MSG_MOVE_E " " MSG_E2, true); break;
             #if EXTRUDERS > 2
               case 2: STATIC_ITEM(MSG_MOVE_E " " MSG_E3, true); break;
             #endif
             #if EXTRUDERS > 3
               case 3: STATIC_ITEM(MSG_MOVE_E " " MSG_E4, true); break;
             #endif
             #if EXTRUDERS > 4
               case 4: STATIC_ITEM(MSG_MOVE_E " " MSG_E5, true); break;
             #endif
           }
         #endif
         MENU_ITEM(function,MSG_FILAMENTUNLOAD , lcd_unload_extruder);
         MENU_ITEM(function,MSG_FILAMENTLOAD , lcd_load_extruder);
         MENU_ITEM(function,MSG_EXTRUDE, lcd_extrude_extruder);
       }
       #if EXTRUDERS > 1
         if (active_extruder !=0) MENU_ITEM(function, MSG_PREPARE " " MSG_MOVE_E " " MSG_E1, lcd_toolchange_extruder_e0);
         if (active_extruder !=1) MENU_ITEM(function, MSG_PREPARE " " MSG_MOVE_E " " MSG_E2, lcd_toolchange_extruder_e1);
       #endif
       #if EXTRUDERS > 2
         if (active_extruder !=2) MENU_ITEM(function, MSG_PREPARE " " MSG_MOVE_E " " MSG_E3, lcd_toolchange_extruder_e2);
       #endif
       #if EXTRUDERS > 3
         if (active_extruder !=3) MENU_ITEM(function, MSG_PREPARE " " MSG_MOVE_E " " MSG_E4, lcd_toolchange_extruder_e3);
       #endif
       #if EXTRUDERS > 4
         if (active_extruder !=4) MENU_ITEM(function, MSG_PREPARE " " MSG_MOVE_E " " MSG_E5, lcd_toolchange_extruder_e4);
       #endif
       END_MENU();
     }
   #endif //LCD_FILAMENT_LOAD_UNLOAD_MENU
   
  /**
   *
   * "Tune" submenu*/
   
ln 1460
    //
    // Speed:
    //
    MENU_ITEM_EDIT(int3, MSG_SPEED, &feedrate_percentage, 10, 999);
				
    #if ENABLED(BUCKET_FEATURE)
      MENU_ITEM(submenu, MSG_BUCKET_FEATURE, bucket_feature_submenu); 
    #endif

ln 1564
    //
    // Change filament
    //
    #if ENABLED (LCD_FILAMENT_LOAD_UNLOAD_MENU)
      MENU_ITEM(submenu, MSG_FILAMENTCHANGE , lcd_filament_load_unload_menu);
    #endif

ln 2783
    //
    // Change filament
    //
    #if ENABLED (LCD_FILAMENT_LOAD_UNLOAD_MENU)
      MENU_ITEM(submenu, MSG_FILAMENTCHANGE , lcd_filament_load_unload_menu);
    #endif

ln 3371
  void lcd_control_menu() {
    START_MENU();
    MENU_BACK(MSG_MAIN);
    MENU_ITEM(submenu, MSG_TEMPERATURE, lcd_control_temperature_menu);
    MENU_ITEM(submenu, MSG_MOTION, lcd_control_motion_menu);
				
    #if ENABLED(BUCKET_FEATURE)
      MENU_ITEM(submenu, MSG_BUCKET_FEATURE, bucket_feature_submenu); 
    #endif

ln 3965
#endif // !NO_VOLUMETRICS || ADVANCED_PAUSE_FEATURE
		
 /**
   *
   * "Dynamic Tool Migration"  submenu
   *
   */					
  #if ENABLED(BUCKET_FEATURE)
    //Menu function to have ' return to status after click '
    void tool_migration_gcode(const char * const cmd) {
    enqueue_and_echo_commands_P(cmd);
    lcd_return_to_status();
    }				
    void tool_migration_gcode_T0() {tool_migration_gcode(PSTR(BUCKET_FEATURE_COMMAND "0") );}
    void tool_migration_gcode_T1() {tool_migration_gcode(PSTR(BUCKET_FEATURE_COMMAND "1") );}
    #if EXTRUDERS > 2
      void tool_migration_gcode_T2() {tool_migration_gcode(PSTR(BUCKET_FEATURE_COMMAND "2") );}
    #endif
    #if EXTRUDERS > 3
      void tool_migration_gcode_T3() {tool_migration_gcode(PSTR(BUCKET_FEATURE_COMMAND "3") );}
    #endif
    #if EXTRUDERS > 4
      void tool_migration_gcode_T4() {tool_migration_gcode(PSTR(BUCKET_FEATURE_COMMAND "4") );}
    #endif

    //Submenu
    void bucket_feature_submenu() {	
      //Show Auto on/off
      const char * msg_on_off = tool_migration_last_target? PSTR(MSG_BUCKET_TOOL_MIGRATION_ON): PSTR(MSG_BUCKET_TOOL_MIGRATION_OFF);			

      START_MENU();
      MENU_BACK(MSG_MAIN);
      STATIC_ITEM(MSG_BUCKET_TOOL_MIGRATION, false, false);
      STATIC_ITEM_P(msg_on_off, false, false);
      MENU_ITEM_EDIT(int3,MSG_BUCKET_TOOL_MIGRATION_LAST, &tool_migration_last_target, 0, EXTRUDERS-1);
      if (active_extruder !=0) MENU_ITEM(function, MSG_BUCKET_TOOL_MIGRATION_SWAP MSG_E1,tool_migration_gcode_T0);		
      if (active_extruder !=1) MENU_ITEM(function, MSG_BUCKET_TOOL_MIGRATION_SWAP MSG_E2,tool_migration_gcode_T1);
      #if EXTRUDERS > 2
        if (active_extruder !=2) MENU_ITEM(function, MSG_BUCKET_TOOL_MIGRATION_SWAP MSG_E3,tool_migration_gcode_T2);
      #endif
      #if EXTRUDERS > 3
        if (active_extruder !=3) MENU_ITEM(function, MSG_BUCKET_TOOL_MIGRATION_SWAP MSG_E4,tool_migration_gcode_T3);
      #endif
      #if EXTRUDERS > 4
        if (active_extruder !=4) MENU_ITEM(function, MSG_BUCKET_TOOL_MIGRATION_SWAP MSG_E5,tool_migration_gcode_T4);
      #endif	
      MENU_ITEM_EDIT(float3,MSG_BUCKET_PURGE_LENGTH, &bucket_purge_length, 0, 200);
      MENU_ITEM_EDIT(float3,MSG_BUCKET_PURGE_F, &bucket_purge_feedrate, 0, planner.max_feedrate_mm_s[E_AXIS]);
      MENU_ITEM_EDIT(float52,MSG_BUCKET_RETRACT, &bucket_retract, 0, 20);
      #if DISABLED(FWRETRACT)
        MENU_ITEM_EDIT(float3,MSG_BUCKET_RETRACT_F, &bucket_retract_feedrate, 0, planner.max_feedrate_mm_s[E_AXIS]);
        MENU_ITEM_EDIT(float3,MSG_BUCKET_RECOVER_F, &bucket_recover_feedrate, 0, planner.max_feedrate_mm_s[E_AXIS]);
      #endif
      MENU_ITEM_EDIT(int3,MSG_BUCKET_FAN_DWELL, &bucket_fan_dwell, 0, 20);
      END_MENU();
    }
  #endif

  /**
   *
   * "Control" > "Retract" submenu
		*/	
			ln 3593
			 #else // !PID_PARAMS_PER_HOTEND || HOTENDS == 1
        //PID_MENU_ITEMS("", 0);
      #endif // !PID_PARAMS_PER_HOTEND || HOTENDS == 1
----------------------------------------
/** Marlin/src/feature/fwretract.h */
----------------------------------------
/** Marlin/src/feature/fwretract.h */ 
----------------------------------------
/** Marlin/src/feature/fwretract.h */ 
----------------------------------------
/** Marlin/src/feature/fwretract.h */ 