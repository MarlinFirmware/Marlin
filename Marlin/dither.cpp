#include "dither.h"
#include "macros.h"
#include "math.h"
#include "stepper.h"
#include "temperature.h"
#include "Configuration_adv.h"

uint16_t Dithering::Amplitude = 16; // how many steps for dither
uint16_t Dithering::TimeMS = 100;
float Dithering::MinLayerInterval = 0.1;
float Dithering::PrevZ = 0.0;

uint32_t Dithering::DitherSubTime = 25;
uint8_t Dithering::Ditherssteps[4] = {16, 8, 4, 2};

void Dithering::Handle(float CurrZ)
{

  if (Dithering::Amplitude == 0 || Dithering::TimeMS == 0 || (fabs(CurrZ - Dithering::PrevZ) < Dithering::MinLayerInterval))
  {
    SERIAL_ECHOLNPGM("Delta Z to small, skipping dithering");
    return;
  }
  else
  {

    Dithering::PrevZ = CurrZ;

    while (!Temperature::babystepsTodo[Z_AXIS] == 0) // if there are some existing babysteps to do just wait it out
      ;

    uint32_t DitherStartTime = millis();

    int j = 0;

    SERIAL_ECHOLNPGM("Start Dithering..");

    while (millis() < (DitherStartTime + (Dithering::DitherSubTime * (j + 1))))
    {

      Temperature::babystepsTodo[Z_AXIS] = Temperature::babystepsTodo[Z_AXIS] + Dithering::Ditherssteps[j];
      while (!Temperature::babystepsTodo[Z_AXIS] == 0)
        ;

      Temperature::babystepsTodo[Z_AXIS] = Temperature::babystepsTodo[Z_AXIS] - 2 * Dithering::Ditherssteps[j];
      while (!Temperature::babystepsTodo[Z_AXIS] == 0)
        ;

      Temperature::babystepsTodo[Z_AXIS] = Temperature::babystepsTodo[Z_AXIS] + Dithering::Ditherssteps[j];
      while (!Temperature::babystepsTodo[Z_AXIS] == 0)
        ;

      j++;

      SERIAL_ECHOLNPGM("Dithering Done.");

      if (j > 3)
      { //for safety
        return;
      }
    }
  }
}

void Dithering::CalculateParameters()
{
  Dithering::DitherSubTime = Dithering::TimeMS / 4;

  Dithering::Ditherssteps[0] = Dithering::Amplitude;
  Dithering::Ditherssteps[1] = Dithering::Amplitude / 2;
  Dithering::Ditherssteps[2] = Dithering::Amplitude / 4;
  Dithering::Ditherssteps[3] = Dithering::Amplitude / 8;


}