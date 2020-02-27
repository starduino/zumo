#ifndef tuning_h
#define tuning_h

enum {
  line_detection_near_wheel_power = 100,
  line_detection_far_wheel_power = -100,
  line_detection_back_up_time = 300,
  line_detection_turn_time = 1000
};

enum {
  charge_full_power = 100,
  charge_reduced_power = 50
};

enum {
  seeking_near_wheel_power = 40,
  seeking_far_wheel_power = 100
};

#endif