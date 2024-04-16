ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

#78MHz
MTK_SYSTEM_CLOCK_SET = 78000
