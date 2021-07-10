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
# TX_BUFFER_SIZE respectively. We use the highest value.
mf = env["MARLIN_FEATURES"]
rxBuf = str(max(128, int(mf["RX_BUFFER_SIZE"]) if "RX_BUFFER_SIZE" in mf else 0))
txBuf = str(max(64, int(mf["TX_BUFFER_SIZE"]) if "TX_BUFFER_SIZE" in mf else 0))

build_flags = env.get('BUILD_FLAGS')
build_flags.append("-DSERIAL_RX_BUFFER_SIZE=" + rxBuf)
build_flags.append("-DSERIAL_TX_BUFFER_SIZE=" + txBuf)
build_flags.append("-DUSART_RX_BUF_SIZE=" + rxBuf)
build_flags.append("-DUSART_TX_BUF_SIZE=" + txBuf)
env.Replace(BUILD_FLAGS=build_flags)
