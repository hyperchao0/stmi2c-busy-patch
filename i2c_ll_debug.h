
#pragma once

/* Usage:
 * 0. Check the TODOs and change the code according to your MCU
 * 1. Include this file in your project
 * 2. Put the LL_I2C_Debug function call before the I2C communication to fix the I2C bus, and don'
 */

// TODO: Change this to your MCU series
#include "stm32g4xx_ll_i2c.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_utils.h"

// TODO: Change this to your I2C peripheral
#define I2C_PERIPH I2C1
// TODO: Change this to your I2C SDA and SCL pins
#define I2C_SDA_PORT GPIOB
#define I2C_SDA_PIN LL_GPIO_PIN_7
#define I2C_SCL_PORT GPIOB
#define I2C_SCL_PIN LL_GPIO_PIN_6
// TODO: Change this to your I2C peripheral
#define I2C_PERIPH_DISABLE LL_APB1_GRP1_DisableClock(LL_APB1_GRP1_PERIPH_I2C1)
#define I2C_PERIPH_RESET LL_APB1_GRP1_ForceReset(LL_APB1_GRP1_PERIPH_I2C1)
#define I2C_PERIPH_RELEASE LL_APB1_GRP1_ReleaseReset(LL_APB1_GRP1_PERIPH_I2C1)
#define I2C_PERIPH_ENABLE LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C1)

// TODO: This function should be called before I2C communication to fix the I2C bus if it is stuck
inline void LL_I2C_Debug() {
    // Check if the I2C is busy 3 times
    uint8_t busy_count = 0;
    while (LL_I2C_IsActiveFlag_BUSY(I2C_PERIPH)) {
        busy_count++;
        // TODO: If there are some fixed I2C registers, you can replace the values with the fixed values
        uint32_t i2c_cr1 = LL_I2C_ReadReg(I2C_PERIPH, CR1); 
        uint32_t i2c_cr2 = LL_I2C_ReadReg(I2C_PERIPH, CR2);
        // Deactivate the I2C peripheral
        I2C_PERIPH_DISABLE;
        I2C_PERIPH_RESET;
        // Set the I2C pins as GPIO
        LL_GPIO_SetPinMode(I2C_SDA_PORT, I2C_SDA_PIN, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinMode(I2C_SCL_PORT, I2C_SCL_PIN, LL_GPIO_MODE_OUTPUT);
        LL_GPIO_SetPinOutputType(I2C_SDA_PORT, I2C_SDA_PIN, LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinOutputType(I2C_SCL_PORT, I2C_SCL_PIN, LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetOutputPin(I2C_SDA_PORT, I2C_SDA_PIN);
        LL_GPIO_SetOutputPin(I2C_SCL_PORT, I2C_SCL_PIN);
        // TODO: Add delay if needed
        // LL_mDelay(1);
        LL_GPIO_SetPinMode(I2C_SDA_PORT, I2C_SDA_PIN, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetPinMode(I2C_SCL_PORT, I2C_SCL_PIN, LL_GPIO_MODE_ALTERNATE);
        LL_GPIO_SetPinOutputType(I2C_SDA_PORT, I2C_SDA_PIN, LL_GPIO_OUTPUT_PUSHPULL);
        LL_GPIO_SetPinOutputType(I2C_SCL_PORT, I2C_SCL_PIN, LL_GPIO_OUTPUT_PUSHPULL);
        // Release the reset
        I2C_PERIPH_RELEASE;
        I2C_PERIPH_ENABLE;
        // Restore the I2C registers
        LL_I2C_WriteReg(I2C_PERIPH, CR1, i2c_cr1);
        LL_I2C_WriteReg(I2C_PERIPH, CR2, i2c_cr2);

        if (busy_count > 3) break;
    }
}

