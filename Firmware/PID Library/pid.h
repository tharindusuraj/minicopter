#ifndef PID_H
#define PID_H

class PID {
    
    private:

        float temp_error;
        float error_d;
        float last_error;
        float error_total;
        float pid_output;
        float kp;
        float ki;
        float kd;
        float error_i_limit;
        float output_limit;
	float dt;

    
    public:

        void init(const float p, const float i, const float d, const float i_limit, const float out_limit, const float sampleTime);
        void reset();
        float limit(float value, float limit);
        float update(float input, float set_point);
        float updateGivenError(float error);
	void change_gains(float p, float i, float d);
	void change_kp(float p);
	void change_ki(float i);
	void change_kd(float d);


    };

#endif