#include "spi.h"

SpiBus SpiBus0;
SpiBus SpiBus1;
SpiBus SpiBus2;
SpiBus SpiBus3;

template<> SpiBus& spi_bus_by_pins<50, 52, 51>() { return SpiBus0; }
template<> SpiBus& spi_bus_by_pins<100, 101, 102>() { return SpiBus1; }
template<> SpiBus& spi_bus_by_pins<110, 111, 112>() { return SpiBus2; }
template<> SpiBus& spi_bus_by_pins<120, 121, 122>() { return SpiBus3; }
