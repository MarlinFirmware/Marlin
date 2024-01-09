/// @file
#pragma once
#include "src/MarlinCore.h"
#if HAS_PRUSA_MMU3
#include "mmu2_state.h"
#include "mmu2_marlin.h"

#ifdef __AVR__
  #include "mmu2_protocol_logic.h"
typedef float feedRate_t;

#else
  #include "mmu2_protocol_logic.h"
  // #include <atomic>
#endif

// struct E_Step;

struct E_Step {
  float extrude;  ///< extrude distance in mm
  float feedRate; ///< feed rate in mm/s
};

static constexpr E_Step ramming_sequence[] PROGMEM = {MMU2_RAMMING_SEQUENCE};

static constexpr E_Step load_to_nozzle_sequence[] PROGMEM = {MMU2_LOAD_TO_NOZZLE_SEQUENCE};

namespace MMU2 {

// general MMU setup for MK3
enum : uint8_t {
  FILAMENT_UNKNOWN = 0xffU
};

struct Version {
  uint8_t major, minor, build;
};

/// Top-level interface between Logic and Marlin.
/// Intentionally named MMU2 to be (almost) a drop-in replacement for the previous implementation.
/// Most of the public methods share the original naming convention as well.
class MMU2 {
public:
  MMU2();

  /// Powers ON the MMU, then initializes the UART and protocol logic
  void Start();

  /// Stops the protocol logic, closes the UART, powers OFF the MMU
  void Stop();

  /// Serial output of MMU state
  void Status();

  inline xState State() const { return state; }

  inline bool Enabled() const { return State() == xState::Active; }

  /// Different levels of resetting the MMU
  enum ResetForm : uint8_t {
    Software = 0,     ///< sends a X0 command into the MMU, the MMU will watchdog-reset itself
    ResetPin = 1,     ///< trigger the reset pin of the MMU
    CutThePower = 2,  ///< power off and power on (that includes +5V and +24V power lines)
    EraseEEPROM = 42, ///< erase MMU EEPROM and then perform a software reset
  };

  /// Saved print state on error.
  enum SavedState : uint8_t {
    None = 0,         // No state saved.
    ParkExtruder = 1, // The extruder was parked.
    Cooldown = 2,     // The extruder was allowed to cool.
    CooldownPending = 4,
  };

  /// Source of operation error
  enum ErrorSource : uint8_t {
    ErrorSourcePrinter = 0,
    ErrorSourceMMU = 1,
    ErrorSourceNone = 0xFF,
  };

  /// Tune value in MMU registers as a way to recover from errors
  /// e.g. Idler Stallguard threshold
  void Tune();

  /// Perform a reset of the MMU
  /// @param level physical form of the reset
  void Reset(ResetForm level);

  /// Power off the MMU (cut the power)
  void PowerOff();

  /// Power on the MMU
  void PowerOn();

  /// Read from a MMU register (See gcode M707)
  /// @param address Address of register in hexidecimal
  /// @returns true upon success
  bool ReadRegister(uint8_t address);

  /// Write from a MMU register (See gcode M708)
  /// @param address Address of register in hexidecimal
  /// @param data Data to write to register
  /// @returns true upon success
  bool WriteRegister(uint8_t address, uint16_t data);

  /// The main loop of MMU processing.
  /// Doesn't loop (block) inside, performs just one step of logic state machines.
  /// Also, internally it prevents recursive entries.
  void mmu_loop();

  /// The main MMU command - select a different slot
  /// @param slot of the slot to be selected
  /// @returns false if the operation cannot be performed (Stopped)
  bool tool_change(uint8_t slot);

  /// Handling of special Tx, Tc, T? commands
  bool tool_change(char code, uint8_t slot);

  /// Unload of filament in collaboration with the MMU.
  /// That includes rotating the printer's extruder in order to release filament.
  /// @returns false if the operation cannot be performed (Stopped or cold extruder)
  bool unload();

