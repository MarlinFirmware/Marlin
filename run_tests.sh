#!/bin/bash -e

if [ -z $AVR_TOOLS_PATH ]; then
    echo "AVR_TOOLS_PATH not set.  This could cause problems"
fi

if [ -z $ARDUINO_INSTALL_DIR ]; then
    echo "ARDUINO_INSTALL_DIR not set.  This could cause problems"
fi

function print_vars
{
    echo "Environment variables:"
    echo "   " DELTA=$DELTA
    echo "   " MOTHERBOARD=$MOTHERBOARD
    echo "   " ENABLE_AUTO_BED_LEVELING=$ENABLE_AUTO_BED_LEVELING
    echo "   " AUTO_BED_LEVELING_ANY_POINTS=$AUTO_BED_LEVELING_ANY_POINTS
    echo "   " Z_SAFE_HOMING_FIXED_POINT=$Z_SAFE_HOMING_FIXED_POINT
}

function reset_vars
{
    export DELTA=
    export MOTHERBOARD=
    export ENABLE_AUTO_BED_LEVELING=
    export AUTO_BED_LEVELING_ANY_POINTS=
    export Z_SAFE_HOMING_FIXED_POINT=
}

function reset_configs
{
    git checkout HEAD -- Marlin/Configuration.h
    git checkout HEAD -- Marlin/Configuration_adv.h
}

function run
{
    print_vars

    if [ -n "$DELTA" ]; then
        echo "DELTA" && cp Marlin/example_configurations/delta/Configuration* Marlin/
    fi

    CFG=Marlin/Configuration.h
    cp $CFG $CFG.orig
    cat $CFG.orig | ./set-defines.sh MOTHERBOARD ENABLE_AUTO_BED_LEVELING AUTO_BED_LEVELING_ANY_POINTS Z_SAFE_HOMING_FIXED_POINT > $CFG
    echo "Current defines:"
    grep -e MOTHERBOARD -e ENABLE_AUTO_BED_LEVELING -e AUTO_BED_LEVELING_ANY_POINTS -e Z_SAFE_HOMING_FIXED_POINT -e DELTA $CFG
    sleep 2
    (cd Marlin && make clean && make HARDWARE_MOTHERBOARD=$MOTHERBOARD AVR_TOOLS_PATH=$AVR_TOOLS_PATH ARDUINO_INSTALL_DIR=$ARDUINO_INSTALL_DIR)

    reset_vars
    reset_configs
}


reset_vars
reset_configs

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 AUTO_BED_LEVELING_ANY_POINTS=1 Z_SAFE_HOMING_FIXED_POINT=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 AUTO_BED_LEVELING_ANY_POINTS=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 Z_SAFE_HOMING_FIXED_POINT=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1
run

MOTHERBOARD=81
run

MOTHERBOARD=3
run

MOTHERBOARD=3 DELTA=1
run

