#!/bin/bash -x

#########################################
# Prepare SDK and show selected choices #
#########################################
echo "Selected languages: $LANGUAGE_CHOICE"
echo "Selected hardware: $HARDWARE"

#mkdir -p $WWW_DIR
cd $WORKSPACE
#rm -rf $ARDUINO
#mkdir $ARDUINO

cd $FW_DIR
git log -1 --pretty="%H" > $WWW_DIR/commit.built
cd $WORKSPACE

#cd $ARDUINO
#git init
#git fetch ssh://git@stash.bq.com:7999/tresd/arduino-sdk.git 1.6.7
#git reset --hard FETCH_HEAD


LANG_FILE="$CONFIG_DIR_DEST/Language_texts.h"
CONF_FILE="$CONFIG_DIR_DEST/Configuration.h"
IFS=","

###########################################
# Iterate over selected hardware variants #
###########################################
for VARIANT in $HARDWARE; do

   CONF_FILE="$CONFIG_DIR/$VARIANT/Configuration.h"

   if [ "$VARIANT" != "hephestos_2" ] && [ "$VARIANT" != "witbox_2" ]; then

         for LANG_VALUE in $LANGUAGE_CHOICE; do
            LANG_NUMBER=$(case $LANG_VALUE in
	     "EN") echo "1" ;;
 	     "ES") echo "2" ;;
 	     "DE") echo "3" ;;
	     "FR") echo "4" ;;
	     "PT") echo "5" ;;
	     "IT") echo "6" ;;
 	     "FI") echo "7" ;;
 	     "NL") echo "8" ;;
   	     "PL") echo "9" ;;
 	     "RU") echo "10" ;;
 	     "AN") echo "11" ;;
 	     *) echo "1" ;;
	    esac)

            sed -i "s/define BUILD_VER \"\"/define BUILD_VER \"#$BUILD_NUMBER\"/g" $CONF_FILE
            sed -i "s/define FIRMWARE_LANGUAGE_M115 [^ ]*/define FIRMWARE_LANGUAGE_M115 \"$LANG_VALUE\"/g" $CONF_FILE

            cd $CONFIG_DIR_DEST
	    rm -rf bin/Marlin.hex
	    make $VARIANT $LANG_VALUE
            make
 	    cp bin/Marlin.hex $WWW_DIR/"Marlin_"$VARIANT"-"$BUILD_NUMBER"_"$LANG_VALUE.hex
 	    cd ..
         done
   else
        sed -i "s/define BUILD_VER \"\"/define BUILD_VER \"#$BUILD_NUMBER\"/g" $CONF_FILE
        cd $CONFIG_DIR_DEST
	rm -rf bin/Marlin.hex
	make $VARIANT
        make
 	cp bin/Marlin.hex $WWW_DIR/"Marlin_"$VARIANT"-"$BUILD_NUMBER.hex
 	cd ..
   fi
done
