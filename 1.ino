#include <WiFi.h>
#include <WebServer.h>

const char *NOMBRE_RED = "ES32_Sp3",
            *CLAVE_RED = "123456789";

const int PUERTO = 80,
          lED_D1 = 2;

//ESTADO DEL LED
bool estadoDelLED=false;

IPAddress ip(192, 168, 1, 1);
IPAddress puertaDeEnlace(192, 168, 1, 1);
IPAddress mascaraDeRed(255, 255, 255, 0);
WebServer servidor (PUERTO);

String obtenerInterfaz()
{
String HtmlRespuesta = "<¡DOCTYPE html>"
                        "<html lang='en'>"
                        "<head>"
                        "<meta charset = 'UTF-8'>"
                        "<meta name = 'viewport' content='width=device-width, initial-scale=1.0'>"
                        "<title> LED prueba </title>"
                        "</head>"
                        "<body>";
  
HtmlRespuesta += "<h1> Encender y apagar LED </h1>";
HtmlRespuesta += "<p>El led est&aacute; actualmente</p>";
if(estadoDelLED)
  {
    HtmlRespuesta += "Encendido</p>";
    HtmlRespuesta += "<a href='/apagar'> Apagar </a>";
  
  }
  else
  {
    HtmlRespuesta += "Apagado</p>";
    HtmlRespuesta += "<a href='/encender'>Encender</a>";

  }
  HtmlRespuesta += "</body>"
                    "/html";
  return HtmlRespuesta;

}

void mostrarInterfazHTML()
{
  servidor.send(200, "text/html", obtenerInterfaz());

}


void rutaEncenderLed()
{
  estadoDelLED = true;
  digitalWrite(lED_D1, HIGH);
  mostrarInterfazHTML();
}

void rutaApagarLed()
{
  estadoDelLED = false;
  digitalWrite (lED_D1, LOW);
  mostrarInterfazHTML();
}

void rutaRaiz()
 {
  mostrarInterfazHTML(); 
 }

void rutaNoencontrada()
{
  servidor.send(404, "text/plain", "404");

}



void setup() {
pinMode(lED_D1, OUTPUT);

WiFi.softAP(NOMBRE_RED, CLAVE_RED);
WiFi.softAPConfig(ip, puertaDeEnlace, mascaraDeRed);
delay(100);

servidor.on("/", rutaRaiz);
servidor.on("/encender", rutaEncenderLed);
servidor.on("/apagar", rutaApagarLed);
servidor.onNotFound(rutaNoencontrada);
servidor.begin();

}

void loop() {
  servidor.handleClient();

}
