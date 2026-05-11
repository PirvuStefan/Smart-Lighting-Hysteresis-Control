#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- DEFINIȚII PINI ȘI PRAGURI ---
#define PIN_RS         12
#define PIN_EN         11
#define PIN_D4         5
#define PIN_D5         4
#define PIN_D6         3
#define PIN_D7         2
#define PIN_LDR        A0
#define PIN_LED        13

#define PRAG_LOW       400  // Aprinde sub 400
#define PRAG_HIGH      700  // Stinge peste 700


struct Proiect {
    int valoareLumina;
    bool ledActiv;
};



LiquidCrystal_I2C lcd (0x27 , 16 , 2);
Proiect dateProiect = {0, false};

void setup() {
    pinMode(PIN_LED, OUTPUT);
    Serial.begin(9600);


    lcd.begin(16, 2);
    lcd.print("Sistem LDR");
    delay(1000);
    lcd.clear();
}

void loop() {
    // 1. Citire
    dateProiect.valoareLumina = analogRead(PIN_LDR);

    // 2. Logica Histeris
    controlLumina(&dateProiect);

    // 3. Afișare (Serial + LCD)
    afisareDate(dateProiect);

    delay(200);
}



void controlLumina(Proiect *p) {
    if (!p->ledActiv && p->valoareLumina < PRAG_LOW) {
        p->ledActiv = true;
        digitalWrite(PIN_LED, HIGH);
    }
    else if (p->ledActiv && p->valoareLumina > PRAG_HIGH) {
        p->ledActiv = false;
        digitalWrite(PIN_LED, LOW);
    }
}

void afisareDate(Proiect p) {

    Serial.print("Lumina: ");
    Serial.print(p.valoareLumina);
    Serial.print(" | LED: ");
    Serial.println(p.ledActiv ? "PORNIT" : "OPRIT");


    lcd.setCursor(0, 0);
    lcd.print("Lumina: ");
    lcd.print(p.valoareLumina);
    lcd.print("    ");

    lcd.setCursor(0, 1);
    lcd.print("LED: ");
    lcd.print(p.ledActiv ? "PORNIT " : "OPRIT  ");
}