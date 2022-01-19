#include <SPI.h>
#include <ArduinoWebsockets.h>
#include <WiFi.h>
#include <TJpg_Decoder.h>
#include <TFT_eSPI.h>

#define YPOS_MAX 0
#define YPOS_MIN -105
#define MOVE_FORCE 20

const char* ssid = "St7789_Display";
const char* password = "12345678";

using namespace websockets;
WebsocketsServer server;
WebsocketsClient client;

TFT_eSPI tft = TFT_eSPI();


int yPos = -52;

void setup() {
  Serial.begin(115200);

  tft.begin();
  tft.setRotation(1);
  tft.setTextColor(TFT_WHITE,TFT_BLACK); 
  tft.fillScreen(TFT_BLACK);
  tft.setSwapBytes(true);
  tft.setTextFont(4);
  TJpgDec.setJpgScale(1);
  TJpgDec.setCallback(tft_output);
  
  tft.println("> Setting AP...");
  WiFi.softAP(ssid, password);

  IPAddress IP = WiFi.softAPIP();
  tft.println("> AP IP Address");
  tft.print(">" ); 
  tft.println(IP);
  tft.println("> Waiting for");
  tft.println("> ESP32CAM");

  server.listen(8888);
}

void loop() {

  
  if(server.poll()){
    client = server.accept();
    }

  if(client.available()){
    client.poll();
    tft.fillScreen(TFT_BLACK);
    while (true){
    WebsocketsMessage msg = client.readBlocking();
    
    
    uint32_t t = millis();
    
    TJpgDec.drawJpg(-40, yPos, (const uint8_t*)msg.c_str(), msg.length());
    
    t = millis() - t;
    Serial.print(t); Serial.println(" ms");
    }
  }
}

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap){
  if ( y >= tft.height() ) return 0;
  tft.pushImage(x, y, w, h, bitmap);
  return 1;
}
