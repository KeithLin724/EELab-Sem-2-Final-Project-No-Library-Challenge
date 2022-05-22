# 1 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
# 2 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 2
# 3 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 2
// set up pin 
//seven segment display 
// a b c d e f g DP
const uint8_t displayPins[8] = { 0,1,2,3,4,5,6,7 };
const uint8_t pinDiff[2] = { 5,4 }; // control two different display

// input button about the interrupt 
const uint8_t button = 2;
bool MODE = false; // display mode  
// temp setup  


// distance setup 
// trigger is 0 , echo 1 
const uint8_t triggerEchoPin[] = { A1,A2 };


// measure temperature function 
float measure_temp() {
    auto temp = analogRead(A0);
    return (static_cast<float>(temp)*(5/1024.0)*100);
}

// measure distance function
int measure_distance() {
    pinMode(triggerEchoPin[0], 0x1);
    digitalWrite(triggerEchoPin[0], 0x0);

    delayMicroseconds(2);

    digitalWrite(triggerEchoPin[0], 0x1);
    delayMicroseconds(10);

    digitalWrite(triggerEchoPin[0], 0x0);
    pinMode(triggerEchoPin[1], 0x0);

    auto tmp = pulseIn(triggerEchoPin[1], 0x1);
    int distance = tmp * 0.034 / 2;

    return distance;
}

//init the setup function 
void setup() {
    for (auto& pin : displayPins) {
        pinMode(pin, 0x1);
    }
    for (auto& pin : triggerEchoPin) {
        pinMode(pin, 0x1);
    }
    
# 53 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   __asm__ __volatile__ ("cli" ::: "memory")
# 53 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
                 ; // disable all interrupts
    
# 54 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   (*(volatile uint8_t *)(0x80)) 
# 54 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
          = 0;

    
# 56 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 56 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
          = 0;


    
# 59 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   (*(volatile uint16_t *)(0x84)) 
# 59 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
         = value; // preload timer

    
# 61 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   (*(volatile uint8_t *)(0x81)) 
# 61 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
          |= (1 << 
# 61 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
                   0
# 61 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
                       ) | (1 << 
# 61 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
                                 2
# 61 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
                                     ); // 1024 prescaler 

    
# 63 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   (*(volatile uint8_t *)(0x6F)) 
# 63 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
          |= (1 << 
# 63 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
                   0
# 63 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
                        );


    // enable timer overflow interrupt ISR

    
# 68 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino" 3
   __asm__ __volatile__ ("sei" ::: "memory")
# 68 "d:\\Arduino\\School\\EELab\\Sem2\\Final project no libary\\Final_project_main_No_lib.ino"
               ;
    // enable all interrupts


}

void loop() {

}


//display function in one seven segment 
// input number range 0 to 9 
void output_number(int number, bool DP) {

    byte numbersHex[] = {
        63,
        6,
        91,
        79,
        102,
        109,
        125,
        7,
        127,
        111

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
        outDisplaySignal = (((hex) >> (i)) & 0x01);
        //output
        digitalWrite(displayPins[i], outDisplaySignal);
    }
    if (DP) {
        digitalWrite(displayPins[8], true);
    }
}
