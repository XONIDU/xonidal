# 🎓 XONIDAL

**Advertencia:** Este código tiene únicamente fines educativos. Está diseñado para enseñar los principios de comunicación serial, IoT y control remoto de dispositivos. No debe utilizarse para aplicaciones críticas, sistemas de seguridad o control de dispositivos que puedan causar daños sin las medidas de seguridad apropiadas.

## 🎯 Objetivo

Este proyecto tiene como propósito enseñar cómo crear un puente de comunicación universal entre una interfaz web y un Arduino a través de un ESP32. Está diseñado como una herramienta educativa para prácticas de IoT, comunicación serial, y control remoto de dispositivos en entornos controlados.

## ⚙️ ¿Qué hace?

- 🌐 **Interfaz Web Cyberpunk**: Proporciona una interfaz de usuario con diseño tecnológico (estilo XONI-FESC) para enviar comandos.
- 🔌 **Puente ESP32 → Arduino**: El ESP32 actúa como puente WiFi-Serial, recibiendo comandos HTTP y reenviándolos al Arduino.
- 💡 **Feedback Visual**: Tanto el ESP32 como el Arduino encienden sus LEDs integrados al recibir cualquier mensaje.
- 📡 **Configuración por Monitor Serial**: El ESP32 solicita SSID y contraseña WiFi por el monitor serial al iniciar.
- 🔐 **Autenticación Basic**: Sistema de login básico (admin/1234) para proteger el acceso.
- 🚀 **Acceso Global**: Integración con Cloudflare Tunnel para acceder al sistema desde cualquier parte del mundo.
- 📨 **Comandos Universales**: Envía cualquier texto como comando; el Arduino lo recibe y ejecuta la acción programada.
- 🎯 **Estructura Modular**: Código organizado para que puedas modificar fácilmente la lógica del Arduino según tu proyecto.

## 🛡️ Aspectos Educativos

Este proyecto enseña:

- **Programación de ESP32**: Conexión WiFi, servidor HTTP, manejo de autenticación Basic.
- **Comunicación Serial**: Protocolo de comunicación entre ESP32 y Arduino.
- **Desarrollo con Flask**: Creación de servidores web en Python.
- **HTML/CSS Avanzado**: Diseño cyberpunk con efectos de neón, gradientes y animaciones.
- **IoT (Internet de las Cosas)**: Conceptos de dispositivos conectados a internet.
- **Túneles con Cloudflare**: Exposición segura de servicios locales a internet.
- **Manejo de LEDs**: Indicadores visuales de estado y recepción de comandos.
- **Decodificación Base64**: Procesamiento de autenticación HTTP.
- **Diseño de Interfaces**: Creación de UI/UX con temática tecnológica.

## 📦 Estructura del Proyecto

```
XONIDAL/
├── esp32_xonidal.ino      # Código para ESP32 (puente WiFi → Serial)
├── arduino_xonidal.ino    # Código para Arduino (recepción serial)
├── start.py               # Servidor Flask
├── templates/
│   └── index.html         # Interfaz web (diseño XONI-FESC)
└── README.md              # Este archivo
```

## 🔧 Requisitos

### Hardware:
- 1x ESP32 (cualquier modelo)
- 1x Arduino (Uno, Nano, Mega, etc.)
- Cables jumper para conexión serial
- LEDs integrados (ya vienen en las placas)

### Software:
- Arduino IDE con soporte para ESP32
- Python 3.8+ con Flask
- Cloudflared (opcional, para acceso global)
- Navegador web moderno

## 🚀 Instalación y Uso

### Paso 1: Cargar ESP32
1. Abre `esp32_xonidal.ino` en Arduino IDE
2. Selecciona la placa ESP32 correcta
3. Sube el código
4. Abre Monitor Serial (9600 baud)
5. Ingresa SSID y contraseña WiFi cuando se solicite
6. **Anota la IP que aparece** (ej: 192.168.1.45)

### Paso 2: Cargar Arduino
1. Abre `arduino_xonidal.ino` en Arduino IDE
2. **Personaliza** la función `ejecutarComando()` según tu proyecto
3. Sube el código al Arduino
4. Conecta físicamente:
   ```
   ESP32 TX → Arduino RX
   ESP32 RX → Arduino TX
   GND      → GND
   ```

