/* Bootstrap Interface for ESP8266 projects
  * Display DHT11
 * Use GPIO from Web Interface (use Leds to simulate for example)
 * 
 * Libraries used :WiFiClient, ESP8266WebServer, DHT

 * Licence : MIT
 * baseado neste código https://github.com/projetsdiy/ESP8266WebServer-Bootstrap-Bootswatch
*/

#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHTesp.h> //https://github.com/beegee-tokyo/DHTesp

//variaveis do router
#define ssid      "Tamanduai"       // WiFi SSID
#define password  "semsenha"   // WiFi password


// variaveis GPIO
const uint8_t GPIOPIN[4] = {D5, D6, D7, D8}; // Led array
String  etatGpio[4] = {"OFF", "OFF", "OFF", "OFF"}; // inicia todos os pins e

//variais de leitura dos sensores
DHTesp dht;
float   t = 0 ;               //varivaveis que armazenam temporariamente a ultima leitura do sensor
float   h = 0 ;               //varivaveis que armazenam temporariamente a ultima leitura do sensor

//variaveis do servidor
ESP8266WebServer server ( 80 ); // porta local do servidor

IPAddress ip(192, 168, 1, 116); //NodeMCU static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

String buildWebsite() { //pagina home html
  
  String webSite = "<html lang='pt-br'><head>"; // <meta name="viewport" content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no'>
  webSite += "<meta charset='UTF-8'> <meta http-equiv='refresh' content='60' width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no'/>";
  webSite += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  webSite += "<title>ESP8266 Demo - www.projetsdiy.fr</title></head><body>";
  webSite += "<div class='container-fluid'>";
  webSite +=   "<div class='row'>";
  webSite +=     "<div class='col-md-12'>";
  webSite +=       "<h1>Demo Webserver ESP8266 + Bootstrap</h1>";
  webSite +=       "<h3>Mini estação Metereologica</h3>";
  webSite +=       "<ul class='nav nav-pills'>";
  webSite +=         "<li class='active'>";
  webSite +=           "<a href='#'> <span class='badge pull-right'>";
  webSite +=           t;
  webSite +=           "</span> Temperatura</a>";
  webSite +=         "</li><li>";
  webSite +=           "<a href='#'> <span class='badge pull-right'>";
  webSite +=           h;
  webSite +=           "</span>Umidade</a>";
  webSite +=         "</li>";
  webSite +=       "</ul>";
  webSite +=       "<table class='table'>";  // Tableau des relevés
  webSite +=         "<thead><tr><th>Sensor</th><th>Medição</th><th>Leitura</th><th>Leitura anterior</th></tr></thead>"; //Entrada
  webSite +=         "<tbody>";  // Conteudo da tabela
  webSite +=           "<tr><td>DHT11</td><td>Temp</td><td>"; // primeira linha exibe a  temperatura
  webSite +=             t;
  webSite +=             "&deg;C</td><td>";
  webSite +=             "-</td></tr>";
  webSite +=           "<tr class='active'><td>DHT11</td><td>Umid</td><td>"; // primeira linha exibe a  umidade
  webSite +=             h;
  webSite +=             "%</td><td>";
  webSite +=             "-</td></tr>";
  webSite +=       "</tbody></table>";
  webSite +=       "<h3>GPIO</h3>"; // botões
  webSite +=       "<div class='row'>";
  webSite +=         "<div class='col-md-4'><h4 class ='text-left'>D5 ";
  webSite +=           "<span class='badge'>";
  webSite +=           etatGpio[0]; //Array botão D5 exibe estatus do pino
  webSite +=         "</span></h4></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D5' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D5' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  webSite +=         "<div class='col-md-4'><h4 class ='text-left'>D6 ";
  webSite +=           "<span class='badge'>";
  webSite +=           etatGpio[1]; //Array botão D6 exibe estatus do pino
  webSite +=         "</span></h4></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D6' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D6' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  webSite +=         "<div class='col-md-4'><h4 class ='text-left'>D7 ";
  webSite +=           "<span class='badge'>";
  webSite +=           etatGpio[2]; //Array botão D7 exibe estatus do pino
  webSite +=         "</span></h4></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D7' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D7' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  webSite +=         "<div class='col-md-4'><h4 class ='text-left'>D8 ";
  webSite +=           "<span class='badge'>";
  webSite +=           etatGpio[3]; //Array botão D8 exibe estatus do pino
  webSite +=         "</span></h4></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D8' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  webSite +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='D8' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  webSite +=       "</div>";
  // webSite +=     "<br><p><a href='http://www.projetsdiy.fr'>www.projetsdiy.fr</p>
  webSite += "</div></div></div>";
  webSite += "</body></html>";
  return webSite;
}
void updateGPIO(int gpio, String DxValue) { // função de alteração do status do pino (pino,status)
  Serial.println("");
  Serial.println("Update GPIO "); Serial.print(GPIOPIN[gpio]); Serial.print(" -> "); Serial.println(DxValue);

  if ( DxValue == "1" ) { // liga o pino
    digitalWrite(GPIOPIN[gpio], HIGH);
    etatGpio[gpio] = "On";
    server.send ( 200, "text/html", buildWebsite() );//atualiza a pagina
  } else if ( DxValue == "0" ) {//desliga o pino
    digitalWrite(GPIOPIN[gpio], LOW);
    etatGpio[gpio] = "Off";
    server.send ( 200, "text/html", buildWebsite() ); // Atualiza a pagina
  } else {
    Serial.println("Err Led Value");
  }

  void handleD5() {
  String D5Value; // busca a string
  updateGPIO(0, server.arg("D5"));// verifica no webserver se o name='D5' foi precionado
}

void handleD6() {
  String D6Value;
  updateGPIO(1, server.arg("D6")); // verifica no webserver se o name='D6' foi precionado
}

void handleD7() {
  String D7Value;
  updateGPIO(2, server.arg("D7")); // verifica no webserver se o name='D7' foi precionado
}

void handleD8() {
  String D8Value;
  updateGPIO(3, server.arg("D8")); // verifica no webserver se o name='D8' foi precionado

}

void handleRoot() {
  if ( server.hasArg("D5") ) { // busca no html a variavel
    handleD5();
  } else if ( server.hasArg("D6") ) {
    handleD6();
  } else if ( server.hasArg("D7") ) {
    handleD7();
  } else if ( server.hasArg("D8") ) {
    handleD8();
  } else {
    server.send ( 200, "text/html", buildWebsite() ); // Atualiza a pagina
  }
}

void setup() {
  for ( int x = 0 ; x < 5 ; x++ ) {
    pinMode(GPIOPIN[x], OUTPUT); // Array de consulta dos pinos GPIO
  }
  dht.setup(D4, DHTesp::DHT11); // conecta o sensor na porta D4 //DHT11 || DHT22

  Serial.begin ( 115200 ); // impressão do monitor serial 115200 é o ideal para o ESP
  // leitura das variaveis da configuração de connexão
  WiFi.begin ( ssid, password ); // senha do router
  WiFi.config(ip, gateway, subnet);// ip fixo
  // Attente de la connexion au réseau WiFi / Wait for connection
  while ( WiFi.status() != WL_CONNECTED ) {//enquanto não conecta mostra as tentativas
    delay ( 500 ); Serial.print ( "." );
  }
  // Connexion WiFi établie / WiFi connexion is OK
  Serial.println ( "" );
  Serial.print ( "Connected to " ); Serial.println ( ssid );
  Serial.print ( "IP address: " ); Serial.println ( WiFi.localIP() );

  //monta o servidor
  server.on ( "/", handleRoot );// recupera a pagina principal

  server.begin(); // inicia o servidor
  Serial.println ( "HTTP server started" );

}

unsigned long TempoAnterior = 0;

void loop() {
unsigned long TempoAtual = millis(); // inicia a contagem do tempo

  server.handleClient(); // executa o servidor
  if ((unsigned long)(TempoAtual - TempoAnterior) >= 1000) {// A cada 1 segundo faz a leitura do sensor
    // faz a leitura dos sensores
    t = dht.getTemperature();
    h = dht.getHumidity();
  }
  TempoAnterior = TempoAtual; // zera o cronometro
}
