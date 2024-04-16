#!/bin/bash

#/* Copyright Statement:
#*
#* (C) 2005-2017  MediaTek Inc. All rights reserved.
#*
#* This software/firmware and related documentation ("MediaTek Software") are
#* protected under relevant copyright laws. The information contained herein
#* is confidential and proprietary to MediaTek Inc. ("MediaTek") and/or its licensors.
#* Without the prior written permission of MediaTek and/or its licensors,
#* any reproduction, modification, use or disclosure of MediaTek Software,
#* and information contained herein, in whole or in part, shall be strictly prohibited.
#* You may only use, reproduce, modify, or distribute (as applicable) MediaTek Software
#* if you have agreed to and been bound by the applicable license agreement with
#* MediaTek ("License Agreement") and been granted explicit permission to do so within
#* the License Agreement ("Permitted User").  If you are not a Permitted User,
#* please cease any access or use of MediaTek Software immediately.
#* BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
#* THAT MEDIATEK SOFTWARE RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES
#* ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
#* WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
#* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
#* NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
#* SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
#* SUPPLIED WITH MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
#* THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
#* THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
#* CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
#* SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
#* STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
#* CUMULATIVE LIABILITY WITH RESPECT TO MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
#* AT MEDIATEK'S OPTION, TO REVISE OR REPLACE MEDIATEK SOFTWARE AT ISSUE,
#* OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
#* MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
#*/
#/*
# **************************************************************************/

gen_editor_header (){
  echo "typedef char kal_char;"                                         >$1 
  echo "typedef unsigned short kal_wchar;"                              >>$1
  echo "typedef unsigned char kal_uint8;"                               >>$1
  echo "typedef signed char kal_int8;"                                  >>$1
  echo "typedef unsigned short int kal_uint16;"                         >>$1
  echo "typedef signed short int kal_int16;"                            >>$1
  echo "typedef unsigned int kal_uint32;"                               >>$1
  echo "typedef signed int kal_int32;"                                  >>$1
  echo "typedef enum {"                                                 >>$1
  echo "    TST_NULL_COMMAND"                                           >>$1
  echo "} tst_command_type;"                                            >>$1
  echo "typedef kal_uint8 tst_null_command_struct;"                     >>$1
  echo "typedef struct"                                                 >>$1
  echo "{"                                                              >>$1
  echo "   tst_command_type  command_type;"                             >>$1
  echo "   kal_uint16        cmd_len;"                                  >>$1
  echo "} tst_command_header_struct;"                                   >>$1
  echo "typedef struct"                                                 >>$1
  echo "{"                                                              >>$1
  echo "    kal_uint32     dummy;"                                      >>$1
  echo "} tst_log_prim_header_struct;"                                  >>$1
  echo "typedef struct"                                                 >>$1
  echo "{"                                                              >>$1
  echo "    kal_uint32     dummy;"                                      >>$1
  echo "} tst_index_trace_header_struct;"                               >>$1
  echo "typedef struct"                                                 >>$1
  echo "{"                                                              >>$1
  echo "    kal_uint32     dummy;"                                      >>$1
  echo "} tst_prompt_trace_header_struct;"                              >>$1
  echo "typedef struct"                                                 >>$1
  echo "{"                                                              >>$1
  echo "    kal_uint8   index;"                                         >>$1
  echo "    kal_uint8   string[128];"                                   >>$1
  echo "} tst_inject_string_struct;"                                    >>$1
  echo "typedef kal_uint8 tst_null_command_struct ;"                    >>$1
  echo "typedef kal_uint8 tst_set_l1_trc_filter_struct;"                >>$1
  echo "typedef kal_uint8 tst_reboot_target_cmd_struct;"                >>$1
  echo "typedef kal_uint8 tst_query_memory_range_cmd_struct;"           >>$1
  echo "typedef kal_uint8 tst_query_soft_fc_char_cmd_struct;"           >>$1
  echo "typedef kal_uint8 tst_force_assert_cmd_struct;"                 >>$1
  echo "typedef kal_uint8 tst_flush_swdbg_cmd_struct;"                  >>$1
  echo "typedef kal_uint8 tst_st_stop_cmd_struct;"                      >>$1
  echo "typedef kal_uint8 tst_reboot_for_mmi_auto_test_cmd_struct;"     >>$1
  echo "typedef enum"                                                   >>$1
  echo "{"                                                              >>$1
  echo "   TRACE_FUNC,"                                                 >>$1
  echo "   TRACE_STATE,"                                                >>$1
  echo "   TRACE_INFO,"                                                 >>$1
  echo "   TRACE_WARNING,"                                              >>$1
  echo "   TRACE_ERROR,"                                                >>$1
  echo "   TRACE_GROUP_1,"                                              >>$1
  echo "   TRACE_GROUP_2,"                                              >>$1
  echo "   TRACE_GROUP_3,"                                              >>$1
  echo "   TRACE_GROUP_4,"                                              >>$1
  echo "   TRACE_GROUP_5,"                                              >>$1
  echo "   TRACE_GROUP_6,"                                              >>$1
  echo "   TRACE_GROUP_7,"                                              >>$1
  echo "   TRACE_GROUP_8,"                                              >>$1
  echo "   TRACE_GROUP_9,"                                              >>$1
  echo "   TRACE_GROUP_10,"                                             >>$1
  echo "   TRACE_PEER"                                                  >>$1
  echo "}trace_class_enum;"                                             >>$1
  echo "typedef enum {"                                                 >>$1
  echo "    SAP_ID_NULL"                                                >>$1
  echo "}sap_type;"                                                     >>$1
  echo "typedef enum {"                                                 >>$1
  echo "    MOD_ID_NULL"                                                >>$1
  echo "}module_type;"                                                  >>$1
  echo "typedef enum {"                                                 >>$1
  echo "    MSG_ID_NULL"                                                >>$1
  echo "}msg_type;"                                                     >>$1
}

