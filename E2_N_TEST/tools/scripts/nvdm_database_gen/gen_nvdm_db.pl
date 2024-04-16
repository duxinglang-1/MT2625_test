#!/usr/bin/perl

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

my $cc = shift @ARGV;
my $cflags = shift @ARGV;
my $out_dir = shift @ARGV;
my $tool_dir = shift @ARGV;
my $internal_dir = shift @ARGV;
my $database_name = shift @ARGV;
my $db_ver = shift @ARGV;
my $customer_id =shift @ARGV;

#-----------------------------------------------
my $script_ver = 0x31303056;
my $auto_gen_dir = $out_dir."/autogen";
my $nv_autogen_dir = $auto_gen_dir."/nvdm_database_autogen";
my $nvram_editor_db = $nv_autogen_dir."/nvdm_editor.db";
my $raw_database = $nv_autogen_dir."/nvdm_database.rawdata";
my $nv_log = $nv_autogen_dir."/gen_db.log";
my $cgen = $tool_dir."/Cgen";
my $pc_cfg = $tool_dir."/Pc_Cnf";
my $tgt_cfg = $tool_dir."/Tgt_Cnf_GCC";
my $nvdm_enum_file = $nv_autogen_dir."/nvdm_enum.txt";
my @ori_editor_h = ("nvdm_modem_editor.h", "nvdm_editor.h"); #"nvdm_editor.h"
my @ori_mapping_h = ("nvdm_modem_data_item_table.h", "nvdm_data_item_table.h"); #"nvdm_data_item_table.h"
my $editor_h = $nv_autogen_dir."/nvdm_autogen_editor.h";
my $mapping_h = $nv_autogen_dir."/nvdm_mapping.h";
my $mapping_bin = $nv_autogen_dir."/nvdm_mapping.bin";
my $mapping_txt = $nv_autogen_dir."/nvdm_mapping.txt";
my $database_name = $out_dir."/".$database_name;
my $cc_flags = "\@$cflags -DGEN_NVDM_DATABASE_FOR_PC -DSYS_USE_ENUM_BOOLEAN";

printf("Start gen nvdm database ....\n\r");
#step 0. check output directory
die "$out_dir not exists" if (!(-e $out_dir));
if (!(-e $auto_gen_dir)){
    mkdir "$auto_gen_dir", 0755 or die "Cannot make $auto_gen_dir\n\r";
   }

if (!(-e $nv_autogen_dir)){
    mkdir "$nv_autogen_dir", 0755 or die "Cannot make $nv_autogen_dir\n\r";
  }
    
#step 1. generate nvdm_autogen_editor.h
my $editor_text = &get_head_text();
$editor_text = $editor_text."\n\n\n";
foreach my $h (@ori_editor_h){
    $editor_text = $editor_text."#include \"".$h."\"\n";
}

