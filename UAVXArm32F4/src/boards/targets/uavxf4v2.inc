// ===============================================================================================
// =                                UAVX Quadrocopter Controller                                 =
// =                           Copyright (c) 2008 by Prof. Greg Egan                             =
// =                 Original V3.15 Copyright (c) 2007 Ing. Wolfgang Mahringer                   =
// =                     http://code.google.com/p/uavp-mods/ http://uavp.ch                      =
// ===============================================================================================

//    This is part of UAVX.

//    UAVX is free software: you can redistribute it and/or modify it under the terms of the GNU
//    General Public License as published by the Free Software Foundation, either version 3 of the
//    License, or (at your option) any later version.

//    UAVX is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
//    even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//    See the GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along with this program.
//    If not, see http://www.gnu.org/licenses/

#ifndef _pinsf4v2_h
#define _pinsf4v2_h

#include "../UAVX.h"

#define MAX_SPI_DEVICES 0

PinDef RCPins[MAX_RC_INPUTS] = { { true, GPIOA, GPIO_Pin_0, GPIO_PinSource0,
		GPIO_Mode_IPU, 0, 0, true, { TIM2, TIM_Channel_1, TIM_IT_CC1, 0, 0 },
		TIM2_IRQn, 0, },//
		{ true, GPIOA, GPIO_Pin_1, GPIO_PinSource1, GPIO_Mode_IPU, 0, 0, true,
				{ TIM2, TIM_Channel_2, TIM_IT_CC2, 0, 0 }, TIM2_IRQn, 0, },//
		{ true, GPIOA, GPIO_Pin_2, GPIO_PinSource2, GPIO_Mode_IPU, 0, 0, true,
				{ TIM2, TIM_Channel_3, TIM_IT_CC3, 0, 0 }, TIM2_IRQn, 0, },//
		{ true, GPIOA, GPIO_Pin_3, GPIO_PinSource3, GPIO_Mode_IPU, 0, 0, true,
				{ TIM2, TIM_Channel_4, TIM_IT_CC4, 0, 0 }, TIM2_IRQn, 0, },//
		{ true, GPIOA, GPIO_Pin_6, GPIO_PinSource6, GPIO_Mode_IPU, 0, 0, true,
				{ TIM3, TIM_Channel_1, TIM_IT_CC1, 0, 0 }, TIM3_IRQn, 0, },//
		{ true, GPIOA, GPIO_Pin_7, GPIO_PinSource7, GPIO_Mode_IPU, 0, 0, true,
				{ TIM3, TIM_Channel_2, TIM_IT_CC2, 0, 0 }, TIM3_IRQn, 0, },//
		{ true, GPIOB, GPIO_Pin_0, GPIO_PinSource0, GPIO_Mode_IPU, 0, 0, true,
				{ TIM3, TIM_Channel_3, TIM_IT_CC3, 0, 0 }, TIM3_IRQn, 0, },//
		{ true, GPIOB, GPIO_Pin_1, GPIO_PinSource1, GPIO_Mode_IPU, 0, 0, true,
				{ TIM3, TIM_Channel_4, TIM_IT_CC4, 0, 0 }, TIM3_IRQn, 0, }//
};

AnalogPinDef AnalogPins[ANALOG_CHANNELS] = { { true, ADC1, GPIOC, GPIO_Pin_0,
		ADC_Channel_10, 0, DMA1_Channel1, 1 }, // RF
		{ true, ADC1, GPIOC, GPIO_Pin_1, ADC_Channel_11, 0, DMA1_Channel1, 2 }, // Amps
		{ true, ADC1, GPIOC, GPIO_Pin_2, ADC_Channel_12, 0, DMA1_Channel1, 3 }, // Volts
		//TODO: clip analog channels if digitial gyro and/or only super sample gyros
		{ true, ADC1, GPIOC, GPIO_Pin_3, ADC_Channel_13, 0, DMA1_Channel1, 4 }, // Roll
		{ true, ADC1, GPIOC, GPIO_Pin_4, ADC_Channel_14, 0, DMA1_Channel1, 5 }, // Pitch
		{ true, ADC1, GPIOC, GPIO_Pin_5, ADC_Channel_15, 0, DMA1_Channel1, 6 }, // Yaw
		};

