// HardwareSerial1.ino: Read PMS7003 sensor on Serial1
//
// Modified by Soldered for use with PMS7003 board here https://solde.red/333058

#include "PMS7003-SOLDERED.h" // Arduino library for PM sensors with serial interface


// Depending on board write different message and different serial
#if defined(USE_HWSERIAL2)
#define MSG "PMSx003 on HardwareSerial2"
SerialPM pms(PMSx003, Serial2); // PMSx003, UART
#elif defined(USE_HWSERIAL1)
#define MSG "PMSx003 on HardwareSerial1"
SerialPM pms(PMSx003, Serial1); // PMSx003, UART
#else
#define MSG "PMSx003 on HardwareSerial"
SerialPM pms(PMSx003, Serial); // PMSx003, UART
#endif

void setup()
{
    // Initiate Serial
    Serial.begin(115200);
    Serial.println(F("Booted"));

    // Initiate PMS7003
    Serial.println(F(MSG));
    pms.begin();
}

void loop()
{
    // read the PM sensor
    pms.read();
    if (pms)
    { // successfull read
        Serial.print(F("PM1.0 "));
        Serial.print(pms.pm01);
        Serial.print(F(", "));
        Serial.print(F("PM2.5 "));
        Serial.print(pms.pm25);
        Serial.print(F(", "));
        Serial.print(F("PM10 "));
        Serial.print(pms.pm10);
        Serial.println(F(" [ug/m3]"));

        if (pms.has_number_concentration())
        {
            Serial.print(F("N0.3 "));
            Serial.print(pms.n0p3);
            Serial.print(F(", "));
            Serial.print(F("N0.5 "));
            Serial.print(pms.n0p5);
            Serial.print(F(", "));
            Serial.print(F("N1.0 "));
            Serial.print(pms.n1p0);
            Serial.print(F(", "));
            Serial.print(F("N2.5 "));
            Serial.print(pms.n2p5);
            Serial.print(F(", "));
            Serial.print(F("N5.0 "));
            Serial.print(pms.n5p0);
            Serial.print(F(", "));
            Serial.print(F("N10 "));
            Serial.print(pms.n10p0);
            Serial.println(F(" [#/100cc]"));
        }

        if (pms.has_temperature_humidity() || pms.has_formaldehyde())
        {
            Serial.print(pms.temp, 1);
            Serial.print(F(" °C"));
            Serial.print(F(", "));
            Serial.print(pms.rhum, 1);
            Serial.print(F(" %rh"));
            Serial.print(F(", "));
            Serial.print(pms.hcho, 2);
            Serial.println(F(" mg/m3 HCHO"));
        }
    }
    else
    { // something went wrong
        switch (pms.status)
        {
        case pms.OK: // should never come here
            break;   // included to compile without warnings
        case pms.ERROR_TIMEOUT:
            Serial.println(F(PMS_ERROR_TIMEOUT));
            break;
        case pms.ERROR_MSG_UNKNOWN:
            Serial.println(F(PMS_ERROR_MSG_UNKNOWN));
            break;
        case pms.ERROR_MSG_HEADER:
            Serial.println(F(PMS_ERROR_MSG_HEADER));
            break;
        case pms.ERROR_MSG_BODY:
            Serial.println(F(PMS_ERROR_MSG_BODY));
            break;
        case pms.ERROR_MSG_START:
            Serial.println(F(PMS_ERROR_MSG_START));
            break;
        case pms.ERROR_MSG_LENGTH:
            Serial.println(F(PMS_ERROR_MSG_LENGTH));
            break;
        case pms.ERROR_MSG_CKSUM:
            Serial.println(F(PMS_ERROR_MSG_CKSUM));
            break;
        case pms.ERROR_PMS_TYPE:
            Serial.println(F(PMS_ERROR_PMS_TYPE));
            break;
        }
    }

    // wait for 10 seconds
    delay(10000);
}
