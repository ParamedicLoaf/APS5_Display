// ******    Henrique  ****** 04/05/21
//  Teste com botões Touch
//
// ************** Display TFT-  ILI9341  Versão V 3************** \\


#include "mbed.h"
#include "Arduino.h"
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
#include "TouchScreen_kbv_mbed.h"
DigitalOut led_1(PB_13);
DigitalOut led_2(PB_14);
DigitalOut led_3(PB_15);


const int TS_LEFT=150,TS_RT=120,TS_TOP=920,TS_BOT=940;
const PinName XP = D7, YP = A2, XM = A1, YM = D6;  //next common configuration
DigitalInOut YPout(YP);

DigitalInOut XMout(XM);


Serial pc(USBTX, USBRX);

long map(long x, long in_min, long in_max, long out_min, long out_max)
{
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


TouchScreen_kbv ts = TouchScreen_kbv(XP, YP, XM, YM, 300);
TSPoint_kbv tp;

#define MINPRESSURE 10
#define MAXPRESSURE 1000



uint8_t Orientation = 1;    // Orientção do Display

// Tabela de valores de cores :
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

bool botu_1 =0;

void show_tft(void)
{

    tft.setTextSize(2);


    tft.setTextColor(MAGENTA,BLACK);


    while (1) {
        tp = ts.getPoint();
        YPout.output();
        XMout.output();

        if (tp.z < MINPRESSURE && tp.z > MAXPRESSURE)

        tp.x = tft.width() - (map(tp.x, TS_RT, TS_LEFT, tft.width(), 0));
        tp.y = tft.height() - (map(tp.y, TS_BOT, TS_TOP, tft.height(), 0));



        if (tp.x==785) {



            led_1= 1;
            led_2 = 0;
            led_3 = 0;


        }

        if (tp.x== 822) {



            led_2 =1;
            led_1 =0;
            led_3 = 0;

        }

        if (tp.x==875) {



            led_2 =0;
            led_1 =0;
            led_3 = 1;

        }
        
        
    if (tp.x==975) {
            
            led_2 =1;
            led_1 =1;
            led_3 =1;
            
            }
            
    if (tp.x==950) {
            
            led_2 =0;
            led_1 =0;
            led_3 =0;

        }

        tft.setCursor(0, (tft.height() * 0) / 4);

        tft.printf(" ", 0 );

       //tft.printf("tp.x=%d tp.y=%d   ", tp.x, tp.y);

    
}

}


void bot_1 ()
{
    
    
    tft.fillCircle(250,190,20,WHITE);
    tft.fillCircle(250,130,20,CYAN);


    tft.setCursor(40, 50);
    tft.drawRoundRect(5, 15, 165, 50, 5, WHITE);
    tft.setTextColor(RED);
    tft.setTextSize(3);
    tft.setCursor(15, 30);
    tft.println("Button 1");

    tft.drawRoundRect(5, 70, 165, 50, 5, WHITE);
    tft.setTextColor(BLUE);
    tft.setTextSize(3);
    tft.setCursor(15,85);
    tft.println("Button 2");


    tft.drawRoundRect(5, 125, 165, 50, 5, WHITE);
    tft.setTextColor(YELLOW);
    tft.setTextSize(3);
    tft.setCursor(15,140);
    tft.println("Button 3");


}

void setup(void)
{

    tft.reset();
    tft.begin();
    tft.setRotation(Orientation);
    tft.fillScreen(BLACK);
    bot_1();
    show_tft();


    delay(1000);
}

void loop()
{

}

