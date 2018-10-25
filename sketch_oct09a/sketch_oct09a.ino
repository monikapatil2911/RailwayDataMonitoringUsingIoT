#include <SPI.h>//include the SPI bus library
#include <MFRC522.h>//include the RFID reader library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
const char* ssid     = "Gulshan";
const char* password = "gulshan12";
//const char* server = "192.168.43.209";
#define SS_PIN 15 //slave select pin
#define RST_PIN 5  //reset pin
MFRC522 mfrc522(SS_PIN, RST_PIN);        // instatiate a MFRC522 reader object.
MFRC522::MIFARE_Key key;//create a MIFARE_Key struct named 'key', which will hold the card information
ESP8266WebServer server(80);
WiFiClient client;
MDNSResponder mdns;
float humidity, temp_f;  // Values read from sensor
String webPage = "";
int f =0;
void setup(void) {
  webPage += "<h1>ESP8266 Web Server</h1><p>Socket #1 <a href=\"socket1On\"><button>ON</button></a>&nbsp;<a href=\"socket1Off\"><button>OFF</button></a></p>";
  webPage += "<p>Socket #2 <a href=\"socket2On\"><button>ON</button></a>&nbsp;<a href=\"socket2Off\"><button>OFF</button></a></p>";
  Serial.begin(115200);        // Initialize serial communications with the PC
        SPI.begin();               // Init SPI bus
          delay(1000);
 Serial.println("Hii ");
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.print("\n\r \n\r Wifi begin(ssid, password)  Working to connect");
 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("Connected to --->>> ");
  Serial.println(ssid);
  Serial.print("IP address: --->>> ");
  Serial.println(WiFi.localIP());
  if (mdns.begin("esp8266", WiFi.localIP())) {
    Serial.println(" ======== MDNS responder started ========");
  }
        mfrc522.PCD_Init();        // Init MFRC522 card (in case you wonder what PCD means: proximity coupling device)
        Serial.println("Scan a MIFARE Classic card");
        
        // Prepare the security key for the read and write functions - all six key bytes are set to 0xFF at chip delivery from the factory.
        // Since the cards in the kit are new and the keys were never defined, they are 0xFF
        // if we had a card that was programmed by someone else, we would need to know the key to be able to access it. This key would then need to be stored in 'key' instead.
 
        for (byte i = 0; i < 6; i++) {
                key.keyByte[i] = 0xFF;//keyByte is defined in the "MIFARE_Key" 'struct' definition in the .h file of the library
        }
    server.on("/", [](){
    server.send(200, "text/html", webPage);
  });
  server.on("/socket1On", [](){
    server.send(200, "text/html", webPage);
    //digitalWrite(gpio0_pin, HIGH);
    delay(1000);
  });
  server.on("/socket1Off", [](){
    server.send(200, "text/html", webPage);
    //digitalWrite(gpio0_pin, LOW);
    delay(1000); 
  });
  server.on("/socket2On", [](){
    server.send(200, "text/html", webPage);
   // digitalWrite(gpio2_pin, HIGH);
    delay(1000);
  });
  server.on("/socket2Off", [](){
    server.send(200, "text/html", webPage);
    //digitalWrite(gpio2_pin, LOW);
    delay(1000); 
  });
  server.begin();
  Serial.println("HTTP server started");


  delay(10000);
  
  
}
int block=2;//this is the block number we will write into and then read. Do not write into 'sector trailer' block, since this can make the block unusable.
int block1=1;  
int block2=4;
byte readbackblock[18];//This array is used for reading out a block. The MIFARE_Read method requires a buffer that is at least 18 bytes to hold the 16 bytes of a block.
byte readbackblock1[18];
byte readbackblock2[18];
char a[18];
char b[18];
char c[18];
void loop() {
  if ( ! mfrc522.PICC_IsNewCardPresent()) {//if PICC_IsNewCardPresent returns 1, a new card has been found and we continue
    return;//if it did not find a new card is returns a '0' and we return to the start of the loop
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {//if PICC_ReadCardSerial returns 1, the "uid" struct (see MFRC522.h lines 238-45)) contains the ID of the read card.
    return;//if it returns a '0' something went wrong and we return to the start of the loop
  }

         Serial.println("card selected");
         
        
         readBlock(block, readbackblock);//read the block back
         Serial.print("read block: ");
         for (int j=0 ; j<16 ; j++)//print the block contents
         {
         a[j]=readbackblock[j];//Serial.write() transmits the ASCII numbers as human readable characters to serial monitor
         
         }
         
         
         Serial.println("");
         readBlock(block1, readbackblock1);
         Serial.print("read block: ");
         for (int j=0 ; j<16 ; j++)//print the block contents
         {
          b[j]=readbackblock1[j];//Serial.write() transmits the ASCII numbers as human readable characters to serial monitor
         }
         Serial.println("");
         readBlock(block2, readbackblock2);
         Serial.print("read block: ");
         for (int j=0 ; j<16 ; j++)//print the block contents
         {
          c[j]=readbackblock2[j];//Serial.write() transmits the ASCII numbers as human readable characters to serial monitor
         }
         Serial.println("");
         Serial.println(a);
         Serial.println(b);
         Serial.println(c);
   delay(100);

//  name();
//  source();
f = f+1 ;
  temp_f = random(200);//analogRead(A0);
  Serial.println( String(a)+", 1");
  
    server.handleClient();
    if ( f <3 ){Serial.println("\nStarting connection to server..."); }
 
  Serial.print("hii");

  if (client.connect("192.168.43.209", 80)) {//("192.168.0.106", 80)  <========= [*** SIRF ISE CHANGE KARANA 'IPCONFIG' SE ***]
    Serial.print("abc");
    if ( f <3 ){Serial.println("connected to server");}
  client.print("GET /iot/name.php?"); // This
    client.print("value="); 
    client.print(a); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 127.0.0.1"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here 
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server
    
    }
    else {
    // If Arduino can't connect to the server (your computer or web page)
    Serial.println("--> connection failed\n");
  }
}
int readBlock(int blockNumber, byte arrayAddress[]) 
{
  int largestModulo4Number=blockNumber/4*4;
  int trailerBlock=largestModulo4Number+3;
  MFRC522::StatusCode status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
         Serial.print("PCD_Authenticate() failed (read): ");
         Serial.println(mfrc522.GetStatusCodeName(status));
         return 3;//return "3" as error message
  }
byte buffersize = 18;//we need to define a variable with the read buffer size, since the MIFARE_Read method below needs a pointer to the variable that contains the size... 
  status = mfrc522.MIFARE_Read(blockNumber, arrayAddress, &buffersize);//&buffersize is a pointer to the buffersize variable; MIFARE_Read requires a pointer instead of just a number
  if (status != MFRC522::STATUS_OK) {
          Serial.print("MIFARE_read() failed: ");
          Serial.println(mfrc522.GetStatusCodeName(status));
          return 4;//return "4" as error message
  }
  Serial.println("block was read");
}
