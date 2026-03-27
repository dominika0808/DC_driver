#ifndef _TIMER_H_
#define _TIMER_H_

#include <stdint.h>
#include "common.h"

Status_t Tim1InitPWM(void);
Status_t Tim1Start(void);
Status_t Tim1HighRightTranssistor(uint16_t duty);
Status_t Tim1LowRightTranssistor(uint16_t duty);
Status_t Tim1HighLeftTranssistor(uint16_t duty);
Status_t Tim1LowLeftTranssistor(uint16_t duty);

Status_t Tim2InitPWM(void);
Status_t Tim2Start(void);
Status_t Tim2HighRightTranssistor(uint16_t duty);
Status_t Tim2LowRightTranssistor(uint16_t duty);
Status_t Tim2HighLeftTranssistor(uint16_t duty);
Status_t Tim2LowLeftTranssistor(uint16_t duty);

Status_t Tim3InitEncoderMode(void);
Status_t Tim3Start(void);
int16_t Tim3GetValue(void);

Status_t Tim4InitEncoderMode(void);
Status_t Tim4Start(void);
int16_t Tim4GetValue(void);

Status_t Tim7Init(void);
Status_t Tim7Start(void);

Status_t Tim15Init(void);
Status_t Tim15Start(void);

#endif
