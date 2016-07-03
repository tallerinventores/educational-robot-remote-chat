#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
#include <NewPing.h>
NewPing sonar(2, 3, 200);

//movimiento
#define HALT 0
#define FORWARD 1
#define BACKWARD 2
#define TURN_LEFT 3
#define TURN_RIGHT 4
#define LEFT 1
#define RIGHT 2
#define BOTH 3
//estados
#define STG_WAITING 0
#define STG_HEADER_OK 1
#define STG_LINE_OK 2
//tipos
#define LN_UNKNOWN 0
#define LN_TEXT 1
#define LN_ORDER 2
#define LN_ORDER_NCK 3
#define LN_DONE 4
//cabecera
#define HD_ORDER "ORDEN:"
//usuario
#define USER_NAME "Remote"
#define SAYS " dice:"
#define USERNAME_SAYS USER_NAME SAYS
//ordenes
#define ORD_STOP 0
#define ORD_FORWARD 1
#define ORD_BACKWARD 2
#define ORD_TURN_LEFT 3
#define ORD_TURN_RIGHT 4
#define ORD_CLAXON 5
#define ORD_LIGHT_ON 6
#define ORD_LIGHT_OFF 7
#define ORD_RD_DIST 8
#define TXT_STOP "STOP"
#define TXT_FORWARD "AVANZA"
#define TXT_BACKWARD "RETROCEDE"
#define TXT_LEFT "IZQUIERDA"
#define TXT_RIGHT "DERECHA"
#define TXT_CLAXON "CLAXON"
#define TXT_LON "ENCIENDE"
#define TXT_LOFF "APAGA"
#define TXT_DIST "DISTANCIA"

void setup() {
    //LED
    pinMode(12, OUTPUT);
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    //piezo
    pinMode(4, OUTPUT);
    noTone(4);
    //motores
    pinMode(8, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(6, OUTPUT);
    moveRobot(HALT, BOTH, 0, 0);
    digitalWrite(7, HIGH);
    digitalWrite(8, HIGH);
    //pantalla
    pinMode(11, INPUT);
    pinMode(13, INPUT);
    lcd.begin(16,2);
    lcd.clear();
    lcd.setBacklight(HIGH);
    lcd.setCursor(0,0);
    lcd.print("Esperando...");
    //comunicación serie
    Serial.begin(9600);
    while(!Serial.available()) {
        digitalWrite(12, HIGH);
        digitalWrite(A0, HIGH);
        digitalWrite(A1, HIGH);
        delay(50);
        digitalWrite(12, LOW);
        digitalWrite(A0, LOW);
        digitalWrite(A1, LOW);
        delay(50);
    }
    while(Serial.available()) Serial.read();
    Serial.flush();
    lcd.clear();
    //lcd.setBacklight(LOW);
}

void loop() {
    char c = 0;
    static unsigned char stage = 0; //0 esperar cabecera; 1 cabecera recibida; 2 final de texto
    static unsigned char pos = 0;
    static unsigned char kind = 0; //0 desconocido; 1 texto; 2 orden
    static String header;
    static String received;

    if (Serial.available()) {
        c = Serial.read();
        if (isAscii(c)) {
            //identificación de fin de línea `CR`
            if (c == 13) {
                stage = STG_LINE_OK;
                received.trim();
                printIncoming(received, kind);
                if (kind == LN_ORDER) parseOrder(received);
                header = "";
                received = "";
                stage = STG_WAITING;
                kind = LN_UNKNOWN;
                pos = 0;
            }
            //identificación de cabecera
            else if ((c == ' ') && (pos > 0) && (kind == LN_UNKNOWN) && (stage == STG_WAITING)) {
                header.toUpperCase();
                header.trim();
                if (header == HD_ORDER) {
                    kind = LN_ORDER;
                    received.trim();
                    received = received.substring(header.length());
                }
                else {
                    kind = LN_TEXT;
                    received += c;
                }
                stage = STG_HEADER_OK;
            }
            else {
                if (isAlphaNumeric(c) || isPunct(c)) {
                    pos++;
                }
                received += c; //caracteres payload
                if (kind == LN_UNKNOWN) header += c; //caracteres de cabecera
            }
        }
    }
}

void moveRobot(unsigned char course, unsigned char motor, unsigned char value, unsigned int distance) {
    switch (motor) {
      case LEFT:
        if (course == HALT) {
            digitalWrite(5, LOW);
            digitalWrite(10, LOW);
        }
        else if (course == FORWARD) {
            digitalWrite(10, LOW);
            analogWrite(5, value);
            delay(distance);
            digitalWrite(5, LOW);
        }
        else if (course == BACKWARD) {
            digitalWrite(5, LOW);
            analogWrite(10, value);
            delay(distance);
            digitalWrite(10, LOW);
        }
        break;
      case RIGHT:
        if (course == HALT) {
            digitalWrite(6, LOW);
            digitalWrite(9, LOW);
        }
        else if (course == FORWARD) {
            digitalWrite(6, LOW);
            analogWrite(9, value);
            delay(distance);
            digitalWrite(9, LOW);
        }
        else if (course == BACKWARD) {
            digitalWrite(9, LOW);
            analogWrite(6, value);
            delay(distance);
            digitalWrite(6, LOW);
        }
        break;
      case BOTH:
        if (course == HALT) {
            digitalWrite(5, LOW);
            digitalWrite(10, LOW);
            digitalWrite(6, LOW);
            digitalWrite(9, LOW);
        }
        else if (course == FORWARD) {
            digitalWrite(10, LOW);
            analogWrite(5, value);
            digitalWrite(6, LOW);
            analogWrite(9, value);
            delay(distance);
            digitalWrite(5, LOW);
            digitalWrite(9, LOW);
        }
        else if (course == BACKWARD) {
            digitalWrite(5, LOW);
            analogWrite(10, value);
            digitalWrite(9, LOW);
            analogWrite(6, value);
            delay(distance);
            digitalWrite(10, LOW);
            digitalWrite(6, LOW);
        }
        else if (course == TURN_LEFT) {
            digitalWrite(5, LOW);
            analogWrite(10, value);
            digitalWrite(6, LOW);
            analogWrite(9, value);
            delay(distance);
            digitalWrite(10, LOW);
            digitalWrite(9, LOW);
        }
        else if (course == TURN_RIGHT) {
            digitalWrite(10, LOW);
            analogWrite(5, value);
            digitalWrite(9, LOW);
            analogWrite(6, value);
            delay(distance);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
        }
    }
}

void printIncoming(String str, unsigned char type) {
    String line;
    
    if (type == LN_TEXT) {
        playEvent(LN_TEXT);
        if (str.length() > 16) {
            for (unsigned char i = 0; i < (str.length() - 16); i++) {
                lcd.clear();
                lcd.setCursor(0,0);
                lcd.print(USERNAME_SAYS);
                lcd.setCursor(0,1);
                line = str.substring(0+i,16+i);
                lcd.print(line);
                delay(250);
            }
        }
        else {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print(USERNAME_SAYS);
            lcd.setCursor(0,1);
            lcd.print(str);
        }
    }
    else if (type == LN_ORDER) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Orden recibida:");
        lcd.setCursor(0,1);
        lcd.print(str);
    }
    else if (type == LN_DONE) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Orden realizada:");
        lcd.setCursor(0,1);
        lcd.print(str);
    }
    else if (type == LN_ORDER_NCK) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("  Error: orden  ");
        lcd.setCursor(0,1);
        lcd.print("  desconocida.  ");
    }
}

