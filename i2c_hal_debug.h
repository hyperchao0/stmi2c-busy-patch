
#pragma once

/* Usage:
 * 0. Check the TODOs and change the code according to your MCU
 * 1. Include this file in your i2c.c file or other file where you have HAL_I2C_MspInit function
 * 2. Put the HAL_I2C_Pins_Clear function call in the HAL_I2C_MspInit function before the I2C pins are initialized
 * 3. Put the HAL_I2C_Debug function call before the I2C communication to fix the I2C bus, and don't forget to include this file
 */

#include "stm32f4xx_hal.h"  // TODO: Change this to your MCU

// TODO: This function should be called before I2C communication to fix the I2C bus if it is stuck
inline void HAL_I2C_Debug(I2C_HandleTypeDef *i2cHandle) {
    while( i2cHandle->Instance->SR2 & I2C_FLAG_BUSY ) {
        HAL_I2C_MspDeInit(i2cHandle);
        HAL_I2C_MspInit(i2cHandle);
    }
}

// TODO: This function should be called in the HAL_I2C_MspInit function to clear the I2C bus
inline void HAL_I2C_Pins_Clear(I2C_HandleTypeDef *i2cHandle) {
    // I2C SDA and SCL pins should be set as output and set to high
    GPIO_InitTypeDef I2C_Pins_InitStruct = {
        .Mode = GPIO_MODE_OUTPUT_PP,
        .Pull = GPIO_PULLUP,
        .Speed = GPIO_SPEED_FREQ_HIGH,
    }
    I2C_Pins_InitStruct.Pin = I2C_SDA_PIN;              // TODO: Change this to your I2C SDA pin
    HAL_GPIO_Init(I2C_SDA_PORT, &I2C_Pins_InitStruct);  // TODO: Change this to your I2C SDA port
    HAL_GPIO_WritePin(I2C_SDA_PORT, I2C_SDA_PIN, GPIO_PIN_SET);
    I2C_Pins_InitStruct.Pin = I2C_SCL_PIN;              // TODO: Change this to your I2C SCL pin
    HAL_GPIO_Init(I2C_SCL_PORT, &I2C_Pins_InitStruct);  // TODO: Change this to your I2C SCL port
    HAL_GPIO_WritePin(I2C_SCL_PORT, I2C_SCL_PIN, GPIO_PIN_SET);

    // I2C peripheral should be reset.
    i2cHandle->Instance->CR1 &= ~I2C_CR1_PE;
    if(!(i2cHandle->Instance->CR1 & I2C_CR1_PE)) i2cHandle->Instance->CR1 |= I2C_CR1_PE;
}

