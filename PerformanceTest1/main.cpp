#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

//Left
FEHMotor left_wheel_motor(FEHMotor::Motor0,12.0);
DigitalEncoder left_wheel_encoder(FEHIO::P0_0);

//Right
FEHMotor right_wheel_motor(FEHMotor::Motor1,12.0);
DigitalEncoder right_wheel_encoder(FEHIO::P0_1);

//Cds Cell
AnalogInputPin main_Cds(FEHIO::P0_2);

//dimensions and math constants
float maxSpeedPercentage = 50.00;
float distanceBetweenWheels = 7.00; //measure between wheels
float pi = 3.14159265358;
int IGWAN_COUNTS_PER_REVOLUTION = 200;
float WHEEL_RADIUS = 3.00;

float red = 0.2;
float blue = 0.6;
float cds_tolerance = 0.1;
float unlit = 1.5;
int mainState;
enum mainStates{
    MAIN_IDLE,
    MAIN_SEISMOGRAPH
};
enum CDS_Colors{
    CDS_RED,
    CDS_BLUE,
    CDS_UNLIT
};



void playIdle();
void playSeismograph();

int determineColor(float cdsRead);

int main(void)
{
    mainState = MAIN_IDLE;

    while(true){
        switch(mainState){
            case MAIN_IDLE:
                playIdle();
            break;
            case MAIN_SEISMOGRAPH:
                playSeismograph();
            break;
        }
    }
    return 0;
}

int distanceToCounts(float distance){
    return(distance*(IGWAN_COUNTS_PER_REVOLUTION)/(2*pi*WHEEL_RADIUS));
}
float countsToDistance(int counts){
    return(counts*WHEEL_RADIUS*2*pi/IGWAN_COUNTS_PER_REVOLUTION);
}

void playIdle(){
    left_motor.Stop();
    right_motor.Stop();
    LCD.Clear(BLACK);
    LCD.WriteLine("Waiting for Starting Light...");
    while(determineColor(main_Cds.Value()) != CDS_RED){ //while starting light is not on
    }

    LCD.Clear(Black);
    mainState = MAIN_SEISMOGRAPH;
}
void playSeismograph(){
    //set of drive and turns,

    mainState = MAIN_IDLE;
}
int determineColor(float cdsRead){
    if(val > (blue - cds_tolerance) && val < (blue + cds_tolerance)){
        return CDS_BLUE;
    }
    else if(val > (red - cds_tolerance) && val < (red + cds_tolerance)){
        return CDS_RED;
    }
    else{
        return CDS_UNLIT;
    }
}
