#include <AQE.h>
void AQE::get_q_state(){
        int pre_state = q_state;
        if(d_enA->read() == 0) {
                if( d_enB->read() == 0) {
                        q_state = 0;
                }
                else{//B == 1
                        q_state = 3;
                }
        }
        else{//A == 1
                if(d_enB->read() == 0) {
                        q_state = 1;
                }
                else{
                        q_state = 2;
                }
        }
        int d_pos = Q_state_compare(pre_state,q_state);
        pos += d_pos;
        if(velocity_update_state == 0){
          pos_count_timer.start();
          velocity_update_state = 1;
        }
        else if(velocity_update_state == 1){
          pos_count += d_pos;
          pos_count_period = pos_count_timer.read();
        }

        sc_flag = 1;

}
AQE::AQE(PinName pin_A,PinName pin_B){
        /*assign pin*/
        enA = new InterruptIn(pin_A);
        enB = new InterruptIn(pin_B);
        d_enA = new DigitalIn(pin_A);
        d_enB = new DigitalIn(pin_B);
        get_q_state();
        enA->rise(callback(this,&AQE::get_q_state));
        enB->rise(callback(this,&AQE::get_q_state));
        enA->fall(callback(this,&AQE::get_q_state));
        enB->fall(callback(this,&AQE::get_q_state));
        /*Ticker setup*/
        velocity_update_ticker.attach(this,&AQE::velocity_update, velocity_update_period);


}
int AQE::Q_state_compare(int pre_state,int crr_state){
      int fw_state = pre_state+1;
      int bw_state = pre_state-1;
      if(fw_state>3)fw_state -= 4;
      if(bw_state<0)fw_state += 4;
      if(crr_state == fw_state)return 1;
      else if(crr_state == bw_state)return -1;
      else return 0;
}
void AQE::velocity_update(){
  if(velocity_update_state == 1){
    velocity = float(pos_count)/pos_count_period;
    pos_count_period_debug = pos_count_period;
    pos_count = 0;

  }
  velocity_update_state = 0;
  pos_count_timer.reset();
}
