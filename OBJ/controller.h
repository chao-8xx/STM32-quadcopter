#ifndef _controller_h_
#define _controller_h_

#include "stm32f10x.h"

typedef struct 
{
    uint16_t out;

}_OUT_Motor;

typedef struct
{
    uint8_t mode : 4;
    uint8_t high_mode :4;
}_CONTROLLER_MODE;

typedef struct
{
    uint8_t high;
    uint8_t longitude;
    uint8_t latitude;
}_CONTROLLER_CNT;

typedef struct
{
    uint16_t mark_high;
    uint16_t FINAL_OUT;
}Throttle;


void AngleController(void);
void GyroController(void);
void ControllerOutput(void);
void high_controller(void);
void fix_controller(void);
void ControModel(void);
void _controller_perform(void);
    
extern uint8_t high_mark_flag;
extern uint8_t fix_mark_flag;
extern uint8_t flow_fix_flag;

#endif

