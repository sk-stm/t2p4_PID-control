#ifndef PID_H
#define PID_H
#include<math.h>
#include <iostream>

class PID {

private:
    unsigned int step = 0;
    double const twiddle_upd_param = 1.1;
    double const twiddle_dwn_param = 0.9;
    double dp[3] = {1.0, 1.0, 1.0};
    //TODO : double max would be better then a high number
    double best_err = 1000000;
    unsigned int num_of_steps = 1000;
    bool tried_adding = false;
    bool tried_subtracting = false;

public:
  /*
  * Errors
  */
  double p_error;
  double i_error;
  double d_error;

  /*
  * Coefficients
  */
  double Kp;
  double Ki;
  double Kd;

  /*
  * Constructor
  */
  PID();

  /*
  * Destructor.
  */
  virtual ~PID();

  /*
  * Initialize PID.
  */
  void Init(double Kp, double Ki, double Kd, int twiddle_steps);

  /*
  * Update the PID error variables given cross track error.
  */
  void UpdateError(double cte);

  /*
  * Calculate the total PID error.
  */
  double TotalError();
};

#endif /* PID_H */
