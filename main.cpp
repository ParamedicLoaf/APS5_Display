// ************************** Henrique  **************************************//
//  Programa Teste  - Capturando dados do Touch
//
// ************** Display TFT-  ILI9341 Toutch********************************\\


//************************ Biblioteca*****************************************//
#include "mbed.h"
#include "Arduino.h"
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include "TouchScreen_kbv_mbed.h"
#include "Motor.h"

//******************************Configuração Motor*******************************//
InterruptIn botao(PB_2);
AnalogIn EixoYJoyStick(PC_3);

bool REF = 0;
int joy_y;
int pos;

Timer display;


//******************************Configuração do Display***********************//
const PinName XP = D8, YP = A3, XM = A2, YM = D9; 
const int TS_LEFT=121,TS_RT=922,TS_TOP=82,TS_BOT=890;
DigitalInOut YPout(YP);
DigitalInOut XMout(XM);
TouchScreen_kbv ts = TouchScreen_kbv(XP, YP, XM, YM);
TSPoint_kbv tp;

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
//***********************Orientação  Display**********************************//

uint8_t Orientation = 2;

//****************************************************************************//



//***********************Tabela de Cores**************************************//

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

//****************************************************************************//


Serial pc(USBTX, USBRX);



//***********************Escrita no  Display**********************************//
void forma ()
{

    tft.fillScreen(BLACK);
    tft.setCursor(0, 0); // Orientação X,Y
    tft.print(pos);
    tft.println(" passos");
    tft.print(pos*3/200);
    tft.print(" mm");
}

void estado_ref(){
    referencia();
    REF = 1;
    pos = 0;
    tft.fillScreen(BLACK);
    display.start();
    forma();
}


//****************************************************************************//



void setup(void)
{
    
    tft.reset();
    tft.begin();
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);  // Fundo do Display
    tft.setTextColor(CYAN);
    tft.setTextSize(3);
    
    delay(1000);

    botao.rise(&estado_ref);
    tft.printf("\rPor favor aperte o botão para referenciar");
}

void loop(){

    while (REF == 1){

        if (display.read_ms()>2000){
            forma();
            display.reset();
        }

        joy_y = EixoYJoyStick.read() * 1000;

        if (joy_y>600){
            //gira_motor_sentido_horario();
            pos = pos + gira_motor_sentido_horario();

        } else if (joy_y<400){
            //gira_motor_sentido_antihorario();
            pos = pos + gira_motor_sentido_antihorario();

        } else {
            stop();
        }
    } 
}