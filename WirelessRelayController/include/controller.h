#include <WString.h>

#include "utils.h"

class Controller {
   private:
    int index;

   public:
    const String name;
    const uint8_t GPIO;

   private:
    uint8_t value;

    void savePresistentValue();
    uint8_t loadPresistentValue();

   public:
    Controller(int index, String name, uint8_t GPIO, uint8_t value = OFF, void (*onChange)(Controller*) = nullptr, boolean presist_value = true);

    String getValue();
    uint8_t getValueRaw();
    void setValue(uint8_t _value);

    // Shorthand to set HIGH to LOW and vice versa
    void toggleValue();
    // Optional callback function on value change
    void (*onChange)(Controller*);

   private:
    boolean presist_value;
};
