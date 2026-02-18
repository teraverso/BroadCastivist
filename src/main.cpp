#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DNSServer.h>

const char* AP_SSID = "BroadCastivist"; // nome da rede Wi-Fi do AP
const byte DNS_PORT = 53;

DNSServer dnsServer;
ESP8266WebServer webServer(80);

const char PAGE[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
<meta charset="utf-8">
<title>Portal Cativo</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<style>
body{font-family:Arial,Helvetica,sans-serif;background:#111;color:#eee;margin:0}
main{max-width:720px;margin:10vh auto;padding:24px}
h1{color:#4caf50}
p{line-height:1.6}
.box{background:#1e1e1e;border-radius:8px;padding:20px}
</style>
</head>
<body>
<main>
  <div class="box">
    <h1>Essa é uma demonstração de segurança!</h1>
    <p>Fique atento às redes Wi-Fi públicas em que você se conecta para não cair em golpes.</p>
    <p>Use VPN, evite redes abertas e não digite senhas em portais desconhecidos.</p>
    <p>------------------</p>
    <p>Nenhum dado foi coletado. Nenhuma autenticação foi solicitada.</p>
    <p>Essa rede é destinada apenas para demonstrações educacionais. Tenha um excelente dia!</p>
  </div>
</main>
</body>
</html>
)rawliteral";

void handleRoot() {
  webServer.send_P(200, "text/html", PAGE);
}

void setup() {
  Serial.begin(115200);
  delay(200);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(AP_SSID);          // AP aberto, sem senha
  IPAddress apIP = WiFi.softAPIP();

  dnsServer.start(DNS_PORT, "*", apIP); // redireciona qualquer domínio

  webServer.on("/", handleRoot);
  webServer.onNotFound(handleRoot);     // força portal para qualquer URL
  webServer.begin();
}

void loop() {
  dnsServer.processNextRequest();
  webServer.handleClient();
}

