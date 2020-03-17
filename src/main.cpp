#include <ArduinoOTA.h>
#include <WiFi.h>

#define MAX_CHANNEL 8

const char *ssid = "hoge";
const char *password = "moge";

// pin assign
const int AIN1_UL = 33;
const int AIN2_UL = 32;
const int PWMA_UL = 25;
const int BIN1_DL = 27;
const int BIN2_DL = 26;

const int AIN1_UR = 13;
const int AIN2_UR = 12;
const int PWMA_UR = 23;
const int BIN1_DR = 4;
const int BIN2_DR = 2;

const int Trig1 = 14;
const int Trig2 = 15;
const int Trig3 = 16;
const int Trig4 = 17;

const int Echo1 = 18;
const int Echo2 = 19;
const int Echo3 = 21;
const int Echo4 = 22;

const int CHANNEL_0 = 0;
const int CHANNEL_1 = 1;
const int CHANNEL_2 = 2;
const int CHANNEL_3 = 3;
const int CHANNEL_4 = 4;
const int CHANNEL_5 = 5;
const int CHANNEL_6 = 6;
const int CHANNEL_7 = 7;

const int LEDC_TIMER_BIT = 8;       // PWM range(8bit:0〜255、10bit:0〜1023)
const int LEDC_BASE_FREQ = 150000;  // Frequency[Hz]
const int VALUE_MAX = 255;          // MAX PWM

enum e_action {
  FW,  // forward
  TR,  // turn right
  TL,  // turn left
  BK,  // back
  SR,  // sideways right
  SL,  // sideways left
  UR,  // upper right
  UL,  // upper left
  LR,  // lower right
  LL,  // lower left
  ST   // stop
};

void setPWM(e_action action) {
  int tgt_pwm[MAX_CHANNEL];
  int w;

  // const int wait_PWM = 1;   //for cancelling mobile battery's auto power off
  const int wait_PWM = 0;

  w = VALUE_MAX;
  switch (action) {
    case FW:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = w;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = w;
      tgt_pwm[4] = w;
      tgt_pwm[5] = 0;
      tgt_pwm[6] = w;
      tgt_pwm[7] = 0;
      break;
    case TL:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = w;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = w;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = w;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = w;
      break;
    case TR:
      tgt_pwm[0] = w;
      tgt_pwm[1] = 0;
      tgt_pwm[2] = w;
      tgt_pwm[3] = 0;
      tgt_pwm[4] = w;
      tgt_pwm[5] = 0;
      tgt_pwm[6] = w;
      tgt_pwm[7] = 0;
      break;
    case BK:
      tgt_pwm[0] = w;
      tgt_pwm[1] = 0;
      tgt_pwm[2] = w;
      tgt_pwm[3] = 0;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = w;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = w;
      break;
    case SR:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = w;
      tgt_pwm[2] = w;
      tgt_pwm[3] = 0;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = w;
      tgt_pwm[6] = w;
      tgt_pwm[7] = 0;
      break;
    case SL:
      tgt_pwm[0] = w;
      tgt_pwm[1] = 0;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = w;
      tgt_pwm[4] = w;
      tgt_pwm[5] = 0;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = w;
      break;
    case UR:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = w;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = 0;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = 0;
      tgt_pwm[6] = w;
      tgt_pwm[7] = 0;
      break;
    case UL:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = 0;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = w;
      tgt_pwm[4] = w;
      tgt_pwm[5] = 0;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = 0;
      break;
    case LR:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = 0;
      tgt_pwm[2] = w;
      tgt_pwm[3] = 0;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = w;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = 0;
      break;
    case LL:
      tgt_pwm[0] = w;
      tgt_pwm[1] = 0;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = 0;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = 0;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = w;
      break;
    case ST:
      tgt_pwm[0] = 0;
      tgt_pwm[1] = wait_PWM;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = wait_PWM;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = wait_PWM;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = wait_PWM;
      break;
    default:
      // Serial.println("stop");
      tgt_pwm[0] = 0;
      tgt_pwm[1] = wait_PWM;
      tgt_pwm[2] = 0;
      tgt_pwm[3] = wait_PWM;
      tgt_pwm[4] = 0;
      tgt_pwm[5] = wait_PWM;
      tgt_pwm[6] = 0;
      tgt_pwm[7] = wait_PWM;
      break;
  }

  for (int i = 0; i < MAX_CHANNEL; i++) {
    ledcWrite(i, tgt_pwm[i]);
  }

  delay(100);
}

WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(AIN1_UL, OUTPUT);
  pinMode(AIN2_UL, OUTPUT);
  pinMode(BIN1_DL, OUTPUT);
  pinMode(BIN2_DL, OUTPUT);
  pinMode(AIN1_UR, OUTPUT);
  pinMode(AIN2_UR, OUTPUT);
  pinMode(BIN1_DR, OUTPUT);
  pinMode(BIN2_DR, OUTPUT);

  pinMode(Trig1, OUTPUT);
  pinMode(Trig2, OUTPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Trig3, OUTPUT);
  pinMode(Echo1, INPUT);
  pinMode(Echo2, INPUT);
  pinMode(Echo3, INPUT);
  pinMode(Echo4, INPUT);

  ledcSetup(CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_4, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_5, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_6, LEDC_BASE_FREQ, LEDC_TIMER_BIT);
  ledcSetup(CHANNEL_7, LEDC_BASE_FREQ, LEDC_TIMER_BIT);

  ledcAttachPin(AIN1_UL, CHANNEL_0);
  ledcAttachPin(AIN2_UL, CHANNEL_1);
  ledcAttachPin(BIN1_DL, CHANNEL_2);
  ledcAttachPin(BIN2_DL, CHANNEL_3);
  ledcAttachPin(AIN1_UR, CHANNEL_4);
  ledcAttachPin(AIN2_UR, CHANNEL_5);
  ledcAttachPin(BIN1_DR, CHANNEL_6);
  ledcAttachPin(BIN2_DR, CHANNEL_7);

  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  ArduinoOTA
      .onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH)
          type = "sketch";
        else  // U_SPIFFS
          type = "filesystem";

        // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS
        // using SPIFFS.end()
        Serial.println("Start updating " + type);
      })
      .onEnd([]() { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
      })
      .onError([](ota_error_t error) {
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

  server.begin();
}

int value = 0;

void loop() {
  ArduinoOTA.handle();

  WiFiClient client = server.available();  // listen for incoming clients

  if (client) {                     // if you get a client,
    Serial.println("New Client.");  // print a message out the serial port
    String currentLine =
        "";  // make a String to hold incoming data from the client
    while (client.connected()) {  // loop while the client's connected
      if (client.available()) {   // if there's bytes to read from the client,
        char c = client.read();   // read a byte, then
        Serial.write(c);          // print it out the serial monitor
        if (c == '\n') {          // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a
          // row. that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200
            // OK) and a content-type so the client knows what's coming, then a
            // blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();

            // the content of the HTTP response follows the header:
            client.print("<html>");
            client.print("<head>");
            client.print("<meta charset=\"UTF-8\">");
            client.print(
                "<meta name=\"viewport\" "
                "content=\"width=device-width,initial-scale=1,minimum-scale="
                "1\">");
            client.print("<style>");
            client.print("table td{ padding:15px; }");
            client.print("</style>");
            client.print("</head>");
            client.print("<body>");
            client.print("<table>");
            client.print("<tbody align=\"center\">");
            client.print(
                "<tr><td><input type=\"button\" value=\"⇖\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/UL';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"⇑\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/FW';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"⇗\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/UR';\"></td>");
            client.print(
                "<tr><td><input type=\"button\" value=\"⇐\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/SL';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"■\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/ST';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"⇒\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/SR';\"></td>");
            client.print(
                "<tr><td><input type=\"button\" value=\"⇙\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/LL';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"⇓\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/BK';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"⇘\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/LR';\"></td>");
            client.print("</tbody>");
            client.print("</table>");
            client.print("<table>");
            client.print("<tbody align=\"center\">");
            client.print(
                "<tr><td><input type=\"button\" value=\"↻\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/TR';\"></td>");
            client.print(
                "<td><input type=\"button\" value=\"↺\" "
                "style=\"font-size:32px;\" "
                "onclick=\"location.href='/TL';\"></td>");
            client.print("</tbody>");
            client.print("</table>");
            client.print("</body>");
            client.print("</html>");
            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {  // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage
                                 // return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        if (currentLine.endsWith("GET /UL")) {
          setPWM(UL);
        }
        if (currentLine.endsWith("GET /FW")) {
          setPWM(FW);
        }
        if (currentLine.endsWith("GET /UR")) {
          setPWM(UR);
        }
        if (currentLine.endsWith("GET /SL")) {
          setPWM(SL);
        }
        if (currentLine.endsWith("GET /ST")) {
          setPWM(ST);
        }
        if (currentLine.endsWith("GET /SR")) {
          setPWM(SR);
        }
        if (currentLine.endsWith("GET /LL")) {
          setPWM(LL);
        }
        if (currentLine.endsWith("GET /BK")) {
          setPWM(BK);
        }
        if (currentLine.endsWith("GET /LR")) {
          setPWM(LR);
        }
        if (currentLine.endsWith("GET /TR")) {
          setPWM(TR);
        }
        if (currentLine.endsWith("GET /TL")) {
          setPWM(TL);
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
