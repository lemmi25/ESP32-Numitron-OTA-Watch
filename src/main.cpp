/* Example sketch using TLC591x library  to interface
   with the Texas Instruments TLC5916 and TLC5917
   8-Channel Constant-Current LED Sink Drivers.
   https://gitlab.com/Andy4495/TLC591x

*/
/* Version History
   1.0.0    08/06/2018  A.T.   Original
*/
#include <TLC591x.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <NTPClient.h>
#include <ArduinoOTA.h>

TLC591x seg1(2, 4, 2, 5);    // Tube 3,4 SDI,CLK,LE
TLC591x seg2(2, 13, 26, 25); // Tube 1,2 SDI,CLK,LE

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dateStamp;
String timeStamp;
int houre;
int minute;
int month;
int day;

const char *ssid = "<Your SSID>";
const char *password = "<Your PSW>";

const byte interruptPin = 15;
volatile bool interrupt = false;
int numberOfInterrupts = 0;

portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;

volatile unsigned long alteZeit = 0, entprellZeit = 1000;

void IRAM_ATTR handleInterrupt()
{

  if ((millis() - alteZeit) > entprellZeit)
  {
    portENTER_CRITICAL_ISR(&mux);
    alteZeit = millis();
    interrupt = true;
    Serial.println(interrupt);
    portEXIT_CRITICAL_ISR(&mux);
  }
}

void setup()
{
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), handleInterrupt, FALLING);

  Serial.begin(115200);
  Serial.println("Booting");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }
  // Hostname defaults to esp3232-[MAC]
  ArduinoOTA.setHostname("OTA ESP32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR)
      Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR)
      Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR)
      Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR)
      Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR)
      Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(7200);
}

void loop()
{
  ArduinoOTA.handle();
  while (!timeClient.update())
  {
    timeClient.forceUpdate();
  }

  // Extract date
  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  dateStamp = formattedDate.substring(0, splitT - 8);

  //Serial.println(interrupt);

  if (interrupt == true)
  {
    dateStamp = formattedDate.substring(5, splitT - 3);
    month = dateStamp.toInt();

    seg2.print(month);

    dateStamp = formattedDate.substring(8, splitT);
    day = dateStamp.toInt();

    seg1.print(day);

    delay(3000);

    interrupt = false;
  }

  // Extract time
  timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 7);
  houre = timeStamp.toInt();

  seg1.print(houre);

  timeStamp = formattedDate.substring(splitT + 4, formattedDate.length() - 4);
  minute = timeStamp.toInt();

  seg2.print(minute);
}