PinDef PWMPins[MAX_PWM_OUTPUTS] = {
// Drives 1-4
		{ true, GPIOB, GPIO_Pin_9, GPIO_PinSource9, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM4, TIM_Channel_4, 0, &(TIM4->CCR4), 0 } },//
		{ true, GPIOB, GPIO_Pin_8, GPIO_PinSource8, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM4, TIM_Channel_3, 0, &(TIM4->CCR3), 0 } },//
		{ true, GPIOB, GPIO_Pin_7, GPIO_PinSource7, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM4, TIM_Channel_2, 0, &(TIM4->CCR2), 0 } },//
		{ true, GPIOB, GPIO_Pin_6, GPIO_PinSource6, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM4, TIM_Channel_1, 0, &(TIM4->CCR1), 0 } },//
		{ true, GPIOA, GPIO_Pin_11, GPIO_PinSource11, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM1, TIM_Channel_4, 0, &(TIM1->CCR4), 0 } },//
		{ true, GPIOA, GPIO_Pin_8, GPIO_PinSource8, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM1, TIM_Channel_1, 0, &(TIM1->CCR1), 0 } },//
		// Drives 4-8 for brushless
		{ true, GPIOA, GPIO_Pin_6, GPIO_PinSource6, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM3, TIM_Channel_1, 0, &(TIM3->CCR1), 0 } },//
		{ true, GPIOA, GPIO_Pin_7, GPIO_PinSource7, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM3, TIM_Channel_2, 0, &(TIM3->CCR2), 0 } },//
		{ true, GPIOB, GPIO_Pin_0, GPIO_PinSource0, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM3, TIM_Channel_3, 0, &(TIM3->CCR3), 0 } },//
		{ true, GPIOB, GPIO_Pin_1, GPIO_PinSource1, GPIO_Mode_AF_PP, 0, 0,
				true, { TIM3, TIM_Channel_4, 0, &(TIM3->CCR4), 0 } } //

};

const PinDef GPIOPins[MAX_GPIO_PINS] = { { GPIOA, GPIO_Pin_12, GPIO_PinSource12,
		GPIO_Mode_Out_PP, 0, 0, false, { 0, }, 0, }, // Beeper
		{ GPIOC, GPIO_Pin_10, GPIO_PinSource10, GPIO_Mode_IPU, 0, 0, false,
				{ 0, }, 0, }, // Armed
		{ GPIOC, GPIO_Pin_9, GPIO_PinSource9, GPIO_Mode_IPU, 0, 0, false,
				{ 0, }, 0, }, // Landing

		{ GPIOC, GPIO_Pin_6, GPIO_PinSource6, GPIO_Mode_IPU, 0, 0, false,
				{ 0, }, 0, }, // Aux1
		{ GPIOC, GPIO_Pin_7, GPIO_PinSource7, GPIO_Mode_IPU, 0, 0, false,
				{ 0, }, 0, }, // Aux2
		{ GPIOC, GPIO_Pin_8, GPIO_PinSource8, GPIO_Mode_Out_PP, 0, 0, false, {
				0, }, 0, }, // Aux3 / Probe

		//   { GPIOC, GPIO_Pin_13, GPIO_PinSource13, GPIO_Mode_Out_PP, 0, 0,
		//       false, { 0, }, 0, }, // Baro Enable
		};

const PinDef LEDPins[MAX_LEDS] = { { GPIOB, GPIO_Pin_3, GPIO_PinSource3,
		GPIO_Mode_Out_OD, 0, 0, false, { 0, }, 0, }, // LED0
		{ GPIOB, GPIO_Pin_4, GPIO_PinSource4, GPIO_Mode_Out_OD, 0, 0, false, {
				0, }, 0, }, // LED1
		{ GPIOC, GPIO_Pin_11, GPIO_PinSource11, GPIO_Mode_Out_OD, 0, 0, false,
				{ 0, }, 0, }, // LED2
		{ GPIOC, GPIO_Pin_12, GPIO_PinSource12, GPIO_Mode_Out_OD, 0, 0, false,
				{ 0, }, 0, }, // LED3
		};

const PinDef SPISelectPins[MAX_SPI_DEVICES] = { };

uint8 GPSRxSerial, GPSTxSerial, RCSerial, TelemetrySerial;
boolean RxUsingSerial;

// imuSel, baroSel, magSel, memSel, gpsSel, rfSel, escSel, flowSel, assel
const uint8 i2cMap[] = { 2, 2, 2, 2, 2, 2, 2, 2, 2 }; // I2C2

void InitTarget(void) {
	
	currIMUType = mpu6050IMU;
	currBaroType = ms5611Baro;
	currMagType = hmc5xxxMag;
	currGimbalType = noGimbal;
	currBBMem = i2cEEPROMMem;

} // InitTarget


#endif