  /// Load (insert) filament just into the MMU (not into printer's nozzle)
  /// @returns false if the operation cannot be performed (Stopped)
  bool load_filament(uint8_t slot);

  /// Load (push) filament from the MMU into the printer's nozzle
  /// @returns false if the operation cannot be performed (Stopped or cold extruder)
  bool load_filament_to_nozzle(uint8_t slot);

  /// Move MMU's selector aside and push the selected filament forward.
  /// Usable for improving filament's tip or pulling the remaining piece of filament out completely.
  bool eject_filament(uint8_t slot, bool enableFullScreenMsg = true);

  /// Issue a Cut command into the MMU
  /// Requires unloaded filament from the printer (obviously)
  /// @returns false if the operation cannot be performed (Stopped)
  bool cut_filament(uint8_t slot, bool enableFullScreenMsg = true);

  /// Issue a planned request for statistics data from MMU
  void get_statistics();

  /// Issue a Try-Load command
  /// It behaves very similarly like a ToolChange, but it doesn't load the filament
  /// all the way down to the nozzle. The sole purpose of this operation
  /// is to check, that the filament will be ready for printing.
  /// @param slot index of slot to be tested
  /// @returns true
  bool loading_test(uint8_t slot);

  /// @returns the active filament slot index (0-4) or 0xff in case of no active tool
  uint8_t get_current_tool() const;

  /// @returns The filament slot index (0 to 4) that will be loaded next, 0xff in case of no active tool change
  uint8_t get_tool_change_tool() const;

  bool set_filament_type(uint8_t slot, uint8_t type);

  /// Issue a "button" click into the MMU - to be used from Error screens of the MMU
  /// to select one of the 3 possible options to resolve the issue
  void Button(uint8_t index);

  /// Issue an explicit "homing" command into the MMU
  void Home(uint8_t mode);

  /// @returns current state of FINDA (true=filament present, false=filament not present)
  inline bool FindaDetectsFilament() const { return logic.FindaPressed(); }

  inline uint16_t TotalFailStatistics() const { return logic.FailStatistics(); }

  /// @returns Current error code
  inline ErrorCode MMUCurrentErrorCode() const { return logic.Error(); }

  /// @returns Command in progress
  inline uint8_t GetCommandInProgress() const { return logic.CommandInProgress(); }

  /// @returns Last error source
  inline ErrorSource MMULastErrorSource() const { return lastErrorSource; }

  /// @returns Last error code
  inline ErrorCode GetLastErrorCode() const { return lastErrorCode; }

  /// @returns the version of the connected MMU FW.
  /// In the future we'll return the trully detected FW version
  Version GetMMUFWVersion() const {
    if (State() == xState::Active) {
      return { logic.MmuFwVersionMajor(), logic.MmuFwVersionMinor(), logic.MmuFwVersionRevision() };
    } else {
      return { 0, 0, 0 };
    }
  }

  /// Method to read-only mmu_print_saved
  inline bool MMU_PRINT_SAVED() const { return mmu_print_saved != SavedState::None; }

  /// Automagically "press" a Retry button if we have any retry attempts left
  /// @param ec ErrorCode enum value
  /// @returns true if auto-retry is ongoing, false when retry is unavailable or retry attempts are all used up
  bool RetryIfPossible(ErrorCode ec);

  /// @return count for toolchange in current print
  inline uint16_t ToolChangeCounter() const { return toolchange_counter; };

  /// Set toolchange counter to zero
  inline void ClearToolChangeCounter() { toolchange_counter = 0; };

  inline uint16_t TMCFailures() const { return tmcFailures; }
  inline void IncrementTMCFailures() { ++tmcFailures; }
  inline void ClearTMCFailures() { tmcFailures = 0; }

