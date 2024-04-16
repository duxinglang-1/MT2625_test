ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

MTK_SECURE_BOOT_ENABLE = y
MTK_ANTI_CLONE_ENABLE = y
BL_FEATURE = feature_secure.mk

