#include "SettingQuietTime.h"
#include <lvgl/lvgl.h>
#include "displayapp/DisplayApp.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/Symbols.h"
#include "components/settings/Settings.h"

using namespace Pinetime::Applications::Screens;

namespace {
  static void event_handler(lv_obj_t* obj, lv_event_t event) {
    //SettingQuietTime* screen = static_cast<SettingQuietTime*>(obj->user_data);
    // screen->UpdateSelected(obj, event);
  }
}

static void btnEventHandler(lv_obj_t* obj, lv_event_t event) {
  SettingQuietTime* screen = static_cast<SettingQuietTime*>(obj->user_data);
  screen->OnButtonEvent(obj, event);
}

SettingQuietTime::SettingQuietTime(Pinetime::Applications::DisplayApp* app, Pinetime::Controllers::Settings& settingsController)
  : Screen(app), settingsController {settingsController},
    screens {app,
             0,
             {[this]() -> std::unique_ptr<Screen> {
                return CreateScreen1();
              },
              [this]() -> std::unique_ptr<Screen> {
                return CreateScreen2();
              }},
             Screens::ScreenListModes::UpDown} {
}

SettingQuietTime::~SettingQuietTime() {
  lv_obj_clean(lv_scr_act());
  settingsController.SaveSettings();
}

std::unique_ptr<Screen> SettingQuietTime::CreateScreen1() {
  lv_obj_t * container1 = lv_cont_create(lv_scr_act(), nullptr);

  lv_obj_set_style_local_bg_opa(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_pad_all(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);
  lv_obj_set_style_local_pad_inner(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
  lv_obj_set_style_local_border_width(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_pos(container1, 30, 60);
  lv_obj_set_width(container1, LV_HOR_RES - 50);
  lv_obj_set_height(container1, LV_VER_RES - 60);
  lv_cont_set_layout(container1, LV_LAYOUT_COLUMN_LEFT);

  lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_static(title,"Quiet Time");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  
  lv_label_set_text_static(icon, Symbols::clock);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  labelFrom = lv_label_create(lv_scr_act(), nullptr);
  //lv_obj_set_style_local_text_font(labelFrom, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
  lv_obj_set_style_local_text_color(labelFrom, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text_fmt(labelFrom, "From");
  lv_obj_align(labelFrom, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, -60);

  timeFrom = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_font(timeFrom, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, &jetbrains_mono_76);
  lv_obj_set_style_local_text_color(timeFrom, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text_fmt(timeFrom, "%02d:%02d", fromHoursToSet, fromMinutesToSet);
  lv_obj_align(timeFrom, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 0, +25);

  btnFromHoursUp = lv_btn_create(lv_scr_act(), nullptr);
  btnFromHoursUp->user_data = this;
  lv_obj_set_event_cb(btnFromHoursUp, btnEventHandler);
  lv_obj_align(btnFromHoursUp, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 20, -40);
  lv_obj_set_height(btnFromHoursUp, 40);
  lv_obj_set_width(btnFromHoursUp, 60);
  txtFromHUp = lv_label_create(btnFromHoursUp, nullptr);
  lv_label_set_text(txtFromHUp, "+");
  
  btnFromHoursDown = lv_btn_create(lv_scr_act(), nullptr);
  btnFromHoursDown->user_data = this;
  lv_obj_set_event_cb(btnFromHoursDown, btnEventHandler);
  lv_obj_align(btnFromHoursDown, lv_scr_act(), LV_ALIGN_IN_LEFT_MID, 20, +80);
  lv_obj_set_height(btnFromHoursDown, 40);
  lv_obj_set_width(btnFromHoursDown, 60);
  txtFromHDown = lv_label_create(btnFromHoursDown, nullptr);
  lv_label_set_text(txtFromHDown, "-");

  btnFromMinutesUp = lv_btn_create(lv_scr_act(), nullptr);
  btnFromMinutesUp->user_data = this;
  lv_obj_set_event_cb(btnFromMinutesUp, btnEventHandler);
  lv_obj_align(btnFromMinutesUp, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 10, -40);
  lv_obj_set_height(btnFromMinutesUp, 40);
  lv_obj_set_width(btnFromMinutesUp, 60);
  txtFromMUp = lv_label_create(btnFromMinutesUp, nullptr);
  lv_label_set_text(txtFromMUp, "+");
  
  btnFromMinutesDown = lv_btn_create(lv_scr_act(), nullptr);
  btnFromMinutesDown->user_data = this;
  lv_obj_set_event_cb(btnFromMinutesDown, btnEventHandler);
  lv_obj_align(btnFromMinutesDown, lv_scr_act(), LV_ALIGN_IN_RIGHT_MID, 10, +80);
  lv_obj_set_height(btnFromMinutesDown, 40);
  lv_obj_set_width(btnFromMinutesDown, 60);
  txtFromMDown = lv_label_create(btnFromMinutesDown, nullptr);
  lv_label_set_text(txtFromMDown, "-");

  //return std::make_unique<Screens::Screen>(0, 2, app, settingsController);
  return std::unique_ptr<Screen>(new Screens::Screen(0, 2, app, settingsController));
}

std::unique_ptr<Screen> SettingQuietTime::CreateScreen2() {
  lv_obj_t * container1 = lv_cont_create(lv_scr_act(), nullptr);

  lv_obj_set_style_local_bg_opa(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, LV_OPA_TRANSP);
  lv_obj_set_style_local_pad_all(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 10);
  lv_obj_set_style_local_pad_inner(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 5);
  lv_obj_set_style_local_border_width(container1, LV_CONT_PART_MAIN, LV_STATE_DEFAULT, 0);
  lv_obj_set_pos(container1, 30, 60);
  lv_obj_set_width(container1, LV_HOR_RES - 50);
  lv_obj_set_height(container1, LV_VER_RES - 60);
  lv_cont_set_layout(container1, LV_LAYOUT_COLUMN_LEFT);

  lv_obj_t * title = lv_label_create(lv_scr_act(), NULL);  
  lv_label_set_text_static(title,"Quiet Time");
  lv_label_set_align(title, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(title, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  
  lv_label_set_text_static(icon, Symbols::clock);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, title, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  labelTo = lv_label_create(lv_scr_act(), nullptr);
  lv_obj_set_style_local_text_color(labelTo, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text_fmt(labelTo, "From");
  lv_obj_align(labelTo, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 0, -60);

  return std::make_unique<Screens::Screen>(1, 2, app, settingsController);
}

void SettingQuietTime::OnButtonEvent(lv_obj_t* obj, lv_event_t event) {
  if (event == LV_EVENT_CLICKED) {
    if (obj == btnFromMinutesUp) {
      fromMinutesToSet += 15;
      if (fromMinutesToSet >= 60) {
        fromMinutesToSet = 0;
      }
    }
    else if (obj == btnFromMinutesDown) {
      fromMinutesToSet -= 15;
      if (fromMinutesToSet < 0) {
        fromMinutesToSet = 45;
      }
    }
    if (obj == btnFromHoursUp) {
      fromHoursToSet++;
      if (fromHoursToSet > 23) {
        fromHoursToSet = 0;
      }
    }
    else if (obj == btnFromHoursDown) {
      fromHoursToSet--;
      if (fromHoursToSet > 23) {
        fromHoursToSet = 23;
      }
    }
    lv_label_set_text_fmt(timeFrom, "%02d:%02d", fromHoursToSet, fromMinutesToSet);
  }
}

bool SettingQuietTime::Refresh() {
  return running;
}
