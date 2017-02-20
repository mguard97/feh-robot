#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>

float mid_opt_thresh = 1.5;
float tolerance = 0.005;
enum LineStates{
    LINE_ON_RIGHT,
    ON_LINE_FIRST,
    LINE_ON_LEFT,
    ON_LINE_SECOND
};
AnalogInputPin mid_opt(FEHIO::P0_0);
FEHMotor left_motor(FEHMotor::Motor0,12.0);
FEHMotor right_motor(FEHMotor::Motor1,12.0);
bool isSeeLine();
int main(void)
{
    int state = LINE_ON_RIGHT;
    while(true){
        switch(state){
            case LINE_ON_RIGHT:

                if(isSeeLine()){
                    left_motor.SetPercent(15);
                    right_motor.SetPercent(40);
                    state = ON_LINE_FIRST;
                }
            case ON_LINE_FIRST:
                if(!isSeeLine()){
                    left_motor.SetPercent(15);
                    right_motor.SetPercent(15);
                    state = LINE_ON_LEFT;
                }
            break;
            case LINE_ON_RIGHT:
                if(isSeeLine()){
                    left_motor.SetPercent(40);
                    right_motor.SetPercent(15);
                    state = ON_LINE_SECOND;
                }
            }
            case ON_LINE_SECOND:
                if(!isSeeLine()){
                    left_motor.SetPercent(15);
                    right_motor.SetPercent(15);
                    state = LINE_ON_RIGHT;
                }
            break;
    }
    return 0;
}
bool isSeeLine(){
    return ((mid_opt.Value() > (mid_opt_thresh - tolerance)) && (mid_opt.Value() < (mid_opt_thresh + tolerance)));
}
