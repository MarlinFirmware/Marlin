This is a list of files that we have modified in the core Marlin files for our distribution.

This is for our reference to make sure to retain changes made to the core files when merging in the latest Marlin firmware baseline.

This does not include machine configs.

----------


- **Marlin**
	- **src**
		- **core**
			- `language.h`
		- **lcd**
			- **dwin**
				- **e3v2**
					- `dwin.cpp`
			- **menu**
				- `menu_advanced.cpp`
				- `menu_configuration.cpp`
			- **language**
				- `language_en.h`
		- **pins**
			- **lpc1769**
				- `pins_TH3D_EZBOARD.h`
			- **stm32f1**
				- `pins_MKS_ROBIN_MINI.h`
	- `Version.h`