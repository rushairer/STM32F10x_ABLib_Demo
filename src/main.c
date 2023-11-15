#include <string.h>
#include "stm32f10x.h"
#include "stm32f10x_abl_delay.h"
#include "stm32f10x_abl_led.h"
#include "stm32f10x_abl_key.h"
#include "stm32f10x_abl_oled.h"
#include "stm32f10x_abl_servo.h"
#include "stm32f10x_abl_pca9685.h"
#include "stm32f10x_abl_key4x4.h"
#include "stm32f10x_abl_serial.h"
#include "stm32f10x_abl_nrf24l01.h"
#include "stm32f10x_abl_spi_soft.h"
#include "stm32f10x_abl_iap.h"

SERIAL_InitTypeDef serial;

/* Here control sending:1 or receiving:0 mode */
uint8_t WifiMode = 0;

int main()
{

    KEY_InitTypeDef key1;
    KEY_Init(&key1, RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_12);
    // LED_InitTypeDef led1;
    // Led1_Init(&led1);

    NRF24L01_InitTypeDef wifi;
    SPI_Soft_InitTypeDef spi;
    SPI_Soft_Init(&spi, RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_4, GPIO_Pin_5, GPIO_Pin_6, GPIO_Pin_7);
    Nrf24l011_Init(&wifi, &spi);

    IAP_InitTypeDef IAPx;
    IAP_Init(
        &IAPx,
        0x8002000,
        &wifi,
        (pIAPReceiveDataFunction)NRF24L01_ReceiveData,
        (pIAPOutputStringFunction)NRF24L01_SendString);

    if (WifiMode == 1) {
        NRF24L01_TxMode(&wifi);
    } else {
        NRF24L01_RxMode(&wifi);
    }

    // KEY4X4_InitTypeDef keyboard;
    // KEY4X4_Init(
    //     &keyboard,
    //     RCC_APB2Periph_GPIOB,
    //     GPIOB,
    //     GPIO_Pin_12,
    //     GPIO_Pin_13,
    //     GPIO_Pin_14,
    //     GPIO_Pin_15,
    //     RCC_APB2Periph_GPIOA,
    //     GPIOA,
    //     GPIO_Pin_7,
    //     GPIO_Pin_6,
    //     GPIO_Pin_5,
    //     GPIO_Pin_4);

    OLED_InitTypeDef oled1;

    // Soft
    // I2C_Soft_InitTypeDef i2cSoft;
    // I2C_Soft_Init(&i2cSoft, RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_6, GPIO_Pin_7);
    // OLED_Soft_Init(&oled1, &i2cSoft);

    // Device
    Oled1_Init(&oled1);

    // SERVO_TypeDef servo1;
    // SERVO_Init(&servo1, RCC_APB1Periph_TIM3, TIM3, 1, RCC_APB2Periph_GPIOA, GPIOA, GPIO_Pin_6);
    // SERVO_SetAngle(&servo1, servoAngle);

    // PCA9685_InitTypeDef pca9685;
    // PCA9685_Init(&pca9685, RCC_APB1Periph_I2C2, I2C2, RCC_APB2Periph_GPIOB, GPIOB, GPIO_Pin_10, GPIO_Pin_11);

    // while (NRF24L01_Check(&wifi) != 0) {
    //     OLED_ShowString(&oled1, 0, 40, "nRF24L01 check failed\r\n", OLED_FONT_SIZE_12, OLED_COLOR_NORMAL);
    //     OLED_RefreshScreen(&oled1);

    //     Delay_ms(2000);
    // }

    OLED_ShowString(&oled1, 0, 0, "nRF24L01 is ready", OLED_FONT_SIZE_12, OLED_COLOR_NORMAL);
    OLED_RefreshScreen(&oled1);

    // Serial1_Init(&serial);

    // float servoAngle[10] = {0};
    uint8_t Buf[32] = {0};
    // uint8_t Buf[32] = {5, 0x61, 0x62, 0x65, 0x6e, 0x61};
    uint8_t LastChar = 0;
    while (1) {
        // NRF24L01_TxMode(&wifi);
        // Delay_ms(100);
        // IAP_Menu(&IAPx);
        // NRF24L01_RxMode(&wifi);
        // Delay_ms(300);

        if (WifiMode == 1) {
            NRF24L01_TxMode(&wifi);
            Delay_ms(100);

            Buf[0]  = 31;
            Buf[1]  = 0x61;
            Buf[2]  = 0x62;
            Buf[3]  = 0x65;
            Buf[4]  = 0x6E;
            Buf[5]  = LastChar++;
            Buf[6]  = 0x0D; // \r
            Buf[7]  = 0x0A; // \n
            Buf[8]  = 0x62;
            Buf[9]  = 0x0D; // \r
            Buf[10] = 0x0A; // \n
            Buf[11] = 0x62;
            Buf[12] = 0x62;
            Buf[13] = 0x62;
            Buf[14] = 0x62;
            Buf[18] = 0x62;
            Buf[19] = 0x62;
            Buf[20] = 0x62;
            Buf[22] = 0x62;
            Buf[23] = 0x62;
            Buf[24] = 0x62;
            Buf[25] = 0x62;
            Buf[26] = 0x62;
            Buf[27] = 0x62;
            Buf[28] = 0x65;
            Buf[29] = 0x64;
            Buf[30] = 0x63;
            Buf[31] = 0x62;
            NRF24L01_SendTxBuf(&wifi, Buf);
            //  NRF24L01_SendString(&wifi, (char *)Buf);
        } else {
            // NRF24L01_RxMode(&wifi);
            if (!NRF24L01_Get_Value_Flag(&wifi)) {
                NRF24L01_GetStringWithoutSuffix(&wifi, (char *)Buf);
                // NRF24L01_GetRxBuf(&wifi, Buf);
                OLED_ShowString(&oled1, 0, 40, "                                                  ", OLED_FONT_SIZE_12, OLED_COLOR_NORMAL);
                OLED_ShowString(&oled1, 0, 40, (char *)Buf, OLED_FONT_SIZE_12, OLED_COLOR_NORMAL);
                OLED_RefreshScreen(&oled1);

                NRF24L01_TxMode(&wifi);
                Delay_ms(300);
                IAP_ShowMenu(&IAPx);
                IAP_Download(&IAPx);
                Delay_ms(900);
                IAP_Execute(&IAPx);
                NRF24L01_RxMode(&wifi);
                Delay_ms(300);
            }
        }

        // uint16_t keyValue = KEY4X4_GetKey(&keyboard);
        // OLED_ShowBinNumber(&oled1, 0, 0, keyValue, 16, OLED_FONT_SIZE_12, OLED_COLOR_NORMAL);

        // if (KEY_IsPressed(&key1) == 1) {
        //     LED_Toggle(&led1);

        //     if (LED_IsOn(&led1) == 1) {
        //         OLED_CleanBuffer(&oled1);
        //         // OLED_SetPixel(&oled1, 64, 20, OLED_COLOR_NORMAL);
        //         // OLED_DrawLine(&oled1, 0, 0, 127, 63, OLED_COLOR_NORMAL);
        //         // OLED_ShowChar(&oled1, 3, 48, 'A', 12, OLED_COLOR_NORMAL);
        //         // OLED_ShowChar(&oled1, 30, 48, 'A', 16, OlED_COLOR_REVERSED);

        //         // OLED_ShowString(&oled1, 3, 18, "Abendas", 12, OLED_COLOR_NORMAL);
        //         // OLED_ShowString(&oled1, 60, 38, "Abendas", 16, OlED_COLOR_REVERSED);

        //         // OLED_ShowNumber(&oled1, 70, 1, 1234567, 7, 12, OLED_COLOR_NORMAL);

        //         OLED_DrawImage(&oled1, 0, 0, 127, 63, OLED_BMP1);
        //         OLED_RefreshScreen(&oled1);
        //     } else {
        //         OLED_CleanBuffer(&oled1);
        //         OLED_RefreshScreen(&oled1);
        //     }

        //     servoAngle += 30;
        //     if (servoAngle > 180) {
        //         servoAngle = 0;
        //     }

        //     // SERVO_SetAngle(&servo1, servoAngle);
        //     PCA9685_SetAngle(&pca9685, ch, servoAngle);
        //     ch++;
        //     if (ch > 9) {
        //         ch = 0;
        //     }
        // }

        // for (uint16_t i = 0; i < 10; i++) {
        //     if ((keyValue & (1 << i)) == (1 << i)) {
        //         servoAngle[i] += 30;
        //         if (servoAngle[i] > 180) {
        //             servoAngle[i] = 0;
        //         }

        //         PCA9685_SetAngle(&pca9685, i, servoAngle[i]);
        //     }
        // }

        // if (((keyValue & (1 << 14)) == (1 << 14))) {
        //     for (uint16_t i = 0; i < 10; i++) {
        //         PCA9685_SetAngle(&pca9685, i, 0);
        //         servoAngle[i] = 0;
        //         Delay_ms(10);
        //     }
        // } else if (((keyValue & (1 << 15)) == (1 << 15))) {
        //     for (uint16_t i = 0; i < 10; i++) {
        //         PCA9685_SetAngle(&pca9685, i, 180);
        //         Delay_ms(10);
        //         servoAngle[i] = 180;
        //     }
        // }

        // if (SERIAL_GetRxFlag(&serial) == 1) {

        //     OLED_ShowString(&oled1, 1, 20, "                      ", 12, OLED_COLOR_NORMAL);

        //     SERIAL_SendString(&serial, (char *)serial.RxData);
        //     SERIAL_Printf(&serial, "\r\n");

        //     OLED_ShowString(&oled1, 1, 20, (char *)serial.RxData, 12, OLED_COLOR_NORMAL);
        // }

        // OLED_RefreshScreen(&oled1);
        Delay_ms(300);
    }
}

void USART1_IRQHandler(void)
{
    SERIAL_IRQHandler(&serial);
}