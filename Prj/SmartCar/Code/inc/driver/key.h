#ifndef __KEY_H__
#define __KEY_H__

#include "include.h"
#include "define.h"
#include "tft_r61509.h"
#include "params.h"
#include "flash.h"
#include "servo.h"
#include "motor.h"
#include "servo_control.h"
#include "motor_control.h"

void keyInit();
void keyStartScan();
void keyStopScan();

#endif