/*!
 * @file
 * @brief
 */

#ifndef tuning_h
#define tuning_h

enum {
  turn_time_360_degree = 630
};

enum {
  line_detection_near_wheel_power = 100,
  line_detection_far_wheel_power = -100,
  line_detection_back_up_time = 150,
  line_detection_turn_time = turn_time_360_degree / 3
};

enum {
  spin_on_init_near_wheel_power = 100,
  spin_on_init_far_wheel_power = -100,
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
