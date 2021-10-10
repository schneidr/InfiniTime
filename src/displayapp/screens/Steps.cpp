#include "Steps.h"
#include <lvgl/lvgl.h>
#include "../DisplayApp.h"
#include "Symbols.h"

using namespace Pinetime::Applications::Screens;

Steps::Steps(Pinetime::Applications::DisplayApp* app,
             Controllers::MotionController& motionController,
             Controllers::Settings& settingsController)
  : Screen(app), motionController {motionController}, settingsController {settingsController} {

/*
  stepsArc = lv_arc_create(lv_scr_act(), nullptr);

  lv_obj_set_style_local_bg_opa(stepsArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, LV_OPA_0);
  lv_obj_set_style_local_border_width(stepsArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 2);
  lv_obj_set_style_local_radius(stepsArc, LV_ARC_PART_BG, LV_STATE_DEFAULT, 0);
  lv_obj_set_style_local_line_color(stepsArc, LV_ARC_PART_INDIC, LV_STATE_DEFAULT, lv_color_hex(0x0000FF));
  lv_arc_set_end_angle(stepsArc, 200);
  lv_obj_set_size(stepsArc, 220, 220);
  lv_arc_set_range(stepsArc, 0, 500);
  lv_obj_align(stepsArc, nullptr, LV_ALIGN_CENTER, 0, 0);

  stepsCount = motionController.NbSteps();

  lv_arc_set_value(stepsArc, int16_t(500 * stepsCount / settingsController.GetStepsGoal()));
*/

  uint32_t chartMax = (uint32_t)((float)settingsController.GetStepsGoal() * 1.2);

  lSteps = lv_label_create(lv_scr_act(), NULL);  
  lv_obj_set_style_local_text_color(lSteps, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_GRAY);
  lv_label_set_text_fmt(lSteps, "Steps: %li", stepsCount);
  lv_label_set_align(lSteps, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(lSteps, lv_scr_act(), LV_ALIGN_IN_TOP_MID, 15, 15);

  lv_obj_t * icon = lv_label_create(lv_scr_act(), NULL);
  lv_obj_set_style_local_text_color(icon, LV_LABEL_PART_MAIN, LV_STATE_DEFAULT, LV_COLOR_ORANGE);
  lv_label_set_text_static(icon, Symbols::shoe);
  lv_label_set_align(icon, LV_LABEL_ALIGN_CENTER);
  lv_obj_align(icon, lSteps, LV_ALIGN_OUT_LEFT_MID, -10, 0);

  stepsChart = lv_chart_create(lv_scr_act(), nullptr);

  lv_obj_set_size(stepsChart, 240, 240);
  lv_obj_align(stepsChart, nullptr, LV_ALIGN_CENTER, 0, 0);
  // https://docs.lvgl.io/7.11/widgets/chart.html#parts-and-styles
  // background
  lv_obj_set_style_local_text_color(stepsChart, LV_CHART_PART_BG, LV_STATE_DEFAULT, lv_color_hex(0xFFFFFF));
  lv_obj_set_style_local_pad_left(stepsChart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 0); // 4 * (LV_DPI / 10));
  lv_obj_set_style_local_pad_bottom(stepsChart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 25); // 3 * (LV_DPI / 10));
  lv_obj_set_style_local_pad_top(stepsChart,  LV_CHART_PART_BG, LV_STATE_DEFAULT, 25);

  // grid
  lv_obj_set_style_local_line_width(stepsChart, LV_CHART_PART_SERIES_BG, LV_STATE_DEFAULT, 1);
  lv_obj_set_style_local_line_color(stepsChart, LV_CHART_PART_SERIES_BG, LV_STATE_DEFAULT, LV_COLOR_GRAY);

  // chart
  lv_obj_set_style_local_pad_inner(stepsChart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 8);
  lv_obj_set_style_local_radius(stepsChart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 10);
  lv_obj_set_style_local_line_color(stepsChart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, lv_color_hex(0xFF0000));
  lv_obj_set_style_local_line_width(stepsChart, LV_CHART_PART_SERIES, LV_STATE_DEFAULT, 8);

  lv_chart_set_type(stepsChart, LV_CHART_TYPE_COLUMN);
  lv_chart_set_y_range(stepsChart, LV_CHART_AXIS_PRIMARY_Y, 0, chartMax);
  lv_chart_set_point_count(stepsChart, 7);
  // lv_obj_add_event_cb(stepsChart, draw_event_cb, LV_EVENT_DRAW_PART_BEGIN, NULL);

  char dayTexts[50];
  sprintf(dayTexts, "%" PRIx32 "\n%" PRIx32 "\n%" PRIx32 "\n%" PRIx32 "\n%" PRIx32 "\n%" PRIx32 "\n%" PRIx32, 
    motionController.NbSteps(0), motionController.NbSteps(1), motionController.NbSteps(2), motionController.NbSteps(3),
    motionController.NbSteps(4), motionController.NbSteps(5), motionController.NbSteps(6));

  lv_chart_set_x_tick_texts(stepsChart, "4.\n5.\n6.\n7.\n8.\n9.\n10.", 1, LV_CHART_AXIS_DRAW_LAST_TICK);
  // lv_chart_set_x_tick_length(stepsChart, 0, 0);
  lv_chart_set_x_tick_length(stepsChart, 5, 8);
  lv_chart_set_y_tick_texts(stepsChart, "10000", (uint8_t)1000, LV_CHART_AXIS_DRAW_LAST_TICK);
  lv_chart_set_y_tick_length(stepsChart, 5, 8);

  lv_chart_series_t * stepsSeries = lv_chart_add_series(stepsChart, LV_COLOR_BLUE);
  /*
  lv_chart_series_t * stepsSeriesReached = lv_chart_add_series(stepsChart, LV_COLOR_GREEN);
  lv_chart_series_t * stepsSeriesToday = lv_chart_add_series(stepsChart, LV_COLOR_YELLOW);
  for (int i = 0; i < 6; i++) {
    if (motionController.NbSteps(i) >= settingsController.GetStepsGoal()) {
      lv_chart_set_next(stepsChart, stepsSeriesReached, motionController.NbSteps(i));
      lv_chart_set_next(stepsChart, stepsSeries, 0);
    }
    else {
      lv_chart_set_next(stepsChart, stepsSeriesReached, 0);
      lv_chart_set_next(stepsChart, stepsSeries, motionController.NbSteps(i));
    }
    lv_chart_set_next(stepsChart, stepsSeriesToday, 0);    
  }
  lv_chart_set_next(stepsChart, stepsSeriesToday, motionController.NbSteps(6));
  */

  for (int i = 0; i < 7; i++) {
    lv_chart_set_next(stepsChart, stepsSeries, motionController.NbSteps(i));
  }

/*
  lv_obj_t* backgroundLabel = lv_label_create(lv_scr_act(), nullptr);
  lv_label_set_long_mode(backgroundLabel, LV_LABEL_LONG_CROP);
  lv_obj_set_size(backgroundLabel, 240, 240);
  lv_obj_set_pos(backgroundLabel, 0, 0);
  lv_label_set_text_static(backgroundLabel, "");
*/

  taskRefresh = lv_task_create(RefreshTaskCallback, 100, LV_TASK_PRIO_MID, this);
}

/*static void draw_event_cb(lv_event_t * e)
{
    lv_obj_draw_part_dsc_t * dsc = lv_event_get_draw_part_dsc(e);
    if(!lv_obj_draw_part_check_type(dsc, &lv_chart_class, LV_CHART_DRAW_PART_TICK_LABEL)) return;

    if(dsc->id == LV_CHART_AXIS_PRIMARY_X && dsc->text) {
        const char * month[] = {"Jan", "Febr", "March", "Apr", "May", "Jun", "July", "Aug", "Sept", "Oct", "Nov", "Dec"};
        dsc->text = month[dsc->value];
    }
}*/

Steps::~Steps() {
  lv_task_del(taskRefresh);
  lv_obj_clean(lv_scr_act());
}

void Steps::Refresh() {
/*
  stepsCount = motionController.NbSteps();

  lv_label_set_text_fmt(lSteps, "%li", stepsCount);
  lv_obj_align(lSteps, nullptr, LV_ALIGN_CENTER, 0, -20);
*/
  // lv_arc_set_value(stepsArc, int16_t(500 * stepsCount / settingsController.GetStepsGoal()));
}
