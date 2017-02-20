#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

float maxSpeedPercentage = 50.00;
float distanceBetweenWheels = 7.00; //measure between wheels
float pi = 3.14159265358;
int IGWAN_COUNTS_PER_REVOLUTION = 318;
float WHEEL_RADIUS = 3.00;

FEHMotor left_motor(FEHMotor::Motor0,12.0);
FEHMotor right_motor(FEHMotor::Motor1,12.0);

DigitalEncoder right_motor_encoder(FEHIO::P0_0);
DigitalEncoder left_motor_encoder(FEHIO::P0_1);

void driveForward(float distance);
int distanceToCounts(float distance);
float countsToDistance(int counts);

int main(void)
{
    float x,y;
    while(!LCD.Touch(&x,&y)){

    }
    Sleep(1.0);

    driveForward(10.0);

    while(!LCD.Touch(&x,&y)){


    }
}

void driveForward(float distance){
    int totalCounts = distanceToCounts(distance);
    float x,y;
    left_motor_encoder.ResetCounts();
    right_motor_encoder.ResetCounts();
    left_motor.SetPercent(maxSpeedPercentage);
    right_motor.SetPercent(maxSpeedPercentage);
    LCD.WriteLine("I'm Driving");
    while(((left_motor_encoder.Counts() + right_motor_encoder.Counts())/2 < totalCounts) && !LCD.Touch(&x,&y)){
    }
    left_motor.Stop();
    right_motor.Stop();
    LCD.WriteLine("I'm Stopped");
    LCD.WriteLine("Summary:");
    LCD.Write("Distance Traveled: ");
    LCD.Write(distance);
    LCD.Write(" in");
    LCD.Write("Left Motor Final: ");
    LCD.WriteLine(left_motor_encoder.Counts());
    LCD.WriteLine(countsToDistance(left_motor_encoder.Counts()));
    LCD.Write("Right Motor Final: ");
    LCD.WriteLine(right_motor_encoder.Counts());
    LCD.WriteLine(countsToDistance(right_motor_encoder.Counts()));

}

int distanceToCounts(float distance){
    return(distance*(IGWAN_COUNTS_PER_REVOLUTION)/(2*pi*WHEEL_RADIUS));
}
float countsToDistance(int counts){
    return(counts*WHEEL_RADIUS*2*pi/IGWAN_COUNTS_PER_REVOLUTION);
}
