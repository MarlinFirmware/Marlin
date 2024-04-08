These test files are executed through the unit-tests built from the <root>/test folder.

By placing these outside of the main PlatformIO test folder, we are able to collect
all test files and compile them into multiple PlatformIO test binaries. This enables
tests to be executed against a variety of Marlin configurations.

To execute these tests, refer to the top-level Makefile.
