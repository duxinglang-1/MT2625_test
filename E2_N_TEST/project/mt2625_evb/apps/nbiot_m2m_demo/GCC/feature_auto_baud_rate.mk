ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

#Auto baud rate
MTK_AUTO_BAUD_RATE_ENABLE = y
MTK_SAVE_AUTO_BAUD_RATE_ENABLE = y
