#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>

float red = 0.2;
float blue = 0.6;
float cds_tolerance = 0.1;
float unlit = 1.5;

AnalogInputPin main_Cds(FEHIO::P0_2);

enum CDS_Colors{
    CDS_RED,
    CDS_BLUE,
    CDS_UNLIT
};

int determineColor(float cdsRead);
int main(void)
{

    LCD.WriteLine("Initializing StartOnLight");
    LCD.WriteLine("Running...");
    while(determineColor(main_Cds.Value()) == CDS_RED){
    }
    LCD.Clear(RED);
    LCD.WriteLine("Light is on");
    return 0;
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
