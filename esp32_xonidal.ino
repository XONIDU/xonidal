/*
  XONIDAL - ESP32 WiFi a Serial Bridge
  Versión Simplificada - Solo reenvía comandos
*/

#include <WiFi.h>

// Variables globales
String SSID = "";
String PASSWORD = "";
bool configurado = false;
const int LED_PIN = 2;  // LED integrado del ESP32

WiFiServer server(80);  // Servidor HTTP

// ===== CONFIGURAR WIFI =====
void configurarWiFi() {
  Serial.println("\n=================================");
  Serial.println("🔧 XONIDAL - CONFIGURACIÓN INICIAL");
  Serial.println("=================================");
  
  // Pedir SSID
  Serial.print("📶 SSID: ");
  while (!Serial.available()) {
    delay(100);
  }
  SSID = Serial.readStringUntil('\n');
  SSID.trim();
  
  // Pedir contraseña
  Serial.print("🔑 PASSWORD: ");
  while (!Serial.available()) {
    delay(100);
  }
  PASSWORD = Serial.readStringUntil('\n');
  PASSWORD.trim();
  
  Serial.println("\n📡 Conectando...");
  WiFi.begin(SSID.c_str(), PASSWORD.c_str());
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(1000);
    Serial.print(".");
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ CONECTADO!");
    Serial.print("🌐 IP: ");
    Serial.println(WiFi.localIP());
    Serial.println("🚀 Servidor listo");
    configurado = true;
    server.begin();
    
    // Parpadear LED para indicar conexión exitosa
    for(int i = 0; i < 3; i++) {
      digitalWrite(LED_PIN, HIGH);
      delay(200);
      digitalWrite(LED_PIN, LOW);
      delay(200);
    }
  } else {
    Serial.println("\n❌ Error. Reiniciando...");
    delay(3000);
    ESP.restart();
  }
}

// ===== DECODIFICAR BASE64 =====
String base64Decode(String input) {
  const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  String decoded = "";
  int val = 0, valb = -8;
  
  for (char c : input) {
    if (c == '=') break;
    const char *p = strchr(b64, c);
    if (!p) break;
    val = (val << 6) + (p - b64);
    valb += 6;
    if (valb >= 0) {
      decoded += char((val >> valb) & 0xFF);
      valb -= 8;
    }
  }
  return decoded;
}

// ===== VERIFICAR AUTENTICACIÓN =====
bool checkAuth(String authHeader) {
  if (!authHeader.startsWith("Basic ")) return false;
  
  String base64Credentials = authHeader.substring(6);
  base64Credentials.trim();
  
  String credentials = base64Decode(base64Credentials);
  
  int separatorPos = credentials.indexOf(':');
  if (separatorPos == -1) return false;
  
  String user = credentials.substring(0, separatorPos);
  String pass = credentials.substring(separatorPos + 1);
  
  return (user == "admin" && pass == "1234");
}

// ===== PROCESAR PETICIONES =====
void handleClient() {
  WiFiClient client = server.available();
  
  if (client) {
    String header = "";
    String authHeader = "";
    String message = "";
    bool authenticated = false;
    
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        header += c;
        
        if (c == '\n') {
          if (header.length() == 2) {
            
            // Verificar autenticación
            int authIndex = header.indexOf("Authorization:");
            if (authIndex != -1) {
              int start = header.indexOf("Basic", authIndex);
              int end = header.indexOf("\r\n", authIndex);
              if (start != -1 && end != -1) {
                authHeader = header.substring(start, end);
                authHeader.trim();
                authenticated = checkAuth(authHeader);
              }
            }
            
            // Obtener mensaje
            int bodyStart = header.indexOf("\r\n\r\n") + 4;
            if (bodyStart > 4) {
              String body = header.substring(bodyStart);
              if (body.startsWith("message=")) {
                message = body.substring(8);
                // Decodificar URL
                message.replace("%20", " ");
                message.replace("%2C", ",");
                message.replace("%21", "!");
                message.replace("%3F", "?");
                message.trim();
              }
            }
            
            // Responder
            if (!authenticated) {
              client.println("HTTP/1.1 401 Unauthorized");
              client.println("WWW-Authenticate: Basic realm=\"XONIDAL\"");
              client.println("Content-Type: application/json");
              client.println("Connection: close");
              client.println();
              client.println("{\"status\":\"error\",\"message\":\"Auth required\"}");
            }
            else if (message.length() > 0) {
              // ENCENDER LED DEL ESP32
              digitalWrite(LED_PIN, HIGH);
              delay(100);
              digitalWrite(LED_PIN, LOW);
              
              // ENVIAR A ARDUINO
              Serial.println(message);
              
              client.println("HTTP/1.1 200 OK");
              client.println("Content-Type: application/json");
              client.println("Connection: close");
              client.println();
              client.println("{\"status\":\"ok\",\"message\":\"Enviado: " + message + "\"}");
              
              Serial.print("📨 Enviado a Arduino: ");
              Serial.println(message);
            }
            else {
              client.println("HTTP/1.1 400 Bad Request");
              client.println("Content-Type: application/json");
              client.println("Connection: close");
              client.println();
              client.println("{\"status\":\"error\",\"message\":\"No message\"}");
            }
            
            break;
          }
        }
      }
    }
    
    client.stop();
  }
}

// ===== SETUP =====
void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  delay(1000);
  Serial.println("\n🚀 XONIDAL - PUENTE UNIVERSAL");
  configurarWiFi();
}

// ===== LOOP =====
void loop() {
  if (configurado) {
    handleClient();
  }
  
  // Leer respuestas del Arduino
  if (Serial.available()) {
    String respuesta = Serial.readStringUntil('\n');
    respuesta.trim();
    if (respuesta.length() > 0) {
      Serial.print("📥 Arduino: ");
      Serial.println(respuesta);
    }
  }
  
  delay(10);
}
