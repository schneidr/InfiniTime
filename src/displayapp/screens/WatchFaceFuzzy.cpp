#include "WatchFaceFuzzy.h"

#include <date/date.h>
#include <lvgl/lvgl.h>
#include <cstdio>
#include "BatteryIcon.h"
#include "BleIcon.h"
#include "NotificationIcon.h"
#include "Symbols.h"
#include "components/battery/BatteryController.h"
#include "components/ble/BleController.h"
#include "components/ble/NotificationManager.h"
#include "components/heartrate/HeartRateController.h"
#include "components/motion/MotionController.h"
using namespace Pinetime::Applications::Screens;

char const *WatchFaceFuzzy::hourDescriptions[] = {
        "twelve", "one", "two", "three", 
        "four", "five", "six", "seven", 
        "eight", "nine", "ten", "eleven"
      };


WatchFaceFuzzy::WatchFaceFuzzy(DisplayApp* app,
                                   Controllers::DateTime& dateTimeController,
                                   Controllers::Battery& batteryController,
                                   Controllers::Ble& bleController,
                                   Controllers::NotificationManager& notificatioManager,
                                   Controllers::Settings& settingsController)
  : Screen(app),
    currentDateTime {{}},
    dateTimeController {dateTimeController},
    batteryController {batteryController},
    bleController {bleController},
    notificatioManager {notificatioManager},
    settingsController {settingsController} {
    settingsController.SetClockFace(0);

    batteryIcon = lv_label_create(lv_scr_act(), nullptr);
    lv_label_set_text(batteryIcon, Symbols::batteryFull);
    lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);

    batteryPlug = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_color(batteryPlug, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
    lv_label_set_text(batteryPlug, Symbols::plug);
    lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    bleIcon = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_color(bleIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));
    lv_label_set_text(bleIcon, Symbols::bluetooth);
    lv_obj_align(bleIcon, batteryPlug, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    notificationIcon = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_color(notificationIcon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, lv_color_hex(0x00FF00));
    lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(false));
    lv_obj_align(notificationIcon, nullptr, LV_ALIGN_IN_TOP_LEFT, 0, 0);

    label_time = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_style_local_text_font(label_time, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_extrabold_compressed);

    lv_obj_align(label_time, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 0, 0);

    backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
    lv_obj_set_click(backgroundLabel, true);
    lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
    lv_obj_set_size(backgroundLabel, 240, 240);
    lv_obj_set_pos(backgroundLabel, 0, 0);
    lv_label_set_text(backgroundLabel, "");
  }

  WatchFaceFuzzy::~WatchFaceFuzzy() {
    lv_obj_clean(lv_scr_act());
  }

  void WatchFaceFuzzy::Refresh() {
    batteryPercentRemaining = batteryController.PercentRemaining();
    if (batteryPercentRemaining.IsUpdated()) {
      auto batteryPercent = batteryPercentRemaining.Get();
      lv_label_set_text(batteryIcon, BatteryIcon::GetBatteryIcon(batteryPercent));
      auto isCharging = batteryController.IsCharging() or batteryController.IsPowerPresent();
      lv_label_set_text(batteryPlug, BatteryIcon::GetPlugIcon(isCharging));
    }

    bleState = bleController.IsConnected();
    if (bleState.IsUpdated()) {
      lv_label_set_text(bleIcon, BleIcon::GetIcon(bleState.Get()));
    }
    lv_obj_align(batteryIcon, lv_scr_act(), LV_ALIGN_IN_TOP_RIGHT, 0, 0);
    lv_obj_align(batteryPlug, batteryIcon, LV_ALIGN_OUT_LEFT_MID, -5, 0);
    lv_obj_align(bleIcon, batteryPlug, LV_ALIGN_OUT_LEFT_MID, -5, 0);

    notificationState = notificatioManager.AreNewNotificationsAvailable();
    if (notificationState.IsUpdated()) {
      lv_label_set_text(notificationIcon, NotificationIcon::GetIcon(notificationState.Get()));
    }

    currentDateTime = dateTimeController.CurrentDateTime();

    if (currentDateTime.IsUpdated()) {
      auto newDateTime = currentDateTime.Get();

      auto dp = date::floor<date::days>(newDateTime);
      auto time = date::make_time(newDateTime - dp);

      int hour = time.hours().count();
      auto minute = time.minutes().count();

      char *minutesChar;
      char *betweenChar;
      char *afterChar;

      switch (minute) {
        case 58:
        case 59:
          hour++;
        case 0:
        case 1:
        case 2:
          sprintf(afterChar, "o' clock"); /// TODO: muss nach hinten
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
          sprintf(minutesChar, "five");
          sprintf(betweenChar, "past");
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
          sprintf(minutesChar, "ten");
          sprintf(betweenChar, "past");
        case 13:
        case 14:
        case 15:
        case 16:
        case 17:
          sprintf(minutesChar, "quarter");
          sprintf(betweenChar, "past");
        case 18:
        case 19:
        case 20:
        case 21:
        case 22:
          sprintf(minutesChar, "twenty");
          sprintf(betweenChar, "past");
        case 23:
        case 24:
        case 25:
        case 26:
        case 27:
          sprintf(minutesChar, "five");
          sprintf(betweenChar, "to half");
          hour++;
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
          sprintf(minutesChar, "");
          sprintf(betweenChar, "half");
          hour++;
        case 33:
        case 34:
        case 35:
        case 36:
        case 37:
          sprintf(minutesChar, "five");
          sprintf(betweenChar, "past half");
          hour++;
        case 38:
        case 39:
        case 40:
        case 41:
        case 42:
          sprintf(minutesChar, "twenty");
          sprintf(betweenChar, "to");
          hour++;
        case 43:
        case 44:
        case 45:
        case 46:
        case 47:
          sprintf(minutesChar, "quarter");
          sprintf(betweenChar, "to");
          hour++;
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
          sprintf(minutesChar, "ten");
          sprintf(betweenChar, "to");
          hour++;
        case 53:
        case 54:
        case 55:
        case 56:
        case 57:
          sprintf(minutesChar, "five");
          sprintf(betweenChar, "to");
          hour++;
      }

      char *hoursChar;
      if (hour > 11) {
        hour -= 12;
      }
      sprintf(hoursChar, hourDescriptions[hour]);

      lv_label_set_text_fmt(label_time, "%s %s %s", minutesChar, betweenChar, hoursChar);
    }
  }
}
