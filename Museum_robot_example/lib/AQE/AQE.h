#include <mbed.h>
class AQE {
private:
  int Q_state_compare(int pre_state,int crr_state);
  InterruptIn *enA;
  InterruptIn *enB;
  DigitalIn   *d_enA;
  DigitalIn   *d_enB;
  Ticker velocity_update_ticker;
  void velocity_update();
  int velocity_update_state = -1;

public:


int q_state = 0;
int sc_flag = 0;//state change flag
long pos = 0;
long pos_count = 0;//for velocity_update
Timer pos_count_timer;
float pos_count_period = 0;
float pos_count_period_debug = 0;
float  velocity = 0;
float velocity_update_period = 1.0/100.0; //100 Hz default

AQE(PinName pin_A,PinName pin_B);
void get_q_state();

};
