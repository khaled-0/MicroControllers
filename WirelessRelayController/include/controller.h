#include <WString.h>

#include "utils.h"

class Controller {
   public:
    const String name;
    const uint8_t GPIO;

   private:
    uint8_t value;

   public:
    Controller(String name, uint8_t GPIO, uint8_t value = OFF, void (*onChange)(Controller) = nullptr);

    String getValue();
    uint8_t getValueRaw();
    void setValue(uint8_t _value);

    // Shorthand to set HIGH to LOW and vice versa
    void toggleValue();
    // Optional callback function on value change
    void (*onChange)(Controller);
};
