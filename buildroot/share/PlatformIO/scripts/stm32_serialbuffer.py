#
# stm32_serialbuffer.py
#
Import("env")

# Marlin has `RX_BUFFER_SIZE` and `TX_BUFFER_SIZE` to configure the
# buffer size for receiving and transmitting data respectively.
# Stm32duino uses another set of defines for the same purpose,
# so we get the values from the Marlin configuration and set
# them in `SERIAL_RX_BUFFER_SIZE` and `SERIAL_TX_BUFFER_SIZE`.
# It is not possible to change the values at runtime, they must
# be set with build flags.
#
# The script will set the value as the default one (64 bytes)
# or the user-configured one, whichever is higher.
#
# Marlin has 128 and 32 as default values for RX_BUFFER_SIZE and
# TX_BUFFER_SIZE respectively. We use the highest value (128/64).
#
# If the specific flags SERIAL_*X_BUFFER_SIZE, USART_*X_BUF_SIZE,
# or MF_*X_BUFFER_SIZE are set, then the script will use these
# values as minimum.
build_flags = env.ParseFlags(env.get('BUILD_FLAGS'))["CPPDEFINES"]
mf = env["MARLIN_FEATURES"]

def getBuildFlagValue(name):
	for flag in build_flags:
		if isinstance(flag, list) and flag[0] == name:
			return flag[1]

	return None

def getInternalSize(side):
	return getBuildFlagValue(f"MF_{side}_BUFFER_SIZE") or \
		getBuildFlagValue(f"SERIAL_{side}_BUFFER_SIZE") or \
		getBuildFlagValue(f"USART_{side}_BUF_SIZE")

def getBufferSize(side, default):
	internal = int(getInternalSize(side) or default)
	flag = side + "_BUFFER_SIZE"
	if flag in mf:
		return max(int(mf[flag]), internal)
	else:
		return internal

def tryAddFlag(name, value):
	if getBuildFlagValue(name) is None:
		env.Append(BUILD_FLAGS=[f"-D{name}={value}"])

rxBuf = getBufferSize("RX", 128)
txBuf = getBufferSize("TX", 64)

tryAddFlag("SERIAL_RX_BUFFER_SIZE", rxBuf)
tryAddFlag("SERIAL_TX_BUFFER_SIZE", txBuf)
tryAddFlag("USART_RX_BUF_SIZE", rxBuf)
tryAddFlag("USART_TX_BUF_SIZE", txBuf)