if(-e $internal_dir){
    my @internal_h = <$internal_dir/*_editor.h>;
    my @internal_mapping_h = <$internal_dir/*_data_item_table.h>;
    foreach my $h (@internal_h){
        if($h =~ /([\w]*editor\.h)/){
           $editor_text = $editor_text."#include \"$1\"\n\r";
        }
    }

    foreach my $h (@internal_mapping_h){
        if($h =~ /([\w]*data_item_table\.h)/){
           push @ori_mapping_h, $1;
        }
    }
    $cc_flags = $cc_flags." -I$internal_dir";
}
open EDITOR_H, ">$editor_h" or die "Can't open $editor_h\n";
printf EDITOR_H $editor_text;
close EDITOR_H;

my $temp_db = $nvram_editor_db."\.temp";
#step 2. Pre-compile nvdm_autogen_editor.h
$result = system("$cc $cc_flags -E -W -P -o $nvram_editor_db $editor_h> $nv_log\n");
$result = system("$cc $cc_flags -E -o $temp_db $editor_h> $nv_log\n");
die "please check $nv_log for detail\n" if ($result != 0);

#step 3. Gen database file
$result = system("$cgen -c $nvram_editor_db $tgt_cfg $pc_cfg $raw_database $nvdm_enum_file swVer $db_ver >> $nv_log");
die "please check $nv_log for detail\n" if ($result != 0);

#step 4. generate nvdm_mapping.h
&gen_mapping_h(\@ori_mapping_h, $mapping_h);

#step 5. Pre-compile nvdm_mapping.h1
$result = system("$cc $cc_flags -E -W -P -o $mapping_txt $mapping_h > $nv_log");

#step 6. create mapping bin
&gen_mapping_bin($mapping_txt, $mapping_bin);

#step 7. pack database and mapping_bin, gen final database
&pack_db_and_mapping($raw_database, $mapping_bin, $script_ver, $customer_id, $database_name);
printf("generate nvdm database done\n\r!");

sub gen_mapping_h(){
    my ($mapping_list, $mapping_h) = @_;

    
    open MAPPING_H, ">$mapping_h" or die "Can't open $mapping_h\n";

    printf MAPPING_H "#undef NVDM_MODEM_ITEM_DEF\n\r";
    printf MAPPING_H "#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION\n\r";
    printf MAPPING_H "#undef NVDM_AP_ITEM_DEF\n\r";
    printf MAPPING_H "#undef NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION\n\r";
    printf MAPPING_H "\n\r\n\r";
    printf MAPPING_H "#define NVDM_MODEM_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 0\n\r";
    printf MAPPING_H "#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 0\n\r";
    printf MAPPING_H "#define NVDM_AP_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 1\n\r";
    printf MAPPING_H "#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VERNO) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 1\n\r";
    
    foreach my $file (@$mapping_list) {
       printf MAPPING_H "#include \"$file\"\n\r";
    }
    close MAPPING_H;
}

sub gen_mapping_bin(){
    my ($src, $dst) = @_;
    my $nvdm_item_num = 0;
    open SRC, "<$src" or die "Can't open $src\n";
    open DST, ">$dst" or die "Can't open $dst\n";
    binmode(DST);
    
    sysseek(DST, 4, 0);
    while(<SRC>){
       my $line = $_;
       #parse AREA
       $line =~ s/NVDM_MODEM_AREA_NONE/0/;
       $line =~ s/NVDM_MODEM_AREA_NORMAL/1/;
       $line =~ s/NVDM_MODEM_AREA_PROTECTED/2/;
       $line =~ s/NVDM_MODEM_AREA_BACKUP/4/;
       
       if($line =~ m/^\s*(\w*)\s*\,\s*\"*(\w*)\"*\s*\,\s*\"*(\w*)\"*\s*\,\s*(\d+)\s*\,\s*(\d+)/){
           $buf = pack("a128", $1);
           syswrite(DST, $buf, 128);
           $buf = pack("a64", $2);
           syswrite(DST, $buf, 64);
           $buf = pack("a64", $3);
           syswrite(DST, $buf, 64);
           $buf = pack("v", $4);
           syswrite(DST, $buf, 2);
           $buf = pack("v", $5);
           syswrite(DST, $buf, 2);
           $nvdm_item_num = $nvdm_item_num + 1;
       }
       
    }
    sysseek(DST, 0, 0);
    $buf = pack("V", $nvdm_item_num);
    syswrite(DST, $buf, 4);
    
    close(SRC);
    close(DST);
}

sub pack_db_and_mapping(){
    my ($raw_database, $mapping_bin, $script_ver, $customer_id, $out) = @_;
    open DB, "<$raw_database" or die "can't open $raw_database";
    open MAPPING, "<$mapping_bin" or die "can't open $mapping_bin";
    open FINAL, ">$out" or die "can't open $out";

    binmode(MAPPING);
    binmode(DB);
    binmode(FINAL);

    my $mapping_size = -s $mapping_bin;
    my $db_size = -s $raw_database;
    my $buf;
    $buf = pack("V", $script_ver);
    syswrite(FINAL, $buf, 4);
    $buf = pack("a64", $customer_id);
    syswrite(FINAL, $buf, 64);
    $buf = pack("V", 84); #mapping data is at offset (4 + 64 + 16)
    syswrite(FINAL, $buf, 4);
    $buf = pack("V", $mapping_size);
    syswrite(FINAL, $buf, 4);
    $buf = pack("V", ($mapping_size + 84));
    syswrite(FINAL, $buf, 4);
    $buf = pack("V", $db_size);
    syswrite(FINAL, $buf, 4);
    sysread(MAPPING, $buf, $mapping_size);
    syswrite(FINAL, $buf, $mapping_size);
    sysread(DB, $buf, $db_size);
    syswrite(FINAL, $buf, $db_size);

    close(MAPPING);
    close(DB);
    close(FINAL);
}
sub get_head_text(){
    my $text = <<"__TEMPLATE";

typedef char kal_char;

typedef unsigned short kal_wchar;


typedef unsigned char kal_uint8;

typedef signed char kal_int8;

typedef unsigned short int kal_uint16;

typedef signed short int kal_int16;

typedef unsigned int kal_uint32;

typedef signed int kal_int32;

typedef enum {
    TST_NULL_COMMAND
} tst_command_type;

typedef kal_uint8 tst_null_command_struct;

typedef struct
{
   tst_command_type  command_type;
   kal_uint16        cmd_len;
} tst_command_header_struct;

typedef struct
{
    kal_uint32     dummy;
} tst_log_prim_header_struct;

typedef struct
{
    kal_uint32     dummy;
} tst_index_trace_header_struct;

typedef struct
{
    kal_uint32     dummy;
} tst_prompt_trace_header_struct;

typedef struct
{
    kal_uint8   index;
    kal_uint8   string[128];
} tst_inject_string_struct;






typedef kal_uint8 tst_null_command_struct ;

typedef kal_uint8 tst_set_l1_trc_filter_struct;

typedef kal_uint8 tst_reboot_target_cmd_struct;

typedef kal_uint8 tst_query_memory_range_cmd_struct;

typedef kal_uint8 tst_query_soft_fc_char_cmd_struct;

typedef kal_uint8 tst_force_assert_cmd_struct;

typedef kal_uint8 tst_flush_swdbg_cmd_struct;

typedef kal_uint8 tst_st_stop_cmd_struct;

typedef kal_uint8 tst_reboot_for_mmi_auto_test_cmd_struct;

typedef enum
{
   TRACE_FUNC,
   TRACE_STATE,
   TRACE_INFO,
   TRACE_WARNING,
   TRACE_ERROR,
   TRACE_GROUP_1,
   TRACE_GROUP_2,
   TRACE_GROUP_3,
   TRACE_GROUP_4,
   TRACE_GROUP_5,
   TRACE_GROUP_6,
   TRACE_GROUP_7,
   TRACE_GROUP_8,
   TRACE_GROUP_9,
   TRACE_GROUP_10,
   TRACE_PEER
}trace_class_enum;

typedef enum {
    SAP_ID_NULL

}sap_type;

typedef enum {
    MOD_ID_NULL

}module_type;

typedef enum {
    MSG_ID_NULL

}msg_type;

__TEMPLATE
  return $text;
}
