//Display TFT-  ILI9341 Toutch

//Biblioteca
#include "mbed.h"
#include "Arduino.h"
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include "TouchScreen_kbv_mbed.h"
#include "Motor.h"
#include "main.h"

// Configuração Motor Y
DigitalIn confirma(PB_2);
AnalogIn EixoYJoyStick(PC_3);
InterruptIn emergencia(PB_11);
InterruptIn fdc2_y_(PB_1);

// variaveis auxiliares
bool REF = 0;
int joy_y;
int pos_y;

bool flag_emergencia = 1;

Timer display;
Timer debounce;


// Configuração do Display
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

// Orientação  Display
uint8_t Orientation = 1;

// Tabela de Cores

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

// Serial
//Serial pc(USBTX, USBRX);

//***********************Escrita no  Display**********************************//
void print_posicao(){

    tft.fillScreen(BLACK);
    tft.setCursor(0, 0); // Orientação X,Y
    tft.print(pos_y);
    tft.println(" passos");
    tft.print(pos_y*3/200);
    tft.print(" mm");
}

void referenciamento_tela(){

    tft.fillScreen(BLACK);
    tft.setCursor(0, 0); // Orientação X,Y
    tft.printf("\rPor favor\naperte confirma\npara referenciar");
}

void emergencia_tela(){

    tft.fillScreen(BLACK);
    tft.setCursor(0, 0); // Orientação X,Y
    tft.setTextColor(RED);
    tft.print("\rEMERGENCIA\n");
    tft.setTextColor(CYAN);
    tft.println("Desative\no botao\nquando for\nseguro");
}

//****************************************************************************//


// PROGRAMA PRINCIPAL ____________________________________________________________

void setup(void)
{
    //configs da tela
    tft.reset();
    tft.begin();
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);  // Fundo do Display
    tft.setTextColor(CYAN);
    tft.setTextSize(3);
    delay(1000);

    //inicia o debouncing
    debounce.start();

    //interrupções
    emergencia.fall(&desastre);
    referenciamento_tela();
    
}

void loop(){

    if (confirma==1){
        estado_ref();
    }

    //JOG
    while (REF == 1){

        // a cada 2 segundos, printa a posição no display:
        if (display.read_ms()>2000){ 
            print_posicao();
            display.reset();
        }

        joy_y = EixoYJoyStick.read() * 1000;

        // gira o motor de acordo com a leitura do Joystick
        if (joy_y>600){
    
            pos_y = pos_y + gira_y_mais();

        } else if (joy_y<400){

            pos_y = pos_y + gira_y_menos();

        } else {
            stop_y();
        }
    } 
}



//***********************Funções gerais**********************************//

void desastre(){
    

        stop_y(); //para o motor
        REF = 0; //
        flag_emergencia = 0;
        emergencia_tela();
        while (emergencia == 0){ //enquanto etiver apertado

        }

        referenciamento_tela();

        while (1){
            if(confirma == 1){
                flag_emergencia = 1;
                estado_ref();
                break;
            }
        }
        
        
    
}

void estado_ref(){
    if (debounce.read_ms() >30 && REF==0 && emergencia==1){
        referencia();
        REF = 1;
        pos_y = 0;
        display.start();
        print_posicao();
    }

    debounce.reset();
}

void referencia(){


    while(fdc2_y_==0 && flag_emergencia==1){ //roda até bater no fim de curso 2
        
        gira_y_menos();

        if(emergencia==0){
            break;
        }

    }
    stop_y();
    
}