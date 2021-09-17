#pragma once

#include <cstdint>
#include <lvgl/lvgl.h>
#include "components/settings/Settings.h"
#include "displayapp/screens/Screen.h"
#include "displayapp/screens/ScreenList.h"

namespace Pinetime {

  namespace Applications {
    namespace Screens {

      class SettingQuietTime : public Screen {
      public:
        SettingQuietTime(Pinetime::Applications::DisplayApp* app, Pinetime::Controllers::Settings& settingsController);
        ~SettingQuietTime() override;
        void OnButtonEvent(lv_obj_t* obj, lv_event_t event);

      private:
        uint8_t fromHoursToSet = 0;
        uint8_t fromMinutesToSet = 0;
        Controllers::Settings& settingsController;

        ScreenList<2> screens;
        std::unique_ptr<Screen> CreateScreen1();
        std::unique_ptr<Screen> CreateScreen2();

        lv_obj_t * labelFrom, * timeFrom, * labelTo,
          * btnFromHoursUp, * txtFromHUp, * btnFromHoursDown, * txtFromHDown,
          * btnFromMinutesUp, * txtFromMUp, * btnFromMinutesDown, * txtFromMDown;
      };
    }
  }
}