  /// Retrieve cached value parsed from ReadRegister()
  /// or using M707
  inline uint16_t GetLastReadRegisterValue() const {
    return lastReadRegisterValue;
  };
  inline void InvokeErrorScreen(ErrorCode ec) {
    // The printer may not raise an error when the MMU is busy
    if (!logic.CommandInProgress()                // MMU must not be busy
      && MMUCurrentErrorCode() == ErrorCode::OK // The protocol must not be in error state
      && lastErrorCode != ec)                   // The error code is not a duplicate
    {
      ReportError(ec, ErrorSource::ErrorSourcePrinter);
    }
  }

  void ClearPrinterError() {
    logic.ClearPrinterError();
    lastErrorCode = ErrorCode::OK;
    lastErrorSource = ErrorSource::ErrorSourceNone;
  }

  /// @brief Queue a button operation which the printer can act upon
  /// @param btn Button operation
  inline void SetPrinterButtonOperation(Buttons btn) {
    printerButtonOperation = btn;
  }

  /// @brief Get the printer button operation
  /// @return currently set printer button operation, it can be NoButton if nothing is queued
  inline Buttons GetPrinterButtonOperation() {
    return printerButtonOperation;
  }

  inline void ClearPrinterButtonOperation() {
    printerButtonOperation = Buttons::NoButton;
  }

  static uint8_t cutter_mode;     // mode 0:disabled | 1:enabled | 2:always (EXPERIMENTAL)
  static int cutter_mode_addr;    // EEPROM addr for cutter enabled setting
  static uint8_t stealth_mode;    // stealth mode
  static int stealth_mode_addr;   // EEPROM addr for stealth_mode setting
  static bool mmu_hw_enabled;     // MMU hardware can be Enabled/Disabled
                  // with the M709 S0 or M709 S1 commands
                  // and the last state is stored in the
                  // EEPROM
  static int mmu_hw_enabled_addr; // EEPROM addr for mmu_hw_enabled

  bool e_active();
#ifndef UNITTEST
private:
#endif
  /// Perform software self-reset of the MMU (sends an X0 command)
  void ResetX0();

  /// Perform software self-reset of the MMU + erase its EEPROM (sends X2a command)
  void ResetX42();

  /// Trigger reset pin of the MMU
  void TriggerResetPin();

  /// Perform power cycle of the MMU (cold boot)
  /// Please note this is a blocking operation (sleeps for some time inside while doing the power cycle)
  void PowerCycle();

  /// Stop the communication, but keep the MMU powered on (for scenarios with incorrect FW version)
  void StopKeepPowered();

  /// Along with the mmu_loop method, this loops until a response from the MMU is received and acts upon.
  /// In case of an error, it parks the print head and turns off nozzle heating
  /// @returns false if the command could not have been completed (MMU interrupted)
  [[nodiscard]] bool manage_response(const bool move_axes, const bool turn_off_nozzle);

  /// The inner private implementation of mmu_loop()
  /// which is NOT (!!!) recursion-guarded. Use caution - but we do need it during waiting for hotend resume to keep comms alive!
  /// @param reportErrors true if Errors should raise MMU Error screen, false otherwise
  void mmu_loop_inner(bool reportErrors);

  /// Performs one step of the protocol logic state machine
  /// and reports progress and errors if needed to attached ExtUIs.
  /// Updates the global state of MMU (Active/Connecting/Stopped) at runtime, see @ref State
  /// @param reportErrors true if Errors should raise MMU Error screen, false otherwise
  StepStatus LogicStep(bool reportErrors);

  void filament_ramming();
  void execute_extruder_sequence(const E_Step *sequence, uint8_t steps);
  void execute_load_to_nozzle_sequence();

  /// Reports an error into attached ExtUIs
  /// @param ec error code, see ErrorCode
  /// @param res reporter error source, is either Printer (0) or MMU (1)
  void ReportError(ErrorCode ec, ErrorSource res);

  /// Reports progress of operations into attached ExtUIs
  /// @param pc progress code, see ProgressCode
  void ReportProgress(ProgressCode pc);

