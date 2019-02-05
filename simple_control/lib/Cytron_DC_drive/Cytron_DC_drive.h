#include <mbed.h>
class Cytron_DC {
public:
Cytron_DC(PinName dir,PinName pwm);
void motor_drive(float power);//power = [-1,1]
private:
DigitalOut   *dir_pin;
PwmOut   *pwm_pin;
};
