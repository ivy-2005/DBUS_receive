//
// Created by 包辰宸 on 24-10-20.
//

#include "callback.h"
#include "main.h"
#include "usart.h"

extern uint8_t buffer[18];
extern uint8_t data[18];
extern struct RC_Ctl_t RC_CtrlData;
void RemoteDataProcess(const uint8_t data[18]);

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    if(huart == &huart1) {
        HAL_UARTEx_ReceiveToIdle_DMA(&huart1, buffer, sizeof(buffer));
        for(uint8_t i = 0; i<Size; i++) {
            data[i] = buffer[i];
        }
    }
    RemoteDataProcess(data);
}