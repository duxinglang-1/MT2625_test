#undef NVDM_MODEM_ITEM_DEF
#undef NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION
#undef NVDM_AP_ITEM_DEF
#undef NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION

#define NVDM_MODEM_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 0
#define NVDM_MODEM_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 0
#define NVDM_AP_ITEM_DEF(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 1
#define NVDM_AP_ITEM_DEF_NOGEN_DESCRIPTION(NV_NAME, STRUCTURE, GROUP_NAME, ITEM_NAME, AREA, DEFAULT, USE_VER, VER) NV_NAME, GROUP_NAME, ITEM_NAME, AREA, 1
#include "nvdm_modem_data_item_table.h"
#include "nvdm_data_item_table.h"