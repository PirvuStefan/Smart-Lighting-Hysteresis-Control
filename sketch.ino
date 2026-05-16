#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- DEFINIȚII PINI ȘI PRAGURI ---
#define PIN_LDR        A0
#define PIN_LED        13
#define PIN_BUTON      2          // Buton spre GND, INPUT_PULLUP

#define PRAG_LOW       400        // Aprinde sub 400 (în mod NOAPTE)
#define PRAG_HIGH      700        // Stinge peste 700 (în mod NOAPTE)

#define DURATA_SPLASH  1500       // Durata mesaj "mod schimbat" (ms)


struct Proiect {
    int valoareLumina;
    bool ledActiv;
    bool modZi;       // false = NOAPTE, true = ZI
};


LiquidCrystal_I2C lcd (0x27 , 16 , 2);
Proiect dateProiect = {0, false, false};

// Stare splash
bool splashActiv = false;
unsigned long splashStartTime = 0;

// Pentru detectare front (apăsare nouă, nu menținere)
bool butonApasatAnterior = false;


void setup() {
    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_BUTON, INPUT_PULLUP);     
    Serial.begin(9600);

    lcd.begin(16, 2);
    lcd.print("Sistem LDR");
    delay(1000);
    lcd.clear();
}


void loop() {
    // 0. Buton - detectare apăsare nouă
    bool butonApasatAcum = (digitalRead(PIN_BUTON) == LOW);
    if (butonApasatAcum && !butonApasatAnterior) {
        // Tranziție neapăsat -> apăsat: schimbă modul
        dateProiect.modZi = !dateProiect.modZi;
        splashActiv = true;
        splashStartTime = millis();
    }
    butonApasatAnterior = butonApasatAcum;

    // 1. Citire
    dateProiect.valoareLumina = analogRead(PIN_LDR);

    // 2. Verifică expirarea splash-ului
    if (splashActiv && (millis() - splashStartTime >= DURATA_SPLASH)) {
        splashActiv = false;
        lcd.clear();   // pregătește LCD pentru afișaj normal
    }

    // 3. Logica histerezis (depinde de mod)
    controlLumina(&dateProiect);

    // 4. Afișare (splash sau normal)
    if (splashActiv) {
        afisareSplash(dateProiect);
    } else {
        afisareDate(dateProiect);
    }

    delay(200);
}


void controlLumina(Proiect *p) {
    if (!p->modZi) {
        // MOD NOAPTE: LED aprins pe întuneric
        if (!p->ledActiv && p->valoareLumina < PRAG_LOW) {
            p->ledActiv = true;
            digitalWrite(PIN_LED, HIGH);
        }
        else if (p->ledActiv && p->valoareLumina > PRAG_HIGH) {
            p->ledActiv = false;
            digitalWrite(PIN_LED, LOW);
        }
    } else {
        // MOD ZI: LED aprins pe lumină (comparații inversate)
        if (!p->ledActiv && p->valoareLumina > PRAG_HIGH) {
            p->ledActiv = true;
            digitalWrite(PIN_LED, HIGH);
        }
        else if (p->ledActiv && p->valoareLumina < PRAG_LOW) {
            p->ledActiv = false;
            digitalWrite(PIN_LED, LOW);
        }
    }
}


void afisareDate(Proiect p) {

    Serial.print("Mod: ");
    Serial.print(p.modZi ? "ZI" : "NOAPTE");
    Serial.print(" | Lumina: ");
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


// Afișaj 1.5s la schimbarea modului
void afisareSplash(Proiect p) {

    Serial.print("Mod: ");
    Serial.print(p.modZi ? "ZI" : "NOAPTE");
    Serial.print(" | Lumina: ");
    Serial.print(p.valoareLumina);
    Serial.print(" | LED: ");
    Serial.println(p.ledActiv ? "PORNIT" : "OPRIT");


    lcd.setCursor(0, 0);
    lcd.print("> MOD SCHIMBAT <");   // 16 caractere exact

    lcd.setCursor(0, 1);
    if (p.modZi) {
        lcd.print("Mod nou: ZI     ");    // 16 caractere
    } else {
        lcd.print("Mod nou: NOAPTE ");    // 16 caractere
    }
}
