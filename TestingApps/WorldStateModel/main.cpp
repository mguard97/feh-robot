#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

FEHMotor left_motor(FEHMotor::Motor0,12.0);
FEHMotor right_motor(FEHMotor::Motor1,12.0);

DigitalEncoder left_motor_encoder(FEHIO::P0_0);
DigitalEncoder right_motor_encoder(FEHIO::P0_1);

AnalogInputPin CdsCell(FEHIO::P0_2);

int logposition = 0;
struct WorldState{
    //right motor state
    float rightPower;
    float rightCounts;
    //left motor state
    float leftPower;
    float leftCounts;
    //CdS_Cell sensor
    float cdsReading;

    //Main_State
    int main_state;
    //Distance Traveled
    float totalDistance;
    //TimeStamp
    float currentTime;
};
WorldState log[500];
void updateWorldState(float left_power, float right_power){
    struct WorldState currentState = {right_power,right_motor_encoder.Counts(), left_power,left_motor_encoder.Counts(), CdsCell.Value(), main_state,distance, TimeNow()};

}
