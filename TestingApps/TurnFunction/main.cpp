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

int distanceToCounts(float distance);
float countsToDistance(int counts);

int thetaToCounts(float theta);

void turn(float percent, float degrees);

int main(void)
{

    float x,y;

    LCD.Clear( FEHLCD::Black );
    LCD.SetFontColor( FEHLCD::White );

    LCD.WriteLine("Starting Program");
    LCD.WriteLine("Touch to Start...");

    while(!LCD.Touch(&x,&y)){

    }

    LCD.WriteLine("Running");
    Sleep(1.0);
    turn(20.0,90);
    return 0;
}

int distanceToCounts(float distance){
    return(distance*(IGWAN_COUNTS_PER_REVOLUTION)/(2*pi*WHEEL_RADIUS));
}
float countsToDistance(int counts){
    return(counts*WHEEL_RADIUS*2*pi/IGWAN_COUNTS_PER_REVOLUTION);
}

void turn(float percent, float degrees){
    left_motor_encoder.ResetCounts();
    right_motor_encoder.ResetCounts();

    LCD.Clear(BLACK);
    LCD.WriteLine("I'm turning");

    int totalCounts = thetaToCounts(degrees);
    LCD.Write("I think I'm rotating this far: ");
    LCD.WriteLine(totalCounts);

    left_motor.SetPercent(percent);
    right_motor.SetPercent(-percent);
    while(((left_motor_encoder.Counts() + right_motor_encoder.Counts())/2 < totalCounts)){
    }
    LCD.WriteLine("I've Stopped");
    LCD.WriteLine("Left Counts:");
    LCD.WriteLine(left_motor_encoder.Counts());
    LCD.WriteLine("Right Counts:");
    LCD.WriteLine(right_motor_encoder.Counts());


}
int thetaToCounts(float theta){
    return distanceToCounts((theta/360)*pi)*distanceBetweenWheels/2;
}
