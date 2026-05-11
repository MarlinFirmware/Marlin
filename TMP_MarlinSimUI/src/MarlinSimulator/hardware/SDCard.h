#pragma once

#include "../user_interface.h"

#include "SPISlavePeripheral.h"

/**
  * Instructions for create a FAT image:
  * 1) Install mtools
  * 2) Create the image file:
  *    $ mformat -v "EMBEDDED FS" -t 1 -h 1 -s 10000 -S 2 -C -i fs.img -c 1 -r 1 -L 1
  *    -s NUM is the number of sectors
  * 3) Copy files to the image:
  *    $ mcopy -i fs.img CFFFP_flow_calibrator.gcode ::/
  * 4) Set the path for SD_SIMULATOR_FAT_IMAGE
  */
 //#define SD_SIMULATOR_FAT_IMAGE "/full/path/to/fs.img"
#ifndef SD_SIMULATOR_FAT_IMAGE
  #define SD_SIMULATOR_FAT_IMAGE "fs.img"
#endif

class SDCard: public SPISlavePeripheral {
public:
  SDCard(SpiBus& spi_bus, pin_type cs, pin_type sd_detect = -1, bool sd_detect_state = true) : SPISlavePeripheral(spi_bus, cs), sd_detect(sd_detect), sd_detect_state(sd_detect_state), image_filename(SD_SIMULATOR_FAT_IMAGE) {
    if (Gpio::valid_pin(sd_detect)) {
      Gpio::attach(sd_detect, [this](GpioEvent& event){ this->interrupt(event); });
    }
    sd_present = image_exists();
    Gpio::set_pin_value(sd_detect, sd_present);
  }
  virtual ~SDCard() {};

  void update() {}
  const std::string file_dialog_key = "ChooseSDFileDlgKey";
  void ui_widget() {
    if (ImGui::Button("Select Image (FAT32)")) {
      IGFD::FileDialogConfig config { "." };
      config.flags |= ImGuiFileDialogFlags_Modal;
      ImGuiFileDialog::Instance()->OpenDialog(file_dialog_key, "Choose File", "FAT32 Disk Image(*.img){.img},.*", config);
    }
    if (ImGuiFileDialog::Instance()->Display(file_dialog_key, ImGuiWindowFlags_NoDocking))  {
      if (ImGuiFileDialog::Instance()->IsOk()) {
        image_filename = ImGuiFileDialog::Instance()->GetFilePathName();
        sd_present = image_exists();
        Gpio::set_pin_value(sd_detect, sd_present);
      }
      ImGuiFileDialog::Instance()->Close();
    }

    ImGui::Text("FileSystem image \"%s\" selected", image_filename.c_str());
    if (Gpio::valid_pin(sd_detect)) {
      ImGui::Checkbox("SD Card Present ", (bool*)&sd_present);
    }

    if (!sd_present) {
      if (ImGui::Button("Generate Empty Image")) {
        if (image_exists()) {
          printf("File exists unable to create new image!\n");
          //confirm overwrite?
        } else {
          generate_empty_image(image_filename);
          sd_present = image_exists();
          Gpio::set_pin_value(sd_detect, sd_present);
        }
      }
    }
  }

  void onByteReceived(uint8_t _byte) override;
  void onRequestedDataReceived(uint8_t token, uint8_t* _data, size_t count) override;

  void interrupt(GpioEvent &ev) {
    if (ev.pin_id == sd_detect && ev.event == GpioEvent::GET_VALUE) {
      Gpio::set_pin_value(sd_detect, sd_present ? sd_detect_state : !sd_detect_state);
    }
  }

  bool image_exists() {
    auto image_fp = fopen(image_filename.c_str(), "rb+");
    if (image_fp == nullptr) {
      return false;
    }
    fclose(image_fp);
    return true;
  }
  void generate_empty_image(std::string filename);

  int32_t currentArg = 0;
  uint8_t buf[1024];
  FILE *fp = nullptr;
  bool sd_present = false;
  pin_type sd_detect;
  bool sd_detect_state = true;
  std::string image_filename;
};
