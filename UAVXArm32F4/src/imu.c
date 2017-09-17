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

//    UAVX is distributed in the hope that it will be useful,but WITHOUT ANY WARRANTY; without
//    even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  
//    See the GNU General Public License for more details.

//    You should have received a copy of the GNU General Public License along with this program.  
//    If not, see http://www.gnu.org/licenses/

// Gyros

#include "UAVX.h"

const char * GyroName[] = { "MLX90609", "ADXRS613/150", "ST-AY530",
		"ADXRS610/300", "UAVXArm32IMU", "FreeIMU", "GyroUnknown" };

void ShowGyroType(uint8 s, uint8 G) {
	TxString(s, GyroName[G]);
} // ShowGyroType

// ITG3200  1.214142088
const real32 GyroScale[] = { //
		13.0834, // MLX90609
				6.98131, // ADXRS613/150
				15.8666, // ST-AY530 0.0041
				17.4532, // ADXRS610/300
				0.001064225154f, // UAVXArm32IMU
				0.001064225154f, // FreeIMU
				0.001064225154f };

//real32 AccScale[3] = { DEF_ACC_SCALE, DEF_ACC_SCALE, DEF_ACC_SCALE };

uint8 CurrAttSensorType = UAVXArm32IMU;

HistStruct AccF[3];
HistStruct GyroF[3];
boolean UsingSWFilters;

real32 GyroBias[3];
real32 Acc[3], Rate[3];
real32 RateEnergySum;
uint32 RateEnergySamples;

real32 GyroLPFreqHz, AccLPFreqHz;
idx RollPitchLPFOrder;

// NED
// P,R,Y
// BF, LR, UD

void GetIMU(void) {
	idx a;

	ReadAccAndGyro(true);

	if (UsingSWFilters)
		for (a = X; a <= Z; a++) {
			// TODO: perhaps add slewlimiter?
			RawGyro[a] = LPFilter(&GyroF[a], RollPitchLPFOrder, RawGyro[a],
					GyroLPFreqHz, CurrPIDCycleS);
		}

	UpdateGyroTempComp();

	Rate[Pitch] = (RawGyro[X] - GyroBias[X]) * GyroScale[CurrAttSensorType];
	Rate[Roll] = (RawGyro[Y] - GyroBias[Y]) * GyroScale[CurrAttSensorType];
	Rate[Yaw] = -(RawGyro[Z] - GyroBias[Z]) * GyroScale[CurrAttSensorType];

	if (NewAccUpdate) {
		NewAccUpdate = false;

		if (UsingSWFilters)
			for (a = X; a <= Z; a++)
				RawAcc[a] = LPFilter(&AccF[a], RollPitchLPFOrder, RawAcc[a],
						AccLPFreqHz, CurrPIDCycleS);

		if (CurrAttSensorType == InfraRedAngle) {

			// TODO: bias and scale from where? track max min with decay???

		} else {
			Acc[BF] = (RawAcc[Y] - NV.AccCal.Bias[Y]) * NV.AccCal.Scale[Y];
			Acc[LR] = (RawAcc[X] - NV.AccCal.Bias[X]) * NV.AccCal.Scale[X];
			Acc[UD] = -(RawAcc[Z] - NV.AccCal.Bias[Z]) * NV.AccCal.Scale[Z];
		}
	}

	F.IMUFailure = !F.IMUCalibrated;

} // GetIMU

void ErectGyros(int32 TS) {
	const int32 IntervalmS = 2;
	idx g;
	int32 i;
	real32 MaxRawGyro[3], MinRawGyro[3], Av[3];
	int32 s = TS * 1000 / IntervalmS;
	boolean Moving = false;

	LEDOn(LEDRedSel);

	GetIMU();

	for (g = X; g <= Z; g++)
		MaxRawGyro[g] = MinRawGyro[g] = Av[g] = RawGyro[g];

	for (i = 1; i < s; i++) {
		Delay1mS(IntervalmS);

		GetIMU();

		for (g = X; g <= Z; g++) {

			Av[g] += RawGyro[g];

			if (RawGyro[g] > MaxRawGyro[g])
				MaxRawGyro[g] = RawGyro[g];
			else if (RawGyro[g] < MinRawGyro[g])
				MinRawGyro[g] = RawGyro[g];
		}
	}

	for (g = X; g <= Z; g++) {
		Av[g] /= (real32) s;
		MaxRawGyro[g] -= Av[g];
		MinRawGyro[g] -= Av[g];
		Moving |= Max(Abs(MaxRawGyro[g]), Abs(MinRawGyro[g]))
				> GYRO_MAX_SHAKE_RAW;
	}

	if (Moving) {
		SaveLEDs();
		LEDsOff();
		for (i = 0; i < 4; i++) {
			LEDToggle(LEDYellowSel);
			DoBeep(8, 2);
		}
		RestoreLEDs();
	} else {
		if (F.UsingAnalogGyros) {
			for (g = X; g <= Z; g++)
				GyroBias[g] = Av[g];
		} else { // leave MPU6xxx calibration alone
			NV.GyroCal.TRef = MPU6XXXTemperature;
			for (g = X; g <= Z; g++)
				NV.GyroCal.C[g] = Av[g];
		}
	}

	LEDOff(LEDRedSel);

} // ErectGyros

void InitSWFilters(void) {
	idx a;

	for (a = X; a <= Z; a++)
		AccF[a].Primed = GyroF[a].Primed = false;
} // InitSWFilters


void InitIMU(void) {
	idx a;
	boolean r;

	InitMPU6XXX();

	if (F.UsingAnalogGyros) {
		ReadAccAndGyro(true);
		for (a = X; a <= Z; a++)
			GyroBias[a] = RawGyro[a]; //  until erect gyros
	} else if (CurrAttSensorType == InfraRedAngle) {

		// TODO: have to put it through the roll/pitch combos to capture
		//current max/min assume bias is mid point.

	}

	InitSWFilters();

	GetIMU();

	r = true;
	for (a = X; a <= Z; a++)
		r &= NV.AccCal.Bias[a] == 0.0f;

	F.AccCalibrated = F.IMUCalibrated = !r;

} // InitIMU


