/* Bootstrap Interface for ESP8266 projects
  * Display DHT11
 * Use GPIO from Web Interface (use Leds to simulate for example)
 * 
 * Libraries used :WiFiClient, ESP8266WebServer, DHT

 * Licence : MIT
 * baseado neste código https://github.com/projetsdiy/ESP8266WebServer-Bootstrap-Bootswatch
*/
//************Bibliotecas****************************
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHTesp.h> //https://github.com/beegee-tokyo/DHTesp

//************variaveis do router
#define ssid      "SuaSSID"       // WiFi SSID
#define password  "SuaSenha"   // WiFi password


//************ variaveis GPIO*************************
const uint8_t GPIOPIN[4] = {D5, D6, D7, D8}; // Led array
String  statusGpio[4] = {"OFF", "OFF", "OFF", "OFF"}; // inicia todos os pins e
bool  boolGpio[4] = {0, 0, 0, 0}; //muda o status dos botões no server

//************variais de leitura dos sensores
DHTesp dht;
float   t = 0 ;               //varivaveis que armazenam temporariamente a ultima leitura do sensor
float   h = 0 ;               //varivaveis que armazenam temporariamente a ultima leitura do sensor

//************Memoria EEPROM virtual******************
#include <EEPROM.h>
uint8_t  eeAddress [4] = {9, 10, 11, 12}; // variavel inicial para armazenar os dados

void Save_Data() { // Grava as variaveis na EEPROM
  for ( int x = 0 ; x < 4 ; x++ ) {
    EEPROM.put(eeAddress[x] , boolGpio[x]);// Grava a variavel se o valor for diferente
    Serial.print( eeAddress[x] );
    Serial.print(" boolGpio =");
    Serial.println(boolGpio[x]);
  }
  EEPROM.commit(); //fixa as variaveis na eeprom
  Serial.println("*  Dados salvos na EEPROM");
}

void Read_Data() { //Lê as variaveis na EEPROM na função EepromSetup()
  for ( uint8_t x = 0 ; x < 4 ; x++ ) {
    EEPROM.get(eeAddress[x] , boolGpio[x]);// Grava a variavel se o valor for diferente
    Serial.print( eeAddress[x] );
    Serial.print(" boolGpio =");
    Serial.println(boolGpio[x]);
  }
  Serial.println("*  Dados lidos da EEPROM");
}

void EepromSetup() { //Recupera as variaveis no reinicio da placa - manter no Setup
  EEPROM.begin(512); // define o tamanho da memoria virtual da Eeprom
  Read_Data();//Lê as variaveis na EEPROM
}

//************variaveis do servidor***********
ESP8266WebServer server ( 80 ); // porta local do servidor

IPAddress ip(192, 168, 1, 116); //NodeMCU static IP
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

