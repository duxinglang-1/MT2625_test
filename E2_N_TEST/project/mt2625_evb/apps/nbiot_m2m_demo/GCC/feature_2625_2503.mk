ifeq ($(PROJ_PATH),)
include feature.mk
else
include $(PROJ_PATH)/feature.mk
endif

# Indicate if this is a dual mode project or not
MT2503_2625_DUAL = y
