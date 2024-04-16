
BOARD_SRC = driver/board/mt2625_hdk
COMPONENT_SRC = driver/board/component

#digit_clock
APP_FILES += $(BOARD_SRC)/digit_clock/digit_clock_bg.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_battery.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_cal.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_hr.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_kcal.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_km.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_loc.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_morning.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_night.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_afternoon.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_step.c

APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_0.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_1.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_2.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_3.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_4.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_5.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_6.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_7.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_8.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_9.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_date/digit_date_div.c

APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_0.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_1.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_2.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_3.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_4.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_5.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_6.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_7.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_8.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_9.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_small/digit_small_percent.c

APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_0.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_1.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_2.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_3.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_4.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_5.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_6.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_7.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_8.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_9.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_time/digit_time_point.c

APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_0.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_1.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_2.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_3.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_4.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_5.c
APP_FILES += $(BOARD_SRC)/digit_clock/digit_week/digit_week_6.c

C_FILES  += $(BOARD_SRC)/digit_clock/digit_clock.c
CFLAGS  += -I$(BOARD_SRC)/digit_clock




