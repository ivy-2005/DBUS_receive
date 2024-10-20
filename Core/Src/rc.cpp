//
// Created by 包辰宸 on 24-10-20.
//

#include "rc.h"
#include "gpio.h"
#include "main.h"
#include "usart.h"

typedef enum {
    up=1,
    down,
    mid,
  }RC_state;

typedef enum {
    release,
    press,
  } Mouse_state;

typedef struct __packed {
    struct {
        uint16_t ch0;
        uint16_t ch1;
        uint16_t ch2;
        uint16_t ch3;
        RC_state s1;
        RC_state s2;
    }rc;

    struct {
        int16_t x;
        int16_t y;
        int16_t z;
        uint8_t press_l;
        uint8_t press_r;
    }mouse;

    struct{
        uint16_t v;
    }key;

}RC_Ctl_t;

extern uint8_t buffer[18];
extern uint8_t data[18];
RC_Ctl_t RC_CtrlData;

float linearMapping(float in, float in_min, float in_max, float out_min, float out_max){
    float out;
    out = (float)(in - in_min) / (float)(in_max - in_min) * (out_max - out_min) + out_min;
    return out;
}

void RemoteDataProcess(const uint8_t pData[18]){
    if(pData == NULL){
        return;
    }
    uint16_t ch0_temp=((int16_t)pData[0] | ((int16_t)pData[1] << 8)) & 0x07FF;
    RC_CtrlData.rc.ch0 = linearMapping(ch0_temp, 364,1684,-1,1);
    uint16_t ch1_temp=(((int16_t)pData[1] >> 3) | ((int16_t)pData[2] << 5)) & 0x07FF;
    RC_CtrlData.rc.ch1 = linearMapping(ch1_temp, 364,1684,-1,1);
    uint16_t ch2_temp=(((int16_t)pData[2] >> 6) | ((int16_t)pData[3] << 2) |((int16_t)pData[4] << 10)) & 0x07FF;
    RC_CtrlData.rc.ch2 = linearMapping(ch2_temp, 364,1684,-1,1);
    uint16_t ch3_temp=(((int16_t)pData[4] >> 1) | ((int16_t)pData[5]<<7)) & 0x07FF;
    RC_CtrlData.rc.ch3 = linearMapping(ch3_temp, 364,1684,-1,1);

    RC_CtrlData.rc.s1 = (RC_state)(((pData[5]>>4) & 0x000C) >> 2);
    RC_CtrlData.rc.s2 = (RC_state)((pData[5] >> 4) & 0x0003);

    int16_t x_temp=((int16_t)pData[6]) | ((int16_t)pData[7] << 8);
    RC_CtrlData.mouse.x = linearMapping(x_temp, -32768,32768,-1,1);
    int16_t y_temp=((int16_t)pData[8]) | ((int16_t)pData[9] << 8);
    RC_CtrlData.mouse.y = linearMapping(y_temp, -32768,32768,-1,1);
    int16_t z_temp=((int16_t)pData[10]) | ((int16_t)pData[11] << 8);
    RC_CtrlData.mouse.z = linearMapping(z_temp, -32768,32768,-1,1);

    RC_CtrlData.mouse.press_l = pData[12];
    RC_CtrlData.mouse.press_r = pData[13];
    RC_CtrlData.key.v = ((int16_t)pData[14]);// | ((int16_t)pData[15] << 8);

}