write32tobin() {
   let byte1=$1%256
   let byte2=$1/256%256
   let byte3=$1/65536%256
   let byte4=$1/65536/256
   hex1=`printf %02x $byte1`
   hex2=`printf %02x $byte2`
   hex3=`printf %02x $byte3`
   hex4=`printf %02x $byte4`

   #echo -e "$str"
   if [[ $3 -eq "0" ]];then
       echo -n -e "\x$hex1\x$hex2\x$hex3\x$hex4" > $2
   else
       echo -n -e "\x$hex1\x$hex2\x$hex3\x$hex4" >>$2
   fi
}

write16tobin() {
   let byte1=$1%256
   let byte2=$1/256%256
   hex1=`printf %02x $byte1`
   hex2=`printf %02x $byte2`

   #echo -e "$str"
   if [[ $3 -eq "0" ]];then
       echo -n -e "\x$hex1\x$hex2" > $2
   else
       echo -n -e "\x$hex1\x$hex2" >>$2
   fi
}

AddZerotobin() {
   str=$1
   zero_str=""
   for((j=0;j<$2-${#str};j++));
   do
      zero_str=$zero_str"\x00"
   done
   echo -n -e "$zero_str" >> $3
}
################################################################################
#Begin here
cc=$1
cflags=$2
out_dir=$3
tool_dir=$4
internal_dir=$5
database_name=$6
db_ver=$7
customer_id=$8

#-----------------------------------------------------------------
script_ver=0x31303056
nv_autogen_dir=$out_dir/autogen/nvdm_database_autogen
nvram_editor_db=$nv_autogen_dir/nvdm_editor.db
nvram_editor_h=$nv_autogen_dir/nvdm_editor.h
raw_database=$nv_autogen_dir/nvdm_database.rawdata
nv_log=$nv_autogen_dir/gen_db.log
pc_cfg=$tool_dir/Pc_Cnf
tgt_cfg=$tool_dir/Tgt_Cnf_GCC
nvdm_enum_file=$nv_autogen_dir/nvdm_enum.txt
ori_editor_h=("nvdm_modem_editor.h" "nvdm_editor.h")
ori_mapping_h=("nvdm_modem_data_item_table.h" "nvdm_data_item_table.h")
editor_h=$nv_autogen_dir/nvdm_autogen_editor.h
mapping_h=$nv_autogen_dir/nvdm_mapping.h
mapping_bin=$nv_autogen_dir/nvdm_mapping.bin
mapping_txt=$nv_autogen_dir/nvdm_mapping.txt
database_name=$out_dir/$database_name
cc_flags="@$cflags -DGEN_NVDM_DATABASE_FOR_PC -DSYS_USE_ENUM_BOOLEAN"
compatibility_check=$nv_autogen_dir/nvdm_compatibility_check.c
compatibility_check_print_h=$nv_autogen_dir/nvdm_compatibility_check_print.h
compatibility_check_print_c=$nv_autogen_dir/nvdm_compatibility_check_print.c
compatibility_check_out=$nv_autogen_dir/nvdm_compatibility_check.out
compatibility_check_list=$nv_autogen_dir/nvdm_compatibility_check.log

platform=$(uname)
if [[ "$platform" =~ "MINGW" ]]; then
   cgen=$tool_dir/Cgen.exe
else
   cgen=$tool_dir/Cgen
fi
echo "Start gen nvdm database ...."
#step 0. check output directory
mkdir -p $nv_autogen_dir
rm $nv_autogen_dir/*

#step 1. generate nvdm_autogen_editor.h
gen_editor_header $editor_h
if [ -e $internal_dir ]; then
   for file in $(ls $internal_dir|grep '_editor.h')
   do
      ori_editor_h+=($file)
   done
   
   for file in $(ls $internal_dir|grep '_data_item_table.h')
   do
      ori_mapping_h+=($file)
   done
   cc_flags=$cc_flags" -I$internal_dir"
fi

for i in ${ori_editor_h[@]}
do
   echo "#include \"$i\"" >> $editor_h
done

#step 2. Pre-compile nvdm_autogen_editor.h and generate DB
temp_db=$nv_autogen_dir/nvdm_editor_temp.db
$cc $cc_flags -E -W -P -o $nvram_editor_db $editor_h> $nv_log
if [ "$?" -ne "0" ]; then
   echo "[gen db fail]:Pre-compile nvdm_autogen_editor.h fail"
   exit 1
fi

$cc $cc_flags -E -o $temp_db $editor_h>> $nv_log
$cgen -c $nvram_editor_db $tgt_cfg $pc_cfg $raw_database $nvdm_enum_file swVer $db_ver >> $nv_log
if [ "$?" -ne "0" ]; then
   echo "[gen db fail]:gen db from nvdm_editor.db fail"
   exit 1
fi
#step 3. Gen nvdm_mapping.h
echo "#undef NVDM_MODEM_ITEM_DEF"                                                                                                                            >$mapping_h
echo "#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION"                                                                                                          >>$mapping_h
echo "#undef NVDM_AP_ITEM_DEF"                                                                                                                               >>$mapping_h
echo "#undef NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION"                                                                                                             >>$mapping_h
echo ""                                                                                                                                                      >>$mapping_h
echo "#define NVDM_MODEM_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 0"                         >>$mapping_h
echo "#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 0"       >>$mapping_h
echo "#define NVDM_AP_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 1"                            >>$mapping_h
echo "#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 1"          >>$mapping_h
for i in ${ori_mapping_h[@]}
do
   echo "#include \"$i\"" >>$mapping_h
done

$cc $cc_flags -E -W -P -o $mapping_txt $mapping_h >> $nv_log
if [ "$?" -ne "0" ]; then
   echo "[gen db fail]:Pre-compile nvdm_mapping.h fail"
   exit 1
fi


if [ -e $internal_dir ]; then
	   
	#Generate nvram compatibility check list
	cp $nvram_editor_db $nvram_editor_h
	sed -i "s/BEGIN_NVRAM_DATA/\/\*/g" $nvram_editor_h
	sed -i "s/END_NVRAM_DATA/\*\//g" $nvram_editor_h
	sed -i "s/typedef unsigned int size_t;//g" $nvram_editor_h
	sed -i "s/typedef long int __int32_t;//g" $nvram_editor_h
	sed -i "s/typedef long unsigned int __uint32_t;//g" $nvram_editor_h
	sed -i "s/typedef long long int __int64_t;//g" $nvram_editor_h
	sed -i "s/typedef long long unsigned int __uint64_t;//g" $nvram_editor_h
	sed -i "s/typedef int __intptr_t;//g" $nvram_editor_h

	echo "#undef NVDM_MODEM_ITEM_DEF"                                                                                                                            >$compatibility_check_print_h
	echo "#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION"                                                                                                          >>$compatibility_check_print_h
	echo "#undef NVDM_AP_ITEM_DEF"                                                                                                                               >>$compatibility_check_print_h
	echo "#undef NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION"                                                                                                             >>$compatibility_check_print_h
	echo ""                                                                                                                                                      >>$compatibility_check_print_h
	echo "#define NVDM_MODEM_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) printf(\"%s, %s, %s, %d\", #NV_NAME, GROUP_NAME, #STRUCTURE, sizeof(STRUCTURE));printf(\"\\n\");"                         >>$compatibility_check_print_h
	echo "#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) printf(\"%s, %s, %s, %d\", #NV_NAME, GROUP_NAME, #STRUCTURE, sizeof(STRUCTURE));printf(\"\\n\");"       >>$compatibility_check_print_h
	echo "#define NVDM_AP_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) "                            >>$compatibility_check_print_h
	echo "#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) "          >>$compatibility_check_print_h
	for i in ${ori_mapping_h[@]}
	do
	   echo "#include \"$i\"" >>$compatibility_check_print_h
	done

	$cc $cc_flags -E -W -P -o $compatibility_check_print_c $compatibility_check_print_h >> $nv_log
	if [ "$?" -ne "0" ]; then
	   echo "[gen db fail]:Pre-compile nvdm_compatibility_check_print.c fail"
	   exit 1
	fi

	echo "#include <stdio.h>" >>$compatibility_check
	echo "#include \"nvdm_editor.h\"" >>$compatibility_check
	echo "    int main()">>$compatibility_check
	echo "    {">>$compatibility_check
	cat $compatibility_check_print_c >> $compatibility_check
	echo "    }">>$compatibility_check

	gcc $compatibility_check -o $compatibility_check_out 

	$compatibility_check_out>> $compatibility_check_list

	echo "Generate compatibility check Done"
fi

#step 4. Gen mapping.bin
item_num=0
while read myline
do
   info=(${myline//,/})
   if [ ${#info[@]} -eq 5 ];then
      str=${info[0]}
      echo -n -e "$str" >>$mapping_bin
	  AddZerotobin $str 128 $mapping_bin
	  	  
	  str=`echo ${info[1]}|sed s/\"//g`
      echo -n -e "$str" >>$mapping_bin
	  AddZerotobin $str 64 $mapping_bin
	  	  
	  str=`echo ${info[2]}|sed s/\"//g`
      echo -n -e "$str" >>$mapping_bin
	  AddZerotobin $str 64 $mapping_bin
	  
	  str=`echo ${info[3]}|sed -e s/NVDM_MODEM_AREA_NONE/0/g -e s/NVDM_MODEM_AREA_NORMAL/1/g -e s/NVDM_MODEM_AREA_PROTECTED/2/g -e s/NVDM_MODEM_AREA_BACKUP/4/g`
      echo -n -e "\x$str" >>$mapping_bin
	  echo -n -e "\x00" >>$mapping_bin
      	  
	  str=${info[4]}
	  echo -n -e "\x$str" >>$mapping_bin
	  echo -n -e "\x00" >>$mapping_bin
	  
	  ((item_num=item_num+1))
   fi
done <$mapping_txt


#step 5, gen final db file
mapping_size=`ls -l $mapping_bin | awk '{ print $5 }'`
((mapping_size=mapping_size+4))
db_size=`ls -l $raw_database | awk '{ print $5 }'`


write32tobin 0x31303056 $database_name 0
echo -n -e "$customer_id" >>$database_name
AddZerotobin $customer_id 64 $database_name
#mapping_bin offset and size
write32tobin 84 $database_name 1
write32tobin $mapping_size $database_name 1
#DB bin offset and size
let db_off=$mapping_size+84
write32tobin $db_off $database_name 1
write32tobin $db_size $database_name 1
#Mapping context
write32tobin $item_num $database_name 1
cat $mapping_bin >>$database_name
#DB  context
cat $raw_database >>$database_name

echo "generate nvdm database done!"