  /// Responds to a change of MMU's progress
  /// - plans additional steps, e.g. starts the E-motor after fsensor trigger
  /// The function is quite complex, because it needs to handle asynchronnous
  /// progress and error reports coming from the MMU without an explicit command
  /// - typically after MMU's start or after some HW issue on the MMU.
  /// It must ensure, that calls to @ref ReportProgress and/or @ref ReportError are
  /// only executed after @ref BeginReport has been called first.
  void OnMMUProgressMsg(ProgressCode pc);
  /// Progress code changed - act accordingly
  void OnMMUProgressMsgChanged(ProgressCode pc);
  /// Repeated calls when progress code remains the same
  void OnMMUProgressMsgSame(ProgressCode pc);

  /// @brief Save hotend temperature and set flag to cooldown hotend after 60 minutes
  /// @param turn_off_nozzle if true, the hotend temperature will be set to 0degC after 60 minutes
  void SaveHotendTemp(bool turn_off_nozzle);

  /// Save print and park the print head
  void SaveAndPark(bool move_axes);

  /// Resume hotend temperature, if it was cooled. Safe to call if we aren't saved.
  void ResumeHotendTemp();

  /// Resume position, if the extruder was parked. Safe to all if state was not saved.
  void ResumeUnpark();

  /// Check for any button/user input coming from the printer's UI
  void CheckUserInput();

  /// @brief Check whether to trigger a FINDA runout. If triggered this function will call M600 AUTO
  /// if SpoolJoin is enabled, otherwise M600 is called without AUTO which will prompt the user
  /// for the next filament slot to use
  void CheckFINDARunout();

  /// Entry check of all external commands.
  /// It can wait until the MMU becomes ready.
  /// Optionally, it can also emit/display an error screen and the user can decide what to do next.
  /// @returns false if the MMU is not ready to perform the command (for whatever reason)
  bool WaitForMMUReady();

  /// After MMU completes a tool-change command
  /// the printer will push the filament by a constant distance. If the Fsensor untriggers
  /// at any moment the test fails. Else the test passes, and the E-motor retracts the
  /// filament back to its original position.
  /// @returns false if test fails, true otherwise
  bool VerifyFilamentEnteredPTFE();

  /// Common processing of pushing filament into the extruder - shared by tool_change, load_to_nozzle and probably others
  void ToolChangeCommon(uint8_t slot);
  bool ToolChangeCommonOnce(uint8_t slot);

  void HelpUnloadToFinda();
  void UnloadInner();
  void CutFilamentInner(uint8_t slot);

  void SetCurrentTool(uint8_t ex);

  ProtocolLogic logic;          ///< implementation of the protocol logic layer
  uint8_t extruder;             ///< currently active slot in the MMU ... somewhat... not sure where to get it from yet
  uint8_t tool_change_extruder; ///< only used for UI purposes

  xyz_pos_t resume_position;
  int16_t resume_hotend_temp;

  ProgressCode lastProgressCode = ProgressCode::OK;
  ErrorCode lastErrorCode = ErrorCode::MMU_NOT_RESPONDING;
  ErrorSource lastErrorSource = ErrorSource::ErrorSourceNone;
  Buttons lastButton = Buttons::NoButton;
  uint16_t lastReadRegisterValue = 0;
  Buttons printerButtonOperation = Buttons::NoButton;

  StepStatus logicStepLastStatus;

  enum xState state;

  uint8_t mmu_print_saved;
  bool loadFilamentStarted;
  bool unloadFilamentStarted;

  uint16_t toolchange_counter;
  uint16_t tmcFailures;
};

/// following Marlin's way of doing stuff - one and only instance of MMU implementation in the code base
/// + avoiding buggy singletons on the AVR platform
extern MMU2 mmu2;

} // namespace MMU2
#endif // HAS_PRUSA_MMU3