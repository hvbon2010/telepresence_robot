#include "include.h"

extern int32_t Position;
PIDType PIDVelocity_motor_0, PIDPosition_motor_0, PIDVelocity_motor_1, PIDPosition_motor_1;
int32_t RisePulse, FallPulse;

void PIDSpeedSet_motor_0(int32_t SpeedSet)
{
	PIDVelocity_motor_0.Enable = 1;
	PIDVelocity_motor_0.SetPoint = SpeedSet;
}

void PIDPositionSet_motor_0(int32_t SetPoint)
{
	PIDPosition_motor_0.Enable = 1;
	PIDPosition_motor_0.iPart = 0;
	PIDPosition_motor_0.PIDErrorTemp1 = 0;
	PIDPosition_motor_0.SetPoint += SetPoint;
	PIDPosition_motor_0.iPart = 0;
//	ROM_QEIPositionSet(QEI0_BASE, 0);
//	Position += SetPoint;
}

void PIDPosCalc_motor_0(int32_t Position, int32_t MaxResponse)
{
	PIDPosition_motor_0.PIDError = PIDPosition_motor_0.SetPoint - Position;
	PIDPosition_motor_0.pPart = PIDPosition_motor_0.Kp * PIDPosition_motor_0.PIDError;
	PIDPosition_motor_0.iPart += PIDPosition_motor_0.Ki * PIDPosition_motor_0.PIDError;
	PIDPosition_motor_0.dPart = PIDPosition_motor_0.Kd * (PIDPosition_motor_0.PIDError - PIDPosition_motor_0.PIDErrorTemp1);

/*	Uncomment to enable iPart-limit
	if (PIDPosition_motor_0.iPart > 15)
		PIDPosition_motor_0.iPart = 15;
	else if (PIDPosition_motor_0.iPart < -15)
		PIDPosition_motor_0.iPart = -15; */

	PIDPosition_motor_0.PIDResult = PIDPosition_motor_0.pPart + PIDPosition_motor_0.iPart + PIDPosition_motor_0.dPart;
	if (PIDPosition_motor_0.PIDResult > MaxResponse)
		PIDPosition_motor_0.PIDResult = (float)(MaxResponse);
	if (PIDPosition_motor_0.PIDResult < -1 * MaxResponse)
		PIDPosition_motor_0.PIDResult = (float)(-1 * MaxResponse);
	PIDPosition_motor_0.PIDErrorTemp1 = PIDPosition_motor_0.PIDError;
}

//MaxResponse: Max Duty Cycle
void PIDVerCalc_motor_0(int32_t Speed, int32_t MaxResponse)
{
	PIDVelocity_motor_0.PIDError = PIDVelocity_motor_0.SetPoint - Speed;
	PIDVelocity_motor_0.pPart = PIDVelocity_motor_0.Kp * PIDVelocity_motor_0.PIDError;
	PIDVelocity_motor_0.iPart += PIDVelocity_motor_0.Ki * PIDVelocity_motor_0.PIDError;
	PIDVelocity_motor_0.dPart = PIDVelocity_motor_0.Kd * (PIDVelocity_motor_0.PIDError - PIDVelocity_motor_0.PIDErrorTemp1);
/*
	if (PIDVelocity.iPart > 40)
		PIDVelocity.iPart = 40;
	else if (PIDVelocity.iPart < -40)
		PIDVelocity.iPart = -40;
*/
	PIDVelocity_motor_0.PIDResult += (PIDVelocity_motor_0.pPart + PIDVelocity_motor_0.iPart + PIDVelocity_motor_0.dPart);
	if (PIDVelocity_motor_0.PIDResult > MaxResponse)
		PIDVelocity_motor_0.PIDResult = (float)MaxResponse;
	if (PIDVelocity_motor_0.PIDResult < -1 * MaxResponse)
		PIDVelocity_motor_0.PIDResult = (float)(-1 * (MaxResponse));
	PIDVelocity_motor_0.PIDErrorTemp1 = PIDVelocity_motor_0.PIDError;
}


void PIDSpeedSet_motor_1(int32_t SpeedSet)
{
	PIDVelocity_motor_1.Enable = 1;
	PIDVelocity_motor_1.SetPoint = SpeedSet;
}

void PIDPositionSet_motor_1(int32_t SetPoint)
{
	PIDPosition_motor_1.Enable = 1;
	PIDPosition_motor_1.iPart = 0;
	PIDPosition_motor_1.PIDErrorTemp1 = 0;
	PIDPosition_motor_1.SetPoint += SetPoint;
//	ROM_QEIPositionSet(QEI0_BASE, 0);
//	Position += SetPoint;
}

void PIDPosCalc_motor_1(int32_t Position, int32_t MaxResponse)
{
	PIDPosition_motor_1.PIDError = PIDPosition_motor_1.SetPoint - Position;
	PIDPosition_motor_1.pPart = PIDPosition_motor_1.Kp * PIDPosition_motor_1.PIDError;
	PIDPosition_motor_1.iPart += PIDPosition_motor_1.Ki * PIDPosition_motor_1.PIDError;
	PIDPosition_motor_1.dPart = PIDPosition_motor_1.Kd * (PIDPosition_motor_1.PIDError - PIDPosition_motor_1.PIDErrorTemp1);

/*	Uncomment to enable iPart-limit
	if (PIDPosition_motor_1.iPart > 15)
		PIDPosition_motor_1.iPart = 15;
	else if (PIDPosition_motor_1.iPart < -15)
		PIDPosition_motor_1.iPart = -15;*/

	PIDPosition_motor_1.PIDResult = PIDPosition_motor_1.pPart + PIDPosition_motor_1.iPart + PIDPosition_motor_1.dPart;
	if (PIDPosition_motor_1.PIDResult > MaxResponse)
		PIDPosition_motor_1.PIDResult = (float)(MaxResponse);
	if (PIDPosition_motor_1.PIDResult < -1 * MaxResponse)
		PIDPosition_motor_1.PIDResult = (float)(-1 * MaxResponse);
	PIDPosition_motor_1.PIDErrorTemp1 = PIDPosition_motor_1.PIDError;
}

//MaxResponse: Max Duty Cycle
void PIDVerCalc_motor_1(int32_t Speed, int32_t MaxResponse)
{
	PIDVelocity_motor_1.PIDError = PIDVelocity_motor_1.SetPoint - Speed;
	PIDVelocity_motor_1.pPart = PIDVelocity_motor_1.Kp * PIDVelocity_motor_1.PIDError;
	PIDVelocity_motor_1.iPart += PIDVelocity_motor_1.Ki * PIDVelocity_motor_1.PIDError;
	PIDVelocity_motor_1.dPart = PIDVelocity_motor_1.Kd * (PIDVelocity_motor_1.PIDError - PIDVelocity_motor_1.PIDErrorTemp1);
/*
	if (PIDVelocity.iPart > 40)
		PIDVelocity.iPart = 40;
	else if (PIDVelocity.iPart < -40)
		PIDVelocity.iPart = -40;
*/
	PIDVelocity_motor_1.PIDResult += (PIDVelocity_motor_1.pPart + PIDVelocity_motor_1.iPart + PIDVelocity_motor_1.dPart);
	if (PIDVelocity_motor_1.PIDResult > MaxResponse)
		PIDVelocity_motor_1.PIDResult = (float)MaxResponse;
	if (PIDVelocity_motor_1.PIDResult < -1 * MaxResponse)
		PIDVelocity_motor_1.PIDResult = (float)(-1 * (MaxResponse));
	PIDVelocity_motor_1.PIDErrorTemp1 = PIDVelocity_motor_1.PIDError;
}
