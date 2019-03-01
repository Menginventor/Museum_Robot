#include <mbed.h>
#include <AQE.h>
#include "QEI.h"]
#include <Cytron_DC_drive.h>
//simple wheel's position control

//AQE QE_L (PC_5,PC_6);
//AQE QE_R (PA_11,PB_12);
/*
Encoder : 500P/R
<GND,Black>,<INDEX,Yellow>,<Chanel A,Green>,<VCC,Red>,<Chanel B,White>
Motor
reduction 1/51**change to 1:42

noload speed 100 RPM
*/
QEI wheel_L (PC_6, PC_5, NC, 500, QEI::X4_ENCODING);// (PinName channelA, PinName channelB, PinName index, int pulsesPerRev, Encoding encoding=X2_ENCODING)
QEI wheel_R (PA_12, PA_11, NC, 500, QEI::X4_ENCODING);
//X4 Encoding means 4 step per pulse's phase
//with 500 PPR Encoder means 500(4) = 2000 step per encoder's revolution
//with 1/51 Gear reduction means 1 wheel revolution equal 42 encoder's revolution which means 84000 reading step.
Cytron_DC motor_L (PB_13,PB_14);//(PinName dir,PinName pwm);
Cytron_DC motor_R (PB_15,PB_1);//(PinName dir,PinName pwm);
Serial pc(USBTX, USBRX);
Timer t;
Ticker control_loop_tick;
float desireSpeed_L,desireSpeed_R;
float control_loop_period =  1.0/100.0;//100-Hz
int wheel_L_pos = 0,wheel_R_pos = 0;
int wheel_L_goal_pos = 7421680;
void control_loop_fc();
int main() {
        pc.baud(115200);
        control_loop_tick.attach(&control_loop_fc,control_loop_period);//100-Hz
        t.start();
        //desireSpeed_L = 50000;
        //desireSpeed_R = 50000;
        float motor_power = 0.0;
        while(1) {

                //if(t.read() >2)motor_power = 2.0;
                //float motor_power = sin(t.read());

                //pc.printf("%f,%f,%f,%f\n",my_QE.velocity,-motor_power*100000,100000.0,-100000.0);
                //pc.printf("%f\n",my_QE.pos_count_period_debug *1000000.0);
                //motor_L.motor_drive(motor_power);
                //motor_R.motor_drive(motor_power);
                //motor_L.motor_drive(1);
                //motor_R.motor_drive(1);
                //pc.printf("%f\r\n",motor_power);
                //float motor_power = int(t.read())%6/3;
                //desireSpeed_L = 50000*motor_power;
                //desireSpeed_R = 50000*motor_power;
        }
}
void control_loop_fc(){
    //pc.printf("%f,%f\r\n",QE_L.velocity,QE_R.velocity); 	getCurrentState (
    //pc.printf("%d,%d\r\n",wheel_L.getPulses(),wheel_R.getPulses());
    float error_pos_L = float(wheel_L_goal_pos-wheel_L_pos);
    //pc.printf("%f\n",error_pos_L);
    desireSpeed_L = error_pos_L;
    if (desireSpeed_L>50000)desireSpeed_L = 50000;
    ///
    float KP_L = 1.0/100000.0;
    float KP_R = 1.0/100000.0;
    float KI_L = 1.0/10000.0;
    float KI_R = 1.0/10000.0;
    float pulseL = float(wheel_L.getPulses());
    float pulseR = float(wheel_R.getPulses());
    wheel_L_pos += wheel_L.getPulses();
    wheel_R_pos += wheel_R.getPulses();
    float crr_speed_L = pulseL/control_loop_period;
    float crr_speed_R = pulseR/control_loop_period;
    //max 109,600 P/S
    wheel_L.reset();
    wheel_R.reset();

    float max_acc = 50000;
    static float integral_err_L = 0;
    static float integral_err_R = 0;

    static float motor_control_speed_L = 0;
    static float motor_control_speed_R = 0;
    if(abs(desireSpeed_L-motor_control_speed_L)<max_acc*control_loop_period){
      motor_control_speed_L = desireSpeed_L;
    }
    else {
      if(desireSpeed_L > motor_control_speed_L){
        motor_control_speed_L += max_acc*control_loop_period;
      }
      else{
        motor_control_speed_L -= max_acc*control_loop_period;
      }
    }
    if(abs(desireSpeed_R-motor_control_speed_R)<max_acc*control_loop_period){
      motor_control_speed_R = desireSpeed_R;
    }
    else {
      if(desireSpeed_R > motor_control_speed_R){
        motor_control_speed_R += max_acc*control_loop_period;
      }
      else{
        motor_control_speed_R -= max_acc*control_loop_period;
      }
    }
    float err_L = motor_control_speed_L-crr_speed_L;
    float err_R = motor_control_speed_R-crr_speed_R;

    //float err_L = desireSpeed_L-crr_speed_L;
    //float err_R = desireSpeed_R-crr_speed_R;
    integral_err_L += err_L*control_loop_period;
    integral_err_R += err_R*control_loop_period;
    float u_L = KP_L*err_L + KI_L*integral_err_L;
    float u_R = KP_R*err_R + KI_R*integral_err_R;

    motor_L.motor_drive(u_L);
    motor_R.motor_drive(u_R);
    //pc.printf("%f\t%f\t%f\t%f\t%f\r\n",t.read(),desireSpeed_L,desireSpeed_R,crr_speed_L,crr_speed_R);
      //pc.printf("%f\t%f\r\n",crr_speed_L,crr_speed_R);
  //pc.printf("%f\t%f\t%f\t%f\r\n",desireSpeed_L,desireSpeed_R,crr_speed_L,crr_speed_R);
      //pc.printf("%f\t%f\r\n",desireSpeed_L,crr_speed_L);
      pc.printf("%f\n",error_pos_L );
    //pc.printf("%f\t%f\t%f\t%f\t%f\r\n",desireSpeed_L,motor_control_speed_L,crr_speed_L,err_L,u_L*50000);
    //pc.printf("wheel L = %d,wheel R = %d\n", wheel_L_pos,wheel_R_pos);
}
