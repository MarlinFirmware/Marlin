# Resolve the environment names for a board from Marlin/src/pins/pins.h
# The env names live in the // env:... comment of the #include line that
# follows the MB(...) block for the board. Parse the exact board token from
# the MB() argument list (comma-separated) so a board whose name is a prefix
# of another (e.g. MKS_ROBIN_NANO vs MKS_ROBIN_NANO_V1_3_F4) can't collide.
#
# Usage:  awk -v MB=BOARD_NAME -f mfenvs.awk Marlin/src/pins/pins.h
#
/MB\(/ {
  s = $0
  sub(/^.*MB\(/, "", s)
  sub(/\)[[:space:]]*(\/\/.*)?$/, "", s)
  n = split(s, a, ",")
  hit = 0
  for (i = 1; i <= n; i++) {
    gsub(/^[[:space:]]+|[[:space:]]+$/, "", a[i])
    if (a[i] == MB) hit = 1
  }
  if (hit) {
    if (getline incl > 0) {
      while (match(incl, /env:[A-Za-z0-9_]+/)) {
        print substr(incl, RSTART + 4, RLENGTH - 4)
        incl = substr(incl, RSTART + RLENGTH)
      }
    }
  }
}
