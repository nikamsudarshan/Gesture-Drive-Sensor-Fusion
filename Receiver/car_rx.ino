#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins
const byte address[6] = "00001";

struct Data_Package {
  int xAxis;
  int yAxis;
};
Data_Package data;

// Motor Driver Pins (L298N)
int enA = 5;
int in1 = 2;
int in2 = 3;
int in3 = 4;
int in4 = 7;
int enB = 6;

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package));

    int x = data.xAxis;
    int y = data.yAxis;

    // Deadzone: Keeps the car completely still if the hand is flat
    if (y > 350 && y < 700 && x > 350 && x < 700) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      analogWrite(enA, 0);
      analogWrite(enB, 0);
    }
    // Forward
    else if (y < 350) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enA, 200);
      analogWrite(enB, 200);
    }
    // Backward
    else if (y > 700) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enA, 200);
      analogWrite(enB, 200);
    }
    // Turn Left
    else if (x < 350) {
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      analogWrite(enA, 200);
      analogWrite(enB, 200);
    }
    // Turn Right
    else if (x > 700) {
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      analogWrite(enA, 200);
      analogWrite(enB, 200);
    }
  }
}
