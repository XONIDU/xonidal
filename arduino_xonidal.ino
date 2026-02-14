/*
  XONIDAL - Código BASE para Arduino
  RECEPCIÓN UNIVERSAL - Enciende LED con cualquier mensaje
  PUEDES MODIFICAR este código para tu proyecto específico
*/

const int LED_PIN = 13;  // LED integrado del Arduino

String inputBuffer = "";
bool comandoCompleto = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  delay(1000);
  Serial.println("=================================");
  Serial.println("🔧 ARDUINO XONIDAL - LISTO");
  Serial.println("=================================");
  Serial.println("📡 Esperando comandos...");
  Serial.println("⚡ Cualquier mensaje encenderá el LED");
}

void loop() {
  // Leer del Serial (ESP32)
  while (Serial.available()) {
    char c = Serial.read();
    
    if (c == '\n') {
      comandoCompleto = true;
    } else {
      inputBuffer += c;
    }
  }
  
  // Procesar comando
  if (comandoCompleto) {
    inputBuffer.trim();
    
    if (inputBuffer.length() > 0) {
      Serial.print("📨 Recibido: ");
      Serial.println(inputBuffer);
      
      // ===========================================
      // ACCIÓN UNIVERSAL: ENCENDER LED BREVEMENTE
      // ===========================================
      digitalWrite(LED_PIN, HIGH);
      delay(200);  // 200ms de luz
      digitalWrite(LED_PIN, LOW);
      
      // Mostrar mensaje recibido
      Serial.print("💡 LED parpadeó por comando: ");
      Serial.println(inputBuffer);
      
      // ===========================================
      // ¡AQUÍ PUEDES AGREGAR TU LÓGICA PERSONALIZADA!
      // ===========================================
      // EJEMPLOS (descomenta lo que necesites):
      
      /*
      if (inputBuffer == "ON") {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("💡 LED permanente ENCENDIDO");
      }
      else if (inputBuffer == "OFF") {
        digitalWrite(LED_PIN, LOW);
        Serial.println("💡 LED APAGADO");
      }
      else if (inputBuffer.startsWith("MOTOR")) {
        // Control de motor
        int velocidad = inputBuffer.substring(6).toInt();
        analogWrite(9, velocidad);
        Serial.print("⚙️ Motor a velocidad: ");
        Serial.println(velocidad);
      }
      else if (inputBuffer == "TEMP") {
        // Leer sensor de temperatura
        int valor = analogRead(A0);
        float voltaje = valor * (5.0 / 1023.0);
        float temperatura = (voltaje - 0.5) * 100;
        Serial.print("🌡️ Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" °C");
      }
      else if (inputBuffer == "DISTANCIA") {
        // Leer sensor ultrasónico
        Serial.println("📏 Distancia: XX cm");
      }
      */
      
      Serial.println("✅ Listo para siguiente comando");
    }
    
    inputBuffer = "";
    comandoCompleto = false;
  }
  
  // ===========================================
  // TU CÓDIGO CONTINUO AQUÍ
  // ===========================================
  // Ejemplo: leer sensores constantemente
  // int lectura = analogRead(A0);
  
  delay(10);
}
