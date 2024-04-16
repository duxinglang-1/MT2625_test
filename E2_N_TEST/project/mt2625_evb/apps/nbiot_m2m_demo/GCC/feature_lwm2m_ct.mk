ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

MTK_LWM2M_SUPPORT = y
MTK_LWM2M_AT_CMD_SUPPORT = n
MTK_LWM2M_CT_SUPPORT = ctiot
MTK_LWM2M_WITH_DTLS_SUPPORT = mbed
MTK_RIL_UT_TEST_CASE_ENABLE = n
