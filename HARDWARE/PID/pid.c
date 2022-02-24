/*
 * PID Controller Implementation in C
 * 
 * Created by Joshua Saxby (aka @saxbophone) on 1 Jan, 2016
 * 
 * My own attempt at implementing the PID algorithm in some (hopefully) clean, understandable C.
 *
 * See LICENSE for licensing details.
 */

#include "pid.h"
#include "config.h"

PID_State pid_iterate(PID_Calibration calibration, PID_State state) {
    double error = state.target - state.actual;
    state.integral += (error * state.time_delta);
    double derivative = (error - state.previous_error) / state.time_delta;
    state.output = (
        (calibration.kp * error) + (calibration.ki * state.integral) + (calibration.kd * derivative)
    );
	
    state.previous_error = error;
    return state;
}
