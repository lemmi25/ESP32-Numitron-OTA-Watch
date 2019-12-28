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
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <ArduinoOTA.h>

TLC591x seg1(2, 4, 2, 5);    // Tube 3,4 SDI,CLK,LE
TLC591x seg2(2, 13, 26, 25); // Tube 1,2 SDI,CLK,LE

StaticJsonDocument<500> doc;
StaticJsonDocument<5000> docWeather;

HTTPClient http;
HTTPClient httpWeather;

char timeOld;
char timeCurrent;
bool enableTimeOld = false;

const char *ssid = "WLAN-164097";
const char *password = "4028408165188671";

void setTemp(int temperature, int forecastTime);
void setPressure(int pressure, int forecastTime);
int concatenate(int x, int y);

void setup()
{

  Serial.begin(115200);
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

  for (int i = 1; i < 99; i++)
  {
    seg1.print(i);
    seg2.print(i);
    delay(200);
  }
}

void loop()
{
  ArduinoOTA.handle();

  //Time

  http.begin("http://worldtimeapi.org/api/timezone/Europe/Berlin.json"); //Specify the URL
  int httpCodeTime = http.GET();

  if (httpCodeTime > 0)
  { //Check for the returning code

    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, http.getString());

    if (error)
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      delay(2000); //2sec
      return;
    }

    if (enableTimeOld == true)
    {
      timeOld = timeCurrent;
    }
    enableTimeOld = true;

    //Get Time
    const char *date = doc["datetime"]; //Get current time

    seg1.print(concatenate(date[11] - '0', date[12] - '0'));
    seg2.print(concatenate(date[14] - '0', date[15] - '0'));
    vTaskDelay(2000); //2sec
  }

  else
  {
    Serial.println("Error on HTTP request Time");
  }

  //Temperature & Pressure
  if (timeOld != timeCurrent)
  {
    httpWeather.begin("http://api.openweathermap.org/data/2.5/forecast?q=Oldenburg,de&cnt=2&units=metric&appid=03e2fbe874af4836c6bf932b697a809b");
    int httpCodeWeather = httpWeather.GET();

    if (httpCodeWeather > 0)
    { //Check for the returning code

      DeserializationError errorWeather = deserializeJson(docWeather, httpWeather.getString());

      if (errorWeather)
      {
        Serial.print(F("deserializeJson() from weather failed: "));
        Serial.println(errorWeather.c_str());
        vTaskDelay(2000); //2sec
      }

      const int tempTime1 = docWeather["list"][0]["main"]["temp"]; //Get current time forecast
      //const int pressure1 = docWeather["list"][0]["main"]["pressure"]; //Get current time forecast
      setTemp(tempTime1, 3);
      delay(5000); //4sec
      //setPressure(pressure1, 3);
      //delay(5000); //4sec

      const int tempTime2 = docWeather["list"][1]["main"]["temp"]; //Get current time
      //const int pressure2 = docWeather["list"][1]["main"]["pressure"]; //Get current time forecast
      setTemp(tempTime2, 6);
      delay(5000); //4sec
                   /*       setTemp(tempTime2, 6);
      
      setPressure(pressure2, 3);
      delay(5000); //4sec */
    }
    else
    {
      Serial.println("Error on HTTP request Date");
    }
  }
  else
  {
    return;
  }
}

int getdigit(int num, int n)
{
  int r, t1, t2;

  t1 = pow(10, n + 1);
  r = num % t1;

  if (n > 0)
  {
    t2 = pow(10, n);
    r = r / t2;
  }

  return r;
}

int concatenate(int x, int y)
{
  int pow = 10;
  while (y >= pow)
    pow *= 10;
  return x * pow + y;
}

/* void setPressure(int pressure, int forecastTime)
{
  NixiClock.writeSegment(getdigit(pressure, 3), 1);
  NixiClock.writeSegment(getdigit(pressure, 2), 2);
  NixiClock.writeSegment(getdigit(pressure, 1), 3);
  NixiClock.writeSegment(getdigit(pressure, 0), 4);
} */

void setTemp(int temperature, int forecastTime)
{
  if (temperature < 10 && temperature > 0)
  {
    seg1.print(concatenate(0, forecastTime));
    seg2.print(concatenate(0, temperature));
    //Serial.println("Between 0 and 10");
  }
  else if (temperature > 9)
  {
    seg1.print(concatenate(0, forecastTime));
    seg2.print(temperature);
    //Serial.println("above 10");
  }
  else if (temperature > -10 && temperature < 0)
  {

    seg1.print(concatenate(1, forecastTime));
    seg2.print(concatenate(0, abs(temperature)));

    //Serial.println("between -10 and 0");
  }
  else if (temperature < -9)
  {

    seg1.print(concatenate(1, forecastTime));
    seg2.print(abs(temperature));
    //Serial.println("below -9");
  }
  else
  {
    return;
  }
}
