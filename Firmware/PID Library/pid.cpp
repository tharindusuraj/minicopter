#include "pid.h"

void PID::init(const float p, const float i, const float d, const float i_limit, const float out_limit, const float sampleTime){
    kp = p;
    ki = i;
    kd = d;
    temp_error  = 0.0;
    last_error  = 0.0;
    error_d     = 0.0;
    error_total = 0.0;
    pid_output  = 0.0;
    error_i_limit = i_limit;
    output_limit  = out_limit;
    dt = sampleTime; 

}

void PID::reset(){

    temp_error  = 0.0;
    last_error  = 0.0;
    error_d     = 0.0;
    error_total = 0.0;
    pid_output  = 0.0;

}

float PID::update(float input, float set_point){

    temp_error  = input - set_point;
    error_d     = temp_error - last_error;
    last_error  = temp_error;

    error_total += temp_error*dt;
    error_total = limit(error_total, error_i_limit); 

    pid_output  = (temp_error*kp) + (error_d*kd) + (error_total*ki);
    pid_output  = limit(pid_output, output_limit);

    return pid_output;

}

float PID::updateGivenError(float error){
    error_d     = error - last_error;
    last_error  = error;

    error_total = error_total + (error*ki);
    error_total = limit(error_total, error_i_limit); 

    pid_output  = (error*kp) + (error_d*kd) + error_total;
    pid_output  = limit(pid_output, output_limit);

    return pid_output;
}

float PID::limit(float value, float limit){

    if (value > limit)  value =  limit;
    if (value < -limit) value = -limit;

    return value;
}

void PID::change_gains(float p, float i, float d){
	kp = p;
	ki = i;
	kd = d;
}


void PID::change_kp(float p){
	kp = p;
}

void PID::change_ki(float i){
	ki = i;
}

void PID::change_kd(float d){
	kd = d;
}