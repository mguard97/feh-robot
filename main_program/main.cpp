#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <math.h>


//Constants unable to be defined
float maxSpeedPercentage = 50.00;
float distanceBetweenWheels = 7.00; //measure between wheels
float pi = 3.14159265358;
int IGWAN_COUNTS_PER_REVOLUTION = 200;
float WHEEL_RADIUS = 3.00;

//True Global Variables:
int main_State;

//Type Enumerations

//defines expected voltage readings for relevant colors (from Exploration 1)
enum ColorVoltage
{
    Color_RED = 270, //avg V reading from red light
    Color_BLUE = 120, //avg V reading from blue light
    Color_NULL = 0,
    Color_T = 10 //expected range of values for valid colors
};


//Component Declarations:

//Wheel Motors

//Left
FEHMotor left_wheel_motor(FEHMotor::Motor0,12.0);
DigitalEncoder left_wheel_encoder(FEHIO::P0_0);

//Right
FEHMotor right_wheel_motor(FEHMotor::Motor1,12.0);
DigitalEncoder right_wheel_encoder(FEHIO::P0_1);

//Cds Cell
AnalogInputPin main_Cds(FEHIO::P0_2);

//Function Prototypes

//Control Functions
void drive(float distance);
void turn(float theta);

//Sensor Functions
int getCurrentColor();

//Math Utilities
int distanceToCounts(float distance);
float countsToDistance(int counts);
float thetaToArcLength(float theta);
float arcLengthToTheta(float arcLength);

//Action State Function Prototypes
void idle();
void play_SeismographButton();
void play_Lever();
void play_LavaCore();
void play_Sattelite();
void play_FinalButton();

//Transition State Function Prototypes
void transitionToSeismographButtonFromIdle();
void transitionToLeverFromSeismographButton();
void transitionToLavaCoreFromLever();
void transitionToSatteliteFromLavaCore();
void transitionToFinalButtonFromSattelite();

//Lave Core Machine Transitions


int main(void)
{

    main_State = 0;
    //0 - start idle
    //1 - seismograph button
    //2 - lever
    //3 - lava core
    //4 - sattelite
    //5 - final button
    //6 - finish
    while(true){
        switch(main_State){
            case 0:
                idle();
            case 1:
                play_SeismographButton();
            case 2:
                play_Lever();
            case 3:
                play_LavaCore();
            case 4:
                play_Sattelite();
            case 5:
                play_FinalButton();
        }
    }



}



//Control Functions:

//moves robot forward @distance inches (distance <0 to go backwards)
//@param distance - linear distance in inches for the robot to move (forward as positive)
//pre: {@code:left_wheel_motor} & {@code:right_wheel_motor} are both
// stopped
//post: {@code:left_wheel_motor} & {@code:right_wheel_motor} are both
// stopped, robot has traveled distance specified.
void drive(float distance){
    left_wheel_encoder.ResetCounts();//reinitialize count values. consider storing travel information globally (TODO)
    right_wheel_encoder.ResetCounts();

    int leftCounts = left_wheel_encoder.Counts();//should be set to zero from above lines, but leave in for more versatile implementation later
    int rightCounts = right_wheel_encoder.Counts();

    //Straightness correction
    //note: left is control, right is variable
    left_wheel_motor.SetPercent(maxSpeedPercentage);
    right_wheel_motor.SetPercent(maxSpeedPercentage); //consider += error/kp for correction, will need to be updated inside of loop
    while(countsToDistance((left_wheel_encoder.Counts() - leftCounts)) < distance){
        Sleep(100); //adjust buffer to be within margin for motor but above proteus refresh rate
        //Does power not supply to motor when sleep?
    }
    left_wheel_motor.Stop();
    right_wheel_motor.Stop();
    //consider reporting distance information here.
}


