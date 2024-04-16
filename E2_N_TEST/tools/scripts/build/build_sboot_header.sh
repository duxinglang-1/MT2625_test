#!/bin/bash

set -o pipefail
###############################################################################
#Variables
SOURCE_DIR=$1
OUTPUT=$2
BOARD_CONFIG=$3
BIN_FILENAME=$4

SCRIPT_DIR=$SOURCE_DIR/tools/scripts/secure_boot/

###############################################################################
#Functions
change_ini_bin_size () {
    bin_size=`wc --bytes $2 | cut -d ' ' -f1`
    bin_end=`awk '{match($0, /ac_region_'"$3"'_begin\s*=\s*(0x[a-zA-Z0-9]+)/, arr); if(arr[1] != "") printf "0x%x\n", strtonum(arr[1])+"'"$bin_size"'"-1}' $1`
    sed -i "s/ac_region_${3}_end\s*=\s*0x[a-zA-Z0-9]\+/ac_region_${3}_end=$bin_end/g" $1
}

###############################################################################
#Begin here

echo $0
echo `basename $0`"...."
echo 'Board_config= '${BOARD_CONFIG}


if [[ `echo "$BIN_FILENAME" | grep -c  "bootloader"` -gt 0 ]]; then
    cp -f $SCRIPT_DIR/${BOARD_CONFIG}_input_settings_bl.ini $OUTPUT/${BOARD_CONFIG}_input_settings_bl.ini
    if [ -f $OUTPUT/$BIN_FILENAME ] && [ -s $OUTPUT/$BIN_FILENAME ]; then
        change_ini_bin_size $OUTPUT/${BOARD_CONFIG}_input_settings_bl.ini $OUTPUT/$BIN_FILENAME 0
    fi
    $SCRIPT_DIR/pbp $OUTPUT/$BIN_FILENAME $OUTPUT/${BOARD_CONFIG}_input_settings_bl.ini $SCRIPT_DIR/private.pem $OUTPUT/${BOARD_CONFIG}_bl_hdr.bin 
    #rm -f $OUTPUT/${BOARD_CONFIG}_input_settings_bl.ini
else
    $SCRIPT_DIR/pbp $OUTPUT/$BIN_FILENAME $SCRIPT_DIR/${BOARD_CONFIG}_input_settings_cm4.ini $SCRIPT_DIR/private.pem $OUTPUT/${BOARD_CONFIG}_cm4_hdr.bin 
fi



exit 0
