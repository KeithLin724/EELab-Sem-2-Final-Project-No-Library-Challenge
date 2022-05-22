#line 1 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
#include <TimerOne.h>
#include <Arduino.h>
// set up pin 
//seven segment display 
// a b c d e f g DP
const uint8_t displayPins[8] = { 0,1,2,3,4,5,6,7 };
const uint8_t pinDiff[2] = { 5,4 }; // control two different display

// input button about the interrupt 
const uint8_t button = 2;
bool MODE = false; // display mode  
// temp setup  
#define tempInputPin A0 
#define tempCelsius(num) (num*(5/1024.0)*100) 
// distance setup 
// trigger is 0 , echo 1 
const uint8_t triggerEchoPin[] = { A1,A2 };


// measure temperature function 
#line 21 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
float measure_temp();
#line 27 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
int measure_distance();
#line 46 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void setup();
#line 74 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void loop();
#line 81 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void output_number(int number, bool DP);
#line 107 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void Display_SevenSegments(byte hex, bool DP);
#line 21 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
float measure_temp() {
    auto temp = analogRead(tempInputPin);
    return tempCelsius(static_cast<float>(temp));
}

// measure distance function
int measure_distance() {
    pinMode(triggerEchoPin[0], OUTPUT);
    digitalWrite(triggerEchoPin[0], LOW);

    delayMicroseconds(2);

    digitalWrite(triggerEchoPin[0], HIGH);
    delayMicroseconds(10);

    digitalWrite(triggerEchoPin[0], LOW);
    pinMode(triggerEchoPin[1], INPUT);

    auto tmp = pulseIn(triggerEchoPin[1], HIGH);
    int distance = tmp * 0.034 / 2;

    return distance;
}

//init the setup function 
void setup() {
    for (auto& pin : displayPins) {
        pinMode(pin, OUTPUT);
    }
    for (auto& pin : triggerEchoPin) {
        pinMode(pin, OUTPUT);
    }
    noInterrupts();                       // disable all interrupts
    TCCR1A = 0;

    TCCR1B = 0;


    TCNT1 = value;                        // preload timer

    TCCR1B |= (1 << CS10) | (1 << CS12);    // 1024 prescaler 

    TIMSK1 |= (1 << TOIE1);


    // enable timer overflow interrupt ISR

    interrupts();
    // enable all interrupts


}

void loop() {

}


//display function in one seven segment 
// input number range 0 to 9 
void output_number(int number, bool DP) {

    byte numbersHex[] = {
        B0111111,
        B0000110,
        B1011011,
        B1001111,
        B1100110,
        B1101101,
        B1111101,
        B0000111,
        B1111111,
        B1101111

    };

    if (number < 0 || number > 9) {
        // check is it out of range
        return;
    }

    Display_SevenSegments(numbersHex[number], DP);

}

// display only using number 
void Display_SevenSegments(byte hex, bool DP) {
    bool outDisplaySignal = false;

    for (int i = 0; i < 7; i++) {
        outDisplaySignal = bitRead(hex, i);
        //output
        digitalWrite(displayPins[i], outDisplaySignal);
    }
    if (DP) {
        digitalWrite(displayPins[8], true);
    }
}
