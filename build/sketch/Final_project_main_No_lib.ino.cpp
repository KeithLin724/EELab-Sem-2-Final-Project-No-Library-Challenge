#include <Arduino.h>
#line 1 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
/*
Title:Final Project
Written By Keith Lin
This is a code using for EELab
Sem 2 final project
Date: 22/5/2022
gifted to MP Liew
*/

#include <TimerOne.h>

// set up pin 
//seven segment display 
// a b c d e f g DP
const uint8_t displayPins[8] = { 11,10, 8, 7, 6, 12, 13, 9 };
const uint8_t pinDiff[2] = { 5,4 }; // control two different display
char display_chr[6] = {}; // display array 

const byte numbersHex[] = { // table (int to HEX code)
        B0111111, // 0
        B0000110, // 1
        B1011011, // 2
        B1001111, // 3
        B1100110, // 4
        B1101101, // 5
        B1111101, // 6
        B0000111, // 7
        B1111111, // 8
        B1101111  // 9

};
#define NumberHex(index) numbersHex[index] // get the number nex code 

// input button about the interrupt 
const uint8_t button = 2;

// temp setup  
#define tempInputPin A0 
#define tempCelsius(num) (num*(5/1024.0)*100) 

// distance setup 
const uint8_t triggerEchoPin[] = { A1,A2 }; // trigger is 0 , echo is 1 

// main variable
volatile boolean MODE = true; // about the mode , 0 is distance mode , 1 is temperature
volatile uint16_t mainStep = 0; // display step using for traverse the display array 
#define to_INT(chr) (chr - '0')
// step de-overflow func 
#define nextStep(s) ((s + 1) == 6 ? 0: s + 1) 

void Display_SevenSegments(byte hex, bool DP = false, bool DP_only = false);

/**
 * @brief measure temperature function
 *
 * @return float
 */
#line 58 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
float measure_temp();
#line 68 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
int measure_distance();
#line 86 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void to_display_chr_custom(float number);
#line 126 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void change_mode();
#line 142 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void move_step();
#line 154 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void setup();
#line 183 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void loop();
#line 192 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void passive_pin(const boolean& f_s);
#line 198 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void clr_dis();
#line 204 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void diff_SS();
#line 247 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
void display_function(const uint16_t& step);
#line 58 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
float measure_temp() {
    auto temp = analogRead(tempInputPin);
    return tempCelsius(static_cast<float>(temp));
}

/**
 * @brief measure distance function
 *
 * @return int
 */
int measure_distance() {
    pinMode(triggerEchoPin[0], OUTPUT);
    digitalWrite(triggerEchoPin[0], LOW);

    delayMicroseconds(2);

    digitalWrite(triggerEchoPin[0], HIGH);
    delayMicroseconds(10);

    digitalWrite(triggerEchoPin[0], LOW);
    pinMode(triggerEchoPin[1], INPUT);

    auto tmp = pulseIn(triggerEchoPin[1], HIGH);

    return static_cast<int>(tmp * 0.034 / 2);
}

/// @brief convert voltage to a display array to display (is a custom function)
void to_display_chr_custom(float number) {
    String num_Str = "";
    auto tmpNumber = number;
    Serial.print("Mode: "); // Serial output the mode 

    if (number < 0) {
        String("000").toCharArray(display_chr, 6);
        return;
    }

    if (MODE) { // distance mode (int)
        Serial.print("distance mode, ");

        if (number < 100) {// add zero in front 
            num_Str += (number < 10 ? "00" : "0");
        }
        tmpNumber = number;

        Serial.print((number < 0 ? 0 : number));
        Serial.println(" cm");
    }
    else { //temperature mode  (float)
        Serial.print("temperature mode, ");

        if (number < 100) {
            if (number < 10) {
                num_Str += "0";
            }

            tmpNumber *= 10;
        }

        Serial.print(number);
        Serial.println(" C");
    }
    num_Str += String(static_cast<int>(tmpNumber));
    num_Str.toCharArray(display_chr, 6);
}

