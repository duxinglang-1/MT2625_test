ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

#ONENET support
MTK_ONENET_SUPPORT = n

#LWM2M support
MTK_LWM2M_SUPPORT = y
MTK_LWM2M_AT_CMD_SUPPORT = y
