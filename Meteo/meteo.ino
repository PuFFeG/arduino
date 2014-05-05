// ========================Задаем необходимые библиотеки==========
#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>
// ===============================================================
// ========================Задаем данные сети======================
byte mac[] = { 0xCA, 0xAF, 0x78, 0x1C, 0x13, 0x77 }; //mac - адрес ethernet shielda
byte ip[] = { 192, 168, 1, 33 };    	// ip адрес ethernet shielda
byte subnet[] = { 255, 255, 255, 0 }; //маска подсети
EthernetServer server(80); //порт сервера
int ledPin = 6; // указываем что светодиод будет управляться через 4 Pin
String readString = String(30); //string for fetching data from address
boolean LEDON = false; //изначальный статус светодиода - выключен
// ===============================================================
#define DHTPIN 2	// пин для датчика DHT22
#define DHTPIN1 3	// пин для датчика DHT22
#define DHTPIN2 4	// пин для датчика DHT22
const unsigned char OSS = 0;  // Oversampling Setting
// ===============================================================

// ==============================================================
DHT dht(DHTPIN, DHT11);
DHT dht1(DHTPIN1, DHT11);
DHT dht2(DHTPIN2, DHT11);
// ========================СТАРТУЕМ=============================
// ========================Управляем св.диодом на 4-м пине==========
void setup(){
  //запускаем Ethernet
  Ethernet.begin(mac, ip, subnet);
  //устанавливаем pin 6 на выход
  pinMode(ledPin, OUTPUT);
  //enable serial datada print
  Serial.begin(9600);
  	Serial.println("Port Test!"); // Тестовые строки для отображения в мониторе порта
  	Serial.println("GO!");// Тестовые строки для отображения в мониторе порта
  dht.begin();
  dht1.begin();
  dht2.begin();

}
// ==============================================================
void loop(){
  // ========================ДАТЧИКИ=============================
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();

  if (isnan(t) || isnan(h)) {
	Serial.println("Failed to read from DHT");
  } else {
  	Serial.print("H=: ");
	Serial.print(h);
  	Serial.print(" %\t");
  	Serial.print("Temp.=: ");
	Serial.print(t);
  	Serial.println(" *C");
    	Serial.println("Temp2 ======= ");
      	Serial.print("H2=: ");
	Serial.print(h1);
  	Serial.print(" %\t");
  	Serial.print("Temp2.=: ");
	Serial.print(t1);
  	Serial.println(" *C");
    	Serial.println("Temp3 =\\\\\\\\\\ ");
      	Serial.print("H3=: ");
	Serial.print(h2);
  	Serial.print(" %\t");
  	Serial.print("Temp3.=: ");
	Serial.print(t2);
  	Serial.println(" *C");
 // ==============================================================
   // =============Создаем клиентское соединение====================
  EthernetClient client = server.available();
  if (client) {
	while (client.connected()) {
	if (client.available()) {
  	char c = client.read();
  	//read char by char HTTP request
  	if (readString.length() < 30) {
    	//store characters to string
    	readString.concat( c); }
    	//output chars to serial port
    	Serial.print( c);
    	//if HTTP request has ended
    	if (c == '\n') {
    	//Проверяем включили ли светодиод?
    	//Level=1 - включен
    	//Level=0 - выключен
      	if(readString.indexOf("Level=1") >=0) {
        	//Включаем светодиод
        	digitalWrite(ledPin, HIGH); // set the LED on
        	LEDON = true;
      	}else{
        	//Выключаем светодиод
        	digitalWrite(ledPin, LOW); // set the LED OFF
        	LEDON = false;
      	}	 
   // =============Формируем HTML-страницу==========================
      	client.println("HTTP/1.1 200 OK");
      	client.println("Content-Type: text/html");
      	client.println();
      	client.println("<head> ");
      	client.println("<meta http-equiv='Content-Type' content='text/html; charset=utf-8' /> ");
      	client.println("<title> :: Погода в ШАХТЕ :: </title>");
      	client.println("</head> ");
      	client.println("<body");
      	client.println("<hr />");
      	client.println("<h1> ::HELL METEO:: </h1>");
      	if (LEDON){
        	client.println("<form method=get name=LED><input type=radio name=Level value=1 CHECKED>Включить<input type=radio name=Level value=0>Выключить<input type=submit value=OK></form>");
        	client.println("<font size=’5′>Satan-Lamp: ");
        	client.println("<font size=’5′>On.");
      	}else{
        	client.println("<form method=get name=LED><input type=radio name=Level value=1>Включить<input type=radio name=Level value=0 CHECKED>Выключить<input type=submit value=OK></form>");
        	client.println("<font size=’5′>Satan-Lamp: ");
        	client.println("<font size=’5′>Off");
      	}
     	//==============Вывод значений на web-страницу======================
          	client.println("<hr />");//линия=====================================
        	client.println("Tемпература = ");	//Температура с DHT 22
        	client.println(t);
        	client.println(" *C");
          	client.println("<br> "); //перенос на след. строчку
        	client.println("Влажность = ");	//Влажность с DHT 22
        	client.println(h);
        	client.println(" %\t");
          	client.println("<br> "); //перенос на след. строчку
              	client.println("Tемпература = ");	//Температура 2
        	client.println(t1);
               	client.println(" *C");
          	client.println("<br> "); //перенос на след. строчку
        	client.println("Влажность = ");	//Влажность с DHT 22
        	client.println(h1);
        	client.println(" %\t");
          	client.println("<br> "); //перенос на след. строчку
                client.println("Tемпература = ");	//Температура 2
        	client.println(t2);
               	client.println(" *C");
          	client.println("<br> "); //перенос на след. строчку
        	client.println("Влажность = ");	//Влажность с DHT 22
        	client.println(h2);
        	client.println(" %\t");
          	client.println("<br> "); //перенос на след. строчку

          	client.println("<hr />");//линия=====================================
        	//==============================================================
        	client.println("</body></html>");
        	//очищаем строку для следующего считывания 	 
        	//==============Останавливаем web-client===========================
        	readString="";
        	client.stop();
        	//==============================================================
      	}
    	}
  	}
 	}
 }
 }
