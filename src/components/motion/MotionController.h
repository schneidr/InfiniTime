#pragma once

#include <cstdint>
#include <drivers/Bma421.h>

namespace Pinetime {
  namespace Controllers {
    class MotionController {
    public:
      enum class DeviceTypes{
        Unknown,
        BMA421,
        BMA425,
      };

      void Update(int16_t x, int16_t y, int16_t z, uint32_t nbSteps);

      int16_t X() const {
        return x;
      }
      int16_t Y() const {
        return y;
      }
      int16_t Z() const {
        return z;
      }
      uint32_t NbSteps() const {
        return nbSteps[6];
      }
      uint32_t NbSteps(int day) const {
        return dummyValues[day];
      }
      bool ShouldWakeUp(bool isSleeping);

      void IsSensorOk(bool isOk);
      bool IsSensorOk() const {
        return isSensorOk;
      }

      DeviceTypes DeviceType() const {
        return deviceType;
      }

      void Init(Pinetime::Drivers::Bma421::DeviceTypes types);

      void RotateStepHistory();

    private:
      uint32_t nbSteps[7];
      uint32_t dummyValues[7] = { 3458, 7893, 2489, 11589, 5479, 7892, 1243 };
      int16_t x;
      int16_t y;
      int16_t z;
      int16_t lastYForWakeUp = 0;
      bool isSensorOk = false;
      DeviceTypes deviceType = DeviceTypes::Unknown;
    };
  }
}