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
#include "nvdm_modem_editor.h"
#include "nvdm_editor.h"