//turns robot @theta radians (theta > 0 for clockwise, theta < 0 counter-clockwise)
//@param theta - angular distance in radians for the robot to turn
//pre: {@code:left_wheel_motor} & {@code:right_wheel_motor} are both
//stopped
//post: {@code:left_wheel_motor} & {@code:right_wheel_motor} are stopped, robot has turned angle specified.
void turn(float theta){
    left_wheel_encoder.ResetCounts();
    right_wheel_encoder.ResetCounts();

    int leftCounts = left_wheel_encoder.Counts();//should be set to zero from above lines, but leave in for more versatile implementation later
    int rightCounts = right_wheel_encoder.Counts();

    float linearRotation = thetaToArcLength(theta);
    int directionMultiplier = (theta < 0)?(-theta):(theta); //turn left or right? depending on value of theta

    left_wheel_motor.SetPercent(maxSpeedPercentage*directionMultiplier);
    right_wheel_motor.SetPercent(maxSpeedPercentage*directionMultiplier*-1);
    while(countsToDistance((left_wheel_encoder.Counts() - leftCounts)) < linearRotation){ //FIX THIS LINE (HOW FAR SHOULD EACH WHEEL SPIN??)
        Sleep(100); //adjust buffer to be within margin for motor but above proteus refresh rate
        //Does power not supply to motor when sleep?
    }
    left_wheel_motor.Stop();
    right_wheel_motor.Stop();
}

//Sensor Functions:
int getCurrentColor(){
    int cdsReading = main_Cds.Value();
    if(cdsReading >= (Color_BLUE - Color_T) && cdsReading <= (Color_BLUE + Color_T)){
        //BLUE
        cdsReading = Color_BLUE;
    }
    else if(cdsReading >= (Color_RED - Color_T) && cdsReading <= (Color_RED + Color_T)){
        //RED
        cdsReading = Color_RED;
    }
    else{
        cdsReading = Color_NULL;
    }
    return cdsReading;
}

//Math Utilities:

//converts @distance to a number of counts for used motor
int distanceToCounts(float distance){
    int ans = distance/((1/IGWAN_COUNTS_PER_REVOLUTION)*2*pi*WHEEL_RADIUS);
                  //figure out way to typecast??? (int) not working??
    return ans;
}
//converts @counts in terms of given motor to a distance
float countsToDistance(int counts){
    return counts/(IGWAN_COUNTS_PER_REVOLUTION)*2*pi*WHEEL_RADIUS;
}
//converts @theta to arc length
float thetaToArcLength(float theta){
    return(theta*(distanceBetweenWheels/2));
}
//converts @arcLength to an angle measure
float arcLengthToTheta(float arcLength){
    return(arcLength*2/distanceBetweenWheels);
}


//State function implementations:
void idle(){
    //deaden all systems
    left_wheel_motor.Stop();
    right_wheel_motor.Stop();

    //wait for light
    while(!(getCurrentColor() == Color_RED)){
        Sleep(100);
    }

    transitionToSeismographButtonFromIdle();
    main_State = 1;
}
void play_SeismographButton(){
    //navigate to seismograph button
    //press button
    transitionToLeverFromSeismographButton();

    main_State = 2;

}
void play_Lever(){
    //navigate to lever
    //pull lever
    transitionToLavaCoreFromLever();
    main_State = 3;
}

void play_LavaCore(){
    //navigate to ready-zone
    //retrieve lava core
    //navigate to light
    //determine light color
    //navigate to appropriate bucket
    //drop lava
    transitionToSatteliteFromLavaCore();
    main_State = 4;
}

void play_Sattelite(){
    //navigate to satellite
    //turn satellite
    transitionToFinalButtonFromSattelite();
    main_State = 5;
}

void play_FinalButton(){
    //navigate to final button
    //press it!!!
    //cry cause you did it
    transitionToIdleFromEnd();
    main_State = 0;
}

//transition functions
void transitionToSeismographButtonFromIdle(){

}
void transitionToLeverFromSeismographButton(){

}

void transitionToLavaCoreFromLever(){