String buildWebsite() { //pagina home html
  
  String webSite = "<html lang='pt-br'><head>"; // <meta name="viewport" content='width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no'>
  webSite += "<meta charset='UTF-8'>"; 
  webSite += "<meta http-equiv='refresh' content='60' width=device-width, initial-scale=0.1, maximum-scale=1, user-scalable=yes'/>";//define a responsividade da pagina
  webSite += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'>";
  webSite += "<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script>";
  webSite += "<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  webSite += "<title>Mini estação Metereologica</title>";// titulo da pagina no navegador
  webSite += "</head><body>";
  webSite += "<div class='container-fluid'>";
  webSite +=   "<div class='row'>";
  webSite +=     "<div class='col-md-12'>";
  webSite +=       "<h1>Webserver ESP8266 + Bootstrap</h1>";
  webSite +=       "<h3>Mini estação Metereologica</h3>";
  //
  webSite +=       "<ul class='nav nav-pills'>";
  webSite +=         "<li class='active'>";
  webSite +=           "<a href='#'> <span class='badge pull-right'>";
  webSite +=           t;// exibe a variavel de temperatura
  webSite +=           "</span> Temperatura</a>";
  webSite +=         "</li><li>";
  webSite +=           "<a href='#'> <span class='badge pull-right'>";
  webSite +=           h;// exibe a variavel de umidade
  webSite +=           "</span>Umidade</a>";
  webSite +=         "</li>";
  webSite +=       "</ul>";
    //tabela dos sensores
  webSite += "<article>";
  webSite +=       "<<section class='table-responsive-sm'>";//tabela responsiva
  webSite +=       "<table class='table table-condensed'>";  // Tableau des relevés
  webSite +=         "<thead><tr><th>Sensor</th><th>Medição</th><th>Leitura</th></tr></thead>"; //Entrada
  webSite +=         "<tbody>";  // Conteudo da tabela
  webSite +=           "<tr><td>DHT11</td><td>Temp</td><td>"; // primeira linha exibe a  temperatura
  webSite +=             t;// exibe a variavel de temperatura
  webSite +=             "&deg;C</td></tr>";
  webSite +=           "<tr class='active'><td>DHT11</td><td>Umid</td><td>"; // primeira linha exibe a  umidade
  webSite +=             h;// exibe a variavel de umidade
  webSite +=             "%</td></tr>";
  webSite +=       "</tbody></table>";
  webSite +=       "</section>";//fecha tabela responsiva
  //tabela dos botões
  webSite +=       "<div class='container-fluid'>";
  webSite +=       "<div class='row'>"; // botões geral
  webSite +=       "<div class='col-sm-9'>";
  webSite +=       "<h3>GPIO</h3>";
//
  webSite +=       "<article sytle='text-align: center'>"; //abre botões
    webSite +=       "<section class='row'>";// D5
    webSite +=         "<div class='col-xs-1 col-sm-2'><h4 class ='text-left'>D5 ";
    webSite +=           "<span class='badge'>";
    webSite +=           statusGpio[0]; //Array botão D5 exibe estatus do pino
    webSite +=         "</span></h4></div>";
    webSite +=         "<div class='col-xs-2 col-sm-2'>";
    webSite +=         (boolGpio[0]) ? "<form action='/' method='POST'><button type='button submit' name='D5' value='1' class='btn btn-success btn-lg'>ON</button></form>" : "<form action='/' method='POST'><button type='button submit' name='D5' value='0' class='btn btn-danger btn-lg'>OFF</button></form>";
    webSite +=         "</div>";
    webSite +=       "</section>";
  //
    webSite +=       "<section class='row'>";// D6
    webSite +=         "<div class='col-xs-1 col-sm-2'><h4 class ='text-left'>D6 ";
    webSite +=           "<span class='badge'>";
    webSite +=           statusGpio[1]; //Array botão D6 exibe estatus do pino
    webSite +=         "</span></h4></div>";
    webSite +=         "<div class='col-xs-2 col-sm-2'>";
    webSite +=         (boolGpio[1]) ? "<form action='/' method='POST'><button type='button submit' name='D6' value='1' class='btn btn-success btn-lg'>ON</button></form>" : "<form action='/' method='POST'><button type='button submit' name='D6' value='0' class='btn btn-danger btn-lg'>OFF</button></form>";
    webSite +=         "</div>";
    webSite +=       "</section>";
  //
    webSite +=       "<section class='row'>";// D7
    webSite +=         "<div class='col-xs-1 col-sm-2'><h4 class ='text-left'>D7 ";
    webSite +=           "<span class='badge'>";
    webSite +=           statusGpio[2]; //Array botão D7 exibe estatus do pino
    webSite +=         "</span></h4></div>";
    webSite +=         "<div class='col-xs-2 col-sm-2'>";
    webSite +=         (boolGpio[2]) ? "<form action='/' method='POST'><button type='button submit' name='D7' value='1' class='btn btn-success btn-lg'>ON</button></form>" : "<form action='/' method='POST'><button type='button submit' name='D7' value='0' class='btn btn-danger btn-lg'>OFF</button></form>";
    webSite +=         "</div>";
    webSite +=       "</section>";
  //
    webSite +=       "<section class='row'>";// D8
    webSite +=         "<div class='col-xs-1 col-sm-2'><h4 class ='text-left'>D8 ";
    webSite +=           "<span class='badge'>";
    webSite +=           statusGpio[3]; //Array botão D8 exibe estatus do pino
    webSite +=         "</span></h4></div>";
    webSite +=         "<div class='col-xs-2 col-sm-2'>";
    webSite +=         (boolGpio[3]) ? "<form action='/' method='POST'><button type='button submit' name='D8' value='1' class='btn btn-success btn-lg'>ON</button></form>" : "<form action='/' method='POST'><button type='button submit' name='D8' value='0' class='btn btn-danger btn-lg'>OFF</button></form>";
    webSite +=         "</div>";
    webSite +=       "</section>";

    webSite +=       "</article>";// fecha botões

  webSite +=       "</div>"; // botões ON OFF
  webSite +=       "</div>"; //GPIO
  webSite +=       "<div>";//fecha tabela responsiva

  webSite += "</div></div></div>";
  webSite += "</body></html>";
  return webSite;
}

