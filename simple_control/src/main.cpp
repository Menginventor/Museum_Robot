/*
*simple control


*/
#include <mbed.h>
#include "QEI.h"]
#include <Cytron_DC_drive.h>
#include <cstdlib>
#define UNDEFINE_COMMUNICATION -1
#define ASCII_COMMUNICATION 1
//AQE QE_L (PC_5,PC_6);
//AQE QE_R (PA_11,PB_12);
/*
Encoder : 500P/R
<GND,Black>,<INDEX,Yellow>,<Chanel A,Green>,<VCC,Red>,<Chanel B,White>
Motor
reduction 1/51
noload speed 100 RPM
*/
QEI wheel_L (PC_6, PC_5, NC, 500, QEI::X4_ENCODING);// (PinName channelA, PinName channelB, PinName index, int pulsesPerRev, Encoding encoding=X2_ENCODING)
QEI wheel_R (PA_12, PA_11, NC, 500, QEI::X4_ENCODING);
/*
*X4_ENCODING Provide 4 count per pulse.
*From given 500P/R encoder means 2000 count per revolution
*
*/
Cytron_DC motor_L (PB_13,PB_14);//(PinName dir,PinName pwm);
Cytron_DC motor_R (PB_15,PB_1);//(PinName dir,PinName pwm);
Serial pc(USBTX, USBRX);
Timer t;
Ticker control_loop_tick;
float desireSpeed_L,desireSpeed_R;
float control_loop_period =  1.0/100.0;//100-Hz

void control_loop_fc();
char dataSerial = '\0';
bool serialFlag = false;
char Serial_buffer[100];
int Serial_buffer_index = 0;
int comunication_mode = UNDEFINE_COMMUNICATION;
void clear_Serial_buffer(){
  for(int i = 0;i<sizeof(Serial_buffer);i++){
    Serial_buffer[i] = '\0';
  }
  Serial_buffer_index = 0;
}
bool isNumber(char ch){
  if (ch >= '0' && ch<= '9')return true;
  else if (ch == '.' || ch == '-' || ch == '+')return true;
  else return false;
}
void process_ascii_cmd(char buffer[]){
  //pc.printf("ASCII mode:%s , %d\r\n",buffer,sizeof(buffer));
  char sub_buffer[20] = "";
  int sub_buffer_index = 0;
  float L_speed = 0.0;
  float R_speed = 0.0;

  int index = 0;
  while(buffer[index] != '\0'){
    char crr_ch = buffer[index];
    if(crr_ch != ' '){
        if(crr_ch == 'L'){
          //printf("found L\r\n");
          sub_buffer_index = 0;
          sub_buffer[sub_buffer_index] = '\0';
          index++;
          while(true){
              crr_ch = buffer[index];
              if(isNumber(crr_ch)){
                sub_buffer[sub_buffer_index] = crr_ch;
                sub_buffer_index++;
                index++;
              }
              else{
                sub_buffer[sub_buffer_index] = '\0';
                //printf("L = \"%s\"\n",sub_buffer);
                L_speed = atof(sub_buffer);
                desireSpeed_L = L_speed;
                //printf("L val = \"%f\"\n",value);
                break;
              }
          }
        }//found L

        else if(crr_ch == 'R'){
          //printf("found R\r\n");
          sub_buffer_index = 0;
          sub_buffer[sub_buffer_index] = '\0';
          index++;
          while(true){
              crr_ch = buffer[index];
              if(isNumber(crr_ch)){
                sub_buffer[sub_buffer_index] = crr_ch;
                sub_buffer_index++;
                index++;
              }
              else{
                sub_buffer[sub_buffer_index] = '\0';
                R_speed= atof(sub_buffer);
                desireSpeed_R = R_speed;
                break;
              }

          }
        }//found R


    }
    index++;
  }


}
int main() {
        pc.baud(9600);
        pc.printf("%s\n","startprogram" );

        control_loop_tick.attach(&control_loop_fc,control_loop_period);//100-Hz
        t.start();
        //desireSpeed_L = 50000;
        //desireSpeed_R = 50000;


        float timer = t.read();

        while(1) {
            if(pc.readable()){

              char serial_read = pc.getc();

              if(comunication_mode==ASCII_COMMUNICATION&&(serial_read == '\r' || serial_read == '\n')){
                if(Serial_buffer_index!= 0){
                  process_ascii_cmd(Serial_buffer);
                  printf("speed L = %f,speed R = %f\r\n", desireSpeed_L,desireSpeed_R);
                  //pc.printf("ASCII mode:%s\r\n",Serial_buffer);
                }
                clear_Serial_buffer();
              }
              else{
                if(Serial_buffer_index == 0 )comunication_mode=ASCII_COMMUNICATION;
                Serial_buffer[Serial_buffer_index] = serial_read;
                Serial_buffer_index++;
              }

            }
            //printf("speed L = %f,speed R = %f\r\n", desireSpeed_L,desireSpeed_R);
            if(t.read()-timer >=1){
              timer +=1.0;
              printf("OK for %d\n",int(timer) );
            }
        }
        printf("exit program\n" );
}
void control_loop_fc(){

    float KP_L = 1.0/100000.0;
    float KP_R = 1.0/100000.0;
    float KI_L = 1.0/10000.0;
    float KI_R = 1.0/10000.0;
    float pulseL = float(wheel_L.getPulses());
    float pulseR = float(wheel_R.getPulses());

    float crr_speed_L = pulseL/control_loop_period;
    float crr_speed_R = pulseR/control_loop_period;
    //max 109,600 P/S
    wheel_L.reset();
    wheel_R.reset();

    float max_acc = 200000.0;
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
    //pc.printf("%f\t%f\t%f\t%f\t%f\r\n",desireSpeed_L,motor_control_speed_L,crr_speed_L,err_L,u_L*50000);
    //pc.printf("wheel L = %d,wheel R = %d\n", wheel_L_pos,wheel_R_pos);
}