void parseOrder(String order) {
    String upperOrder;
    String strDist;

    upperOrder = order;
    upperOrder.toUpperCase();
    if (upperOrder == TXT_STOP) {
        playEvent(LN_ORDER);
        moveRobot(HALT, BOTH, 0, 0);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_FORWARD) {
        playEvent(LN_ORDER);
        moveRobot(FORWARD, BOTH, 255, 1000);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_BACKWARD) {
        playEvent(LN_ORDER);
        moveRobot(BACKWARD, BOTH, 255, 1000);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_LEFT) {
        playEvent(LN_ORDER);
        moveRobot(TURN_LEFT, BOTH, 255, 1000);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_RIGHT) {
        playEvent(LN_ORDER);
        moveRobot(TURN_RIGHT, BOTH, 255, 1000);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_CLAXON) {
        playEvent(LN_ORDER);
        delay(250);
        playEvent(ORD_CLAXON);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_LON) {
        playEvent(LN_ORDER);
        delay(250);
        lcd.setBacklight(HIGH);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_LOFF) {
        playEvent(LN_ORDER);
        delay(250);
        lcd.setBacklight(LOW);
        printIncoming(order, LN_DONE);
        playEvent(LN_DONE);
    }
    else if (upperOrder == TXT_DIST) {
        playEvent(LN_ORDER);
        delay(250);
        strDist = (String)sonar.ping_cm();
        printIncoming(order + " " + strDist, LN_DONE);
        playEvent(LN_DONE);
    }
    else {
        printIncoming(order, LN_ORDER_NCK);
        playEvent(LN_ORDER_NCK);
    }
}

void playEvent(unsigned char type) {
    switch (type) {
      case LN_TEXT:
        tone(4, 880, 100);
        delay(100);
        tone(4, 1760, 100);
        delay(100);
        break;
      case LN_ORDER:
        tone(4, 440, 100);
        delay(100);
        break;
      case LN_ORDER_NCK:
        tone(4, 440, 50);
        delay(75);
        tone(4, 440, 50);
        delay(75);
        tone(4, 440, 50);
        delay(50);
        break;
      case LN_DONE:
        tone(4, 440, 100);
        delay(150);
        tone(4, 440, 100);
        delay(100);
        break;
      case ORD_CLAXON:
        tone(4, 2000, 500);
        delay(1000);
        break;
    }
    noTone(4);
}