### Paso 3: Iniciar Servidor Flask
```bash
# Instalar dependencias
pip install flask requests

# Ejecutar servidor
python start.py
```

### Paso 4: Acceder desde el navegador
- Local: `http://localhost:5050`
- Usuario: `admin`
- Contraseña: `1234`
- Ingresa la IP del ESP32 en el campo correspondiente

### Paso 5: Acceso Global (Opcional)
```bash
# Instalar cloudflared
# https://github.com/cloudflare/cloudflared

# Ejecutar túnel
cloudflared tunnel --url http://localhost:5050
```
Obtendrás una URL como: `https://random-name.trycloudflare.com`

## 🎮 Comandos de Ejemplo

El código base del Arduino enciende el LED con **cualquier mensaje**. Puedes modificarlo para:

| Comando | Acción Programable |
|---------|-------------------|
| `LED_ON` | Encender LED |
| `LED_OFF` | Apagar LED |
| `MOTOR_150` | Controlar motor |
| `TEMP` | Leer temperatura |
| `SERVO_90` | Mover servo |
| `ALARMA` | Activar alarma |
| `DISTANCIA` | Leer sensor ultrasónico |

## 🔌 Conexiones Físicas

```
ESP32           Arduino
-----           -------
GPIO17 (TX)  →  Pin 0 (RX)
GPIO16 (RX)  ←  Pin 1 (TX)
GND          →  GND
```

## ⚡ Funcionamiento Paso a Paso

1. **ESP32** solicita WiFi por Serial y se conecta
2. **Servidor Flask** inicia en localhost:5050
3. **Usuario** accede a la web y configura IP del ESP32
4. **Usuario** envía cualquier comando desde la web
5. **Flask** envía POST al ESP32 con autenticación
6. **ESP32** verifica auth y reenvía por Serial al Arduino
7. **Arduino** recibe el comando y ejecuta la acción
8. **LEDs** de ambas placas parpadean confirmando recepción

## 🎨 Personalización

### Modificar Arduino:
Edita la función `ejecutarComando()` en `arduino_xonidal.ino`:

```cpp
void ejecutarComando(String comando) {
  if (comando == "TU_COMANDO") {
    // Tu código aquí
    digitalWrite(TU_PIN, HIGH);
  }
}
```

### Cambiar credenciales:
- En `start.py`: modifica `USERNAME` y `PWD`
- En `esp32_xonidal.ino`: modifica `admin:1234` en `checkAuth()`

## ❓ ¿Dudas o sugerencias educativas?

Si tienes preguntas sobre los conceptos de programación, IoT, comunicación serial o control remoto enseñados en este proyecto:

- 📸 **Instagram:** @xonidu
- 📘 **Facebook:** xonidu
- 📧 **Email:** xonidu@gmail.com

**Nota:** Este proyecto es únicamente para fines educativos y de aprendizaje. No está diseñado para aplicaciones críticas, sistemas de seguridad industrial, ni control de dispositivos de alta potencia sin las protecciones adecuadas.

## 📚 Aviso de Uso Educativo

Este software se proporciona "tal cual", sin garantía de ningún tipo. El autor no se hace responsable del uso que se le dé a este código. El usuario es el único responsable de cumplir con todas las leyes y regulaciones aplicables en su jurisdicción respecto al control de dispositivos electrónicos.

### Uso Aceptable:
- ✅ Práctica de programación y desarrollo de habilidades técnicas
- ✅ Demostración de conceptos de IoT en entornos educativos
- ✅ Aprendizaje sobre comunicación serial y control remoto
- ✅ Proyectos personales de bajo voltaje y baja potencia

### Uso No Aceptable:
- ❌ Control de dispositivos de alta potencia sin protecciones
- ❌ Sistemas de seguridad críticos (alarmas, cerraduras, etc.)
- ❌ Aplicaciones médicas o de soporte vital
- ❌ Uso comercial sin las debidas certificaciones
- ❌ Cualquier aplicación que pueda causar daños a personas o propiedades

---

## 🏆 Créditos

**Desarrollado por:** Darian Alberto Camacho Salas y Oscar Rodolfo Barragán Pérez
**Asesor:** Dr. Raúl Dalí Cruz Morales
**Proyecto:** XONIDAL - Universal Serial Bridge  
**Año:** 2025-2026
