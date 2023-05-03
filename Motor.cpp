#include "Motor.h"
#include "mbed.h"

DigitalOut clk(PB_13);
DigitalOut direcao(PB_14);


InterruptIn fdc1(PB_15);
InterruptIn fdc2(PB_1);

int tempo = 2;


int gira_motor_sentido_horario(){

    if (fdc1 == 0){
        direcao = 1;
        clk =1;
        wait_ms(tempo/2);
        clk = 0;
        wait_ms(tempo/2);
        return 1;
    } else {
        return 0;
    }
    
}

int gira_motor_sentido_antihorario(){

    if (fdc2 == 0){
        direcao = 0;
        clk =1;
        wait_ms(tempo/2);
        clk = 0;
        wait_ms(tempo/2);
        return -1;
    } else {
        return 0;
    }

}

void stop(){
    clk = 0;
}

void referencia(){

    while(fdc2==0){
        gira_motor_sentido_antihorario();
    }

    stop();  
    
}

