#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  600 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  2400 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

String statesList[] = {
  "init: -1", "lie: 0",
  "stay: 1", "sit: 2", 
  "go forward: 3", "go backward: 4", 
  "turn right: 5", "turn left: 6", 
  "tilt right: 7", "tilt left: 8"
};

//Подключение выводов к конкретным пинам платы
const int legPins[4][3] = {  
  {0, 1, 2},    // Передняя правая
  {3, 4, 5},    // Передняя левая
  {6, 7, 8},    // Задняя правая
  {9, 10, 11}   // Задняя левая
}; 

//Калибровочные коэффициенты
int legCoeffs[4][3] = {
  {0, 0, 0},    // Передняя правая
  {0, 0, 0},    // Передняя левая
  {0, 0, 0},    // Задняя правая
  {0, 0, 0}     // Задняя левая
};

//Список состояний робота
const int calibrateState[4][3] = {
  {90, 90, 90},
  {30, 90, 90},
  {90, 90, 50},
  {50, 90, 90}
};

const int stayState[4][3] = {
  {30, 0, -2}, 
  {45, 0, 2},
  {40, 0, 2},
  {55, 0, -2}
};

const int runForwardMaxAngleState[4][3] = {
  {20, 5, -2},
  {25, -5, 2},
  {30, 5, 2},
  {50, -5, -2}
};

const int runForwardMinAngleState[4][3] = {
  {35, 5, -2},
  {40, -5, 2},
  {45, 5, 2},
  {65, -5, -2}
};

const int runBackwardMaxAngleState[4][3] = {
  {30, 5, -2},
  {35, -5, 2},
  {40, 10, 2},
  {55, -10, -2}
};

const int runBackwardMinAngleState[4][3] = {
  {30, -5, -2},
  {35, 5, 2},
  {40, -5, 2},
  {55, 5, -2}
};

const int turnRightMaxAngleState[4][3] = {
  {20, 5, -10}, 
  {25, -5, 10},
  {30, -5, 7},
  {50, 5, -10}
};

const int turnRightMinAngleState[4][3] = {
  {35, -5, 5}, 
  {40, 5, -5},
  {45, 5, -5},
  {65, -5, 5}
};

const int turnLeftMaxAngleState[4][3] = {
  {20, 5, -10}, 
  {25, -5, 10},
  {40, 0, 2},
  {55, 0, -2}
};

const int turnLeftMinAngleState[4][3] = {
  {35, -5, 5}, 
  {40, 5, -5},
  {40, 0, 2},
  {55, 0, -2}
};

const int tiltLeftState[4][3] = {
  {35, 0, -10},
  {20, -15, 10},
  {40, 0, 2},
  {30, -15, -15}
};

const int tiltRightState[4][3] = {
  {10, 15, -10},
  {40, 0, 10},
  {15, 15, 5},
  {60, 0, -10}
};

const int sitState[4][3] = {
  {30, 0, 0}, 
  {35, 0, 0},
  {15, 0, 0},
  {30, 0, 0}
};

const int lieState[4][3] = {
  {0, 0, 0}, 
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

const int initState[4][3] = {
  {0, 0, 0}, 
  {0, 0, 0},
  {0, 0, 0},
  {0, 0, 0}
};

#endif //SETTINGS_HPP
