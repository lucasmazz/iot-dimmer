#include <Arduino.h>
#include <WiFi.h>

#define INPUT_PIN 12
#define OUTPUT_PIN 14
#define FREQUENCY 60

// WiFi SSID and PASSWORD.
const char* SSID = "CHANGE TO YOUR SSID";
const char* PASSWORD = "CHANGE TO YOUR PASSWORD";

// Control flags and variables.
uint8_t brightness = 0;
uint32_t last_time = 0; 
uint32_t  zero_crossing_time = 0;
double_t trigger_time = -1;
bool is_crossing_zero = 0;

WiFiServer server(80); 


void webServer(void *arg) 
{
  while(1) 
  {	
    WiFiClient client = server.available();   

    if (client) 
    {                         
      // Holds incoming data.
      String current_line = "";  

      while (client.connected()) 
      {            
        if (client.available()) 
        {             
          // Reads bytes from the client.
          char c = client.read();

          // If it is a newline character.
          if (c == '\n') 
          {                    
            if (current_line.length() == 0) 
            {
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: text/plain");
              client.println();
              client.print(String(brightness));
              break;
            } 
            
            else 
            {    
              // Clears current_line if got a newline.
              current_line = "";
            }
          } 
          
          // If it is a carriage return character.
          else if (c != '\r') 
          {  
            current_line += c; 
          } 
          
          // Searches for brightness parameter on the GET request.
          else if (current_line.indexOf("GET") > -1 && current_line.indexOf("?brightness=") > -1) 
          {
            String value = current_line.substring(current_line.indexOf("?brightness=")+12, current_line.indexOf("HTTP")-1);
            
            // Sets the brightness value.
            if (value.toInt() >= 0 && value.toInt() <= 100)
            {
              brightness = value.toInt();
            } 
            
            else if (value.toInt() < 0)
            {
              brightness = 0;
            } 

            else 
            {
              brightness = 100;
            }
          }
        }

        delay(1);
      }

      client.stop();
    }

    delay(1);
  }
}


void setup() 
{ 
  // Sets the output trigger pin, used to trigger the TRIAC gate. 
  pinMode(OUTPUT_PIN, OUTPUT);
  
  // Sets the input pin to detect the zero crossing.
  pinMode(INPUT_PIN, INPUT);

  // Starts the serial communication.
  Serial.begin(9600);
  Serial.print("Starting...");

  // Connects to the Wi-Fi network.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(SSID);

  WiFi.begin(SSID, PASSWORD);

  // Waits to connect to Wi-Fi.
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  // If everything went well, prints the local IP.
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  // Runs the web server on the the other core in parallel.
  xTaskCreatePinnedToCore(
    webServer, 
    "TaskOnPro", 
    2048, 
    NULL, 
    8, 
    NULL, 
    PRO_CPU_NUM
  );
}


void loop()
{
  bool input_status = digitalRead(INPUT_PIN);
  uint32_t current_time = micros();
  
  // Waveform starts to fall towards zero, before zero-crossing.
  if (input_status && !is_crossing_zero)
  {
    // Calculates the time that will be used to trigger the TRIAC gate.
    trigger_time = (1 - (double_t)brightness/100)*(1000000/(2*FREQUENCY)) + zero_crossing_time;
    last_time = current_time; 
    is_crossing_zero = 1;
  } 

  // Waveform starts to rise, after zero crossing. 
  else if (!input_status && is_crossing_zero)
  {
    zero_crossing_time = (current_time - last_time)/2;
    is_crossing_zero = 0;
  }

  // Triggers TRIAC on the right time.
  if (trigger_time >= 0 && (current_time - last_time) >= trigger_time) 
  {	
    // Applies a small pulse.
    digitalWrite(OUTPUT_PIN, HIGH);
    delay(0.1);
    digitalWrite(OUTPUT_PIN, LOW);

    // Has been triggered, sets trigger time to -1.
    trigger_time = -1;
  } 
}