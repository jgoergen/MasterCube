// https://www.deviceplus.com/how-tos/arduino-guide/using-esp-wroom-02-wifi-module-as-arduino-mcu/
// https://www.instructables.com/id/How-to-Use-Wemos-ESP-Wroom-02-D1-Mini-WiFi-Module-/

// microcontroller board
// https://www.aliexpress.com/item/WeMos-D1-Esp-Wroom-02-Motherboard-ESP8266-Mini-WiFi-Nodemcu-Module-18650-Battery/32816415139.html?spm=a2g0s.9042311.0.0.34084c4duhOCVc

// mpu board
// https://www.aliexpress.com/item/GY-9250-MPU-9250-nine-axis-sensor-module-I2C-SPI-communication/32654909401.html?spm=a2g0s.9042311.0.0.34084c4d9Ymp32

// try this for gesture detection
// https://www.hackster.io/mellis/gesture-recognition-using-accelerometer-and-esp-71faa1

// esp library must be up to date!!
// wemos di mini!!
// https://www.aliexpress.com/item/32816415139.html?spm=a2g0s.9042311.0.0.27424c4d8Rc1Jr

/*
 board pinout
 D1 GPIO 5
 D2 GPIO 4
 D3 GPIO 0
 D4 GPIO 2 SDA
 D5 GPIO 14 SCL
 D6 GPIO 12 MISO
 D7 GPIO 13 MOSI
 D8 GPIO 15 CS
 A0 Tout
 GPIO 16 connected to led
 */

// ############ SETTINGS ############
#define MPU_UPDATE_RATE     16
#define MPU_READ_RATE       500
#define DISPLAY_RATE        30
#define GAME_UPDATE_RATE    30
#define RGB_LED_BRIGHTNESS  100
#define ROUNDS              5
#define TURNS               6
#define DRY_UPDATE_COUNT    50

#define TOP               0
#define BOTTOM            3
#define LEFT              1
#define RIGHT             5
#define FRONT             4
#define BACK              2
#define TOP_R             255
#define TOP_G             0
#define TOP_B             0
#define LEFT_R            0
#define LEFT_G            255
#define LEFT_B            0
#define RIGHT_R           0
#define RIGHT_G           0
#define RIGHT_B           255
#define FRONT_R           255
#define FRONT_G           0
#define FRONT_B           255
#define BACK_R            0
#define BACK_G            255
#define BACK_B            255
#define BOTTOM_R          255
#define BOTTOM_G          255
#define BOTTOM_B          255
// ##################################

#define NEOPIXEL_PIN      5
#define NEOPIXEL_COUNT    6
#define MPU_I2C_ADDRESS1  0x0C
#define MPU_I2C_Address2  0x68
#define MPU_SCL           14
#define MPU_SDA           2
#define ONBOARD_LED       16

int appState = 0;
int bootPhase = 0;
int dryUpdatesLeft = 0;

void setup()
{
  // intentionally left empty. 
  // ESP8266 / ESP32 chips don't like delays 
  // ( they will reset if anything takes too long before a yield / delay ) 
  // so I'm using a boot sequence instead.
}

void loop()
{
  switch(appState) {

    case 0:
      boot_Update();
      break;
      
    case 1: 
      mainUpdate();      
      break;
  }
}

void mainUpdate() {
  MPU_Update();

  if (dryUpdatesLeft > 0) {
    dryUpdatesLeft --;
    delay(10);
    return;
  }
  
  Game_Update();
  Display_update();
}

void boot_Update() {
  switch(bootPhase) {
    case 0: 
      Serial.begin(74880);
      Serial.println("init start");
      bootPhase = 1;
      break;

     case 1:
      pinMode(ONBOARD_LED, OUTPUT);
      digitalWrite(0, HIGH);
      bootPhase = 2;
      break;

     case 2:
      Display_Init();
      bootPhase = 3;
      break;

     case 3:
      MPU_Init();
      bootPhase = 4;
      break;
      
     case 4:      
      Game_Init();
      dryUpdatesLeft = DRY_UPDATE_COUNT;
      bootPhase = 5;
      appState = 1;
      break;
  }
}
