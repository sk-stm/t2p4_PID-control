#include "PID.h"

using namespace std;

/*
* TODO: Complete the PID class.
*/

PID::PID() {}

PID::~PID() {}

void PID::Init(double Kp, double Ki, double Kd, int twiddle_steps) {
    this->Kp = Kp;
    this->Ki = Ki;
    this->Kd = Kd;

    this->p_error = 0;
    this->d_error = 0;
    this->i_error = 0;

    this->dp[0] = 0.01*this->Kp;
    this->dp[1] = 0.01*this->Ki;
    this->dp[2] = 0.01*this->Kd;

    this->step = 0;
}

void PID::UpdateError(double cte) {

    //init error
    if(step == 0){
        best_err = fabs(cte);
    }

    if(step < this->num_of_steps){
        //twittle
        std::cout << "iteration: " << step << "best_error: " << best_err << std::endl;
        if(!this->tried_adding && !this->tried_subtracting){
            //try adding
            this->Kp += dp[0];
            this->Ki += dp[1];
            this->Kd += dp[2];
            this->tried_adding = true;
        }else{
            if(this->tried_adding && !this->tried_subtracting){
                // try subtracting
                this->Kp -= 2*dp[0];
                this->Ki -= 2*dp[1];
                this->Kd -= 2*dp[2];
                this->tried_subtracting = true;
            }else{
                // update twiddle parameters
                this->tried_subtracting = false;
                this->tried_adding = false;
                if(fabs(cte) < best_err){
                    best_err = fabs(cte);
                    dp[0] *= this->twiddle_upd_param;
                    dp[1] *= this->twiddle_upd_param;
                    dp[2] *= this->twiddle_upd_param;
                }else{
                    dp[0] *= this->twiddle_dwn_param;
                    dp[1] *= this->twiddle_dwn_param;
                    dp[2] *= this->twiddle_dwn_param;
                }
            }
        }
        step += 1;
    }

    // finally update the errors for PID-control
    this->d_error = cte - this->p_error;
    this->p_error = cte;
    this->i_error += cte;
}

double PID::TotalError() {
    return -this->Kp*this->p_error - this->Kd*this->d_error - this->Ki*this->i_error;
}