void check() { //monitora o status dos botões
  for ( int x = 0 ; x < 4 ; x++ ) {
    if (boolGpio[x] == 0) { // Array de consulta dos pinos GPIO
      digitalWrite(GPIOPIN[x], HIGH);
    } else {
      digitalWrite(GPIOPIN[x], LOW);
    }
  }
}

void updateGPIO(int gpio, String DxValue) { // função de alteração do status do pino (pino,status)
  Serial.println("");
  Serial.println("Update GPIO "); Serial.print(GPIOPIN[gpio]); Serial.print(" -> "); Serial.println(DxValue);

  if ( DxValue == "1") { // liga o pino
    statusGpio[gpio] = "On"; // alterna Status do texto para ON
    boolGpio[gpio] = 0; //muda o status do botão para OFF
    server.send ( 200, "text/html", buildWebsite() );//atualiza a pagina
    Save_Data(); // grava a alteração na memoria eeprom
  } else if ( DxValue == "0" ) {//desliga o pino
    statusGpio[gpio] = "Off";
    boolGpio[gpio] = 1;//muda o status do botão para ON
    Save_Data(); // grava a alteração na memoria eeprom
    server.send ( 200, "text/html", buildWebsite() ); // Atualiza a pagina
  } else {
    Serial.println("Err Led Value");
  }
}

void handleRoot() { // verifica as variaveis do html
  if ( server.hasArg("D5") ) { // se button nome == D5 
    updateGPIO(0, server.arg("D5"));// chama a função updateGPIO e verifica no webserver se o name='D5' foi precionado ==1
  } else if ( server.hasArg("D6") ) {
    updateGPIO(1, server.arg("D6")); // chama a função updateGPIO e verifica no webserver se o name='D6' foi precionado ==1
  } else if ( server.hasArg("D7") ) {
    updateGPIO(2, server.arg("D7")); // chama a função updateGPIO e verifica no webserver se o name='D7' foi precionado ==1
  } else if ( server.hasArg("D8") ) {
    updateGPIO(3, server.arg("D8")); // chama a função updateGPIO e verifica no webserver se o name='D8' foi precionado ==1
  } else {
    server.send ( 200, "text/html", buildWebsite() ); // Atualiza a pagina
  }
}

//*****************Setup**********************
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
  //
  EepromSetup();//recupera os dados da eeprom
  //monta o servidor
  server.on ( "/", handleRoot );// recupera a pagina principal
  server.begin(); // inicia o servidor
  Serial.println ( "HTTP server started" );
}
//*****************timers**********************
unsigned long TempoAnterior = 0; // cronometro de leitura do sensor
//*****************Loop*************************
void loop() {
 unsigned long TempoAtual = millis(); // inicia a contagem do tempo
  server.handleClient(); // executa o servidor
 //leitura dos senres
  if ((unsigned long)(TempoAtual - TempoAnterior) >= 1000) {// A cada 1 segundo faz a leitura do sensor
    // faz a leitura dos sensores
    t = dht.getTemperature(); // temperatura em Célsius 
    h = dht.getHumidity(); //umidade
  }
  TempoAnterior = TempoAtual; // zera o cronometro
}
