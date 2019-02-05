#include <Cytron_DC_drive.h>
Cytron_DC::Cytron_DC(PinName dir,PinName pwm){

        dir_pin = new DigitalOut(dir);
        pwm_pin = new PwmOut(pwm);
        pwm_pin->period(1.0/20000.0);//SET TO 2kHZ
}
void Cytron_DC::motor_drive(float power){
        if (power >=0) {
                dir_pin->write(0);
                pwm_pin->write(power);
        }
        else{
                dir_pin->write(1);
                pwm_pin->write(-power);
        }
}
