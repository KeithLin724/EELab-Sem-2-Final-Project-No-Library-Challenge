//IMPORT:
#include <HCSR04.h>
#include <AdvancedSevenSegment.h>
#include <LM35.h>
#include <TimerOne.h>

//SETUP: the DistanceSensor 
#define tri A1
#define echo A2 
//setup the button using interrupt 
#define button 2 
// display 
//#define C_SEG 1, 0, 0, 1, 1, 1, 0
#define n_SEG 0, 0, 1, 0, 1, 0, 1
#define o_SEG 1, 1, 0, 0, 0, 1, 1

//sensor
UltraSonicDistanceSensor distanceSensor(tri, echo);
//display 
AdvanceSevenSegment seg(11, 10, 8, 7, 6, 12, 13, 9); // GLOBAL: seven segment
const int pin_dis[] = { 5,4 }; // display change pin 
char display_chr[6];
//GLOBAL: temperature sensor 
LM35 temp(A0);
//mode 
volatile boolean MODE = true;
volatile uint16_t mainStep = 0; // only 6 step 
#define step_plus(s) ((s + 1) == 6 ? 0: s + 1) // step de-overflow func 
#define to_INT(num) num - '0'
// 
// display function according the step 
void display_function(uint16_t step);

// make number to char array 
void to_display_chr_custom(float number) {
    String num_Str;
    //Serial.print("Step: " + String(mainStep));
    Serial.print("Mode: "); // Serial output the mode 

    if (MODE) {// distance mode 
        Serial.print("distance mode, ");
        if (number < 100) {
            // add zero in front 
            if (number > 0) {
                num_Str = String('0') + String(static_cast<int>(number));
            }
            else {
                num_Str = String("000");
            }

        }
        else {
            num_Str = String(static_cast<int>(number));

        }
        Serial.print((number < 0 ? 0 : number));
        Serial.println(" cm");
    }
    else { //temperature mode  
        Serial.print("temperature mode, ");

        if (number < 100) {
            num_Str = String(static_cast<int>(number * 10));
        }
        else {
            num_Str = String(static_cast<int>(number));
        }

        Serial.print(number);
        Serial.println(" C");

    }

    num_Str.toCharArray(display_chr, 6);

}
// change mode from button  
void change_mode() {
    static uint32_t last_interrupt_time = 0;
    uint32_t interrupt_time = millis();

    if (interrupt_time - last_interrupt_time > 200) { //debouncing function 
        MODE = !MODE; //change mode 
        mainStep = 0; //init step 

        if (MODE) { // distance mode 
            to_display_chr_custom(distanceSensor.measureDistanceCm(temp.cel()));
        }
        else { //temperature mode  
            to_display_chr_custom(temp.cel());
        }

    }
    last_interrupt_time = interrupt_time;

}

//function for display 
void move_step() {
    mainStep++;
    if (mainStep == 6) {
        mainStep = 0;

    }
    else if (mainStep == 5) {
        if (MODE) { // distance mode 
            to_display_chr_custom(distanceSensor.measureDistanceCm(temp.cel()));
        }
        else { //temperature mode  
            to_display_chr_custom(temp.cel());
        }

    }

}

void passive_pin(boolean f_s) {
    digitalWrite(pin_dis[0], f_s);
    digitalWrite(pin_dis[1], !f_s);
}

void clr_dis() {
    digitalWrite(pin_dis[0], LOW);
    digitalWrite(pin_dis[1], LOW);
}

// display function 
void dis_ss() {
    auto Main_step_tmp = mainStep;
    int display_step[] = { Main_step_tmp , step_plus(Main_step_tmp) };
    boolean state = LOW;

    for (auto& e : display_step) {
        clr_dis();
        display_function(e);
        state = !state;
        passive_pin(state);
        delay(10);

    }
}

void setup() { // SETUP:
    //for debug 
    Serial.begin(9600);

    //display setup 
    pinMode(pin_dis[0], OUTPUT);
    pinMode(pin_dis[1], OUTPUT);

    // Timer init 
    Timer1.initialize(750000); //0.75 second //750000 us //like thead 
    Timer1.attachInterrupt(move_step);

    //button setup 
    pinMode(button, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button), change_mode, FALLING);

    //init function 
    MODE = 0;
    to_display_chr_custom(distanceSensor.measureDistanceCm(temp.cel()));
    seg.setDot(0);

}

// only display 
void loop() {
    dis_ss();
}

void display_function(uint16_t step) {
    seg.setDot(0);
    if (MODE) { // distance mode 
        switch (step) {
        case 0:
            seg.setNumber(to_INT(display_chr[0]));
            break;
        case 1:
            seg.setNumber(to_INT(display_chr[1]));
            break;
        case 2:
            seg.setNumber(to_INT(display_chr[2]));
            break;
        case 3:
            seg.setCharacter('c');
            break;
        case 4:
            seg.print(n_SEG);
            break;
        case 5: // space
            seg.clean();
            break;

        default: // 
            seg.clean();
            break;
        }
    }
    else { //temperature mode  
        switch (step) {
        case 0:
            seg.setNumber(to_INT(display_chr[0]));
            break;
        case 1:
            seg.setNumber(to_INT(display_chr[1]));
            seg.setDot(1);
            break;
        case 2:
            seg.setNumber(to_INT(display_chr[2]));
            break;
        case 3:
            seg.print(o_SEG);
            break;
        case 4:
            seg.setCharacter('c');
            break;
        case 5: // space
            seg.clean();
            break;

        default: // 
            seg.clean();
            break;
        }
    }
}