/// @brief change mode
void change_mode() {
    static uint32_t lastInterruptTime = 0;
    uint32_t interruptTime = millis();

    if (interruptTime - lastInterruptTime > 200) { //debouncing function 
        MODE = !MODE; //change mode 
        mainStep = 0; //init step 

        to_display_chr_custom((MODE == 1) ? measure_distance() : measure_temp());

    }
    lastInterruptTime = interruptTime;

}

/// @brief control by timer (about the display)
void move_step() {
    mainStep++;
    if (mainStep == 6) {
        mainStep = 0;
    }
    else if (mainStep == 5) {
        to_display_chr_custom((MODE == 1) ? measure_distance() : measure_temp());
    }

}

/// @brief init the setup function
void setup() {
    // out put the source from the Serial port 
    Serial.begin(9600);
    //display setup 
    for (auto& pin : displayPins) {
        pinMode(pin, OUTPUT);
    }
    for (auto& pin : pinDiff) {
        pinMode(pin, OUTPUT);
    }
    //sensor 
    for (auto& pin : triggerEchoPin) {
        pinMode(pin, OUTPUT);
    }
    // Timer1 setup
    Timer1.initialize(750000);
    Timer1.attachInterrupt(move_step);

    pinMode(button, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(button), change_mode, FALLING);

    //initialize
    MODE = 0;
    to_display_chr_custom(measure_distance());
    Display_SevenSegments(0, true, true);

}

/// @brief main loop
void loop() {
    diff_SS();
}

/**
 * @brief make pin display is passive
 *
 * @param f_s control the light pin
 */
void passive_pin(const boolean& f_s) {
    digitalWrite(pinDiff[0], f_s);
    digitalWrite(pinDiff[1], !f_s);
}

//clear display 
void clr_dis() {
    digitalWrite(pinDiff[0], LOW);
    digitalWrite(pinDiff[1], LOW);
}

/// @brief Control two seven segment display function
void diff_SS() {
    auto tmpStep = mainStep;
    byte displayStep[] = { tmpStep, nextStep(tmpStep) };
    bool state = false;

    for (const auto& e : displayStep) {
        clr_dis();
        // TODO 
        display_function(e);
        state = !state;
        passive_pin(state);
        delay(10);
    }

}

/**
 * @brief Display only using number
 *
 * @param hex Display the hex code
 * @param DP About the decimal point
 * @param DP_only Control decimal point mode
 */
void Display_SevenSegments(byte hex, bool DP = false, bool DP_only = false) {
    bool outDisplaySignal = false;

    if (!DP_only) {
        for (int i(0); i < 7; i++) {
            outDisplaySignal = bitRead(hex, i);
            //output
            digitalWrite(displayPins[i], !outDisplaySignal);
        }
    }

    digitalWrite(displayPins[7], !DP);

}

/**
 * @brief function of display using step
 *
 * @param step about the loop display step
 */
void display_function(const uint16_t& step) {

    Display_SevenSegments(0, false, true); //clear dp

    if (MODE) {
        switch (step) {
        case 0:
            Display_SevenSegments(NumberHex(to_INT(display_chr[0])));
            break;
        case 1:
            Display_SevenSegments(NumberHex(to_INT(display_chr[1])));
            break;
        case 2:
            Display_SevenSegments(NumberHex(to_INT(display_chr[2])));
            break;
        case 3:
            Display_SevenSegments(B0111001); // C 
            break;
        case 4:
            Display_SevenSegments(B1010100); // 'n' hex code 
            break;
        case 5:
            Display_SevenSegments(B0000000); // clean 
            break;
        default:
            break;
        }
    }
    else {
        switch (step) {
        case 0:
            Display_SevenSegments(NumberHex(to_INT(display_chr[0])));
            break;
        case 1:
            Display_SevenSegments(NumberHex(to_INT(display_chr[1])), true);
            break;
        case 2:
            Display_SevenSegments(NumberHex(to_INT(display_chr[2])));
            break;
        case 3:
            Display_SevenSegments(B1100011); // o
            break;
        case 4:
            Display_SevenSegments(B0111001); // C
            break;
        case 5:
            Display_SevenSegments(B0000000); // clean 
            break;
        default:
            break;
        }
    }

}
