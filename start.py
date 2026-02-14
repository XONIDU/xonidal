from flask import Flask, request, Response, jsonify, render_template
import requests
import base64

app = Flask(__name__)

USERNAME = "admin"
PWD = "1234"
ESP32_IP = None

# ----------------------- AUTENTICACIÓN -----------------------
def check_auth(auth_header):
    if not auth_header:
        return False
    try:
        auth_type, auth_info = auth_header.split(None, 1)
        if auth_type.lower() != "basic":
            return False
        decoded = base64.b64decode(auth_info).decode("utf-8")
        user, pwd = decoded.split(":", 1)
        return user == USERNAME and pwd == PWD
    except:
        return False

def authenticate():
    return Response(
        "Autenticación requerida", 401,
        {"WWW-Authenticate": 'Basic realm="XONIDAL"'}
    )

# ------------------------- PÁGINA WEB -------------------------
@app.route("/")
def index():
    return render_template('index.html', ESP32_IP=ESP32_IP)

# ---------------------- SET IP ----------------------
@app.route("/set_ip", methods=["POST"])
def set_ip():
    global ESP32_IP
    
    data = request.json
    ESP32_IP = data.get("ip", "").strip()
    
    if not ESP32_IP:
        return jsonify({"status": "❌ IP inválida"})
    
    return jsonify({"status": f"✅ IP: {ESP32_IP}"})

# ---------------------- ENVIAR COMANDO ----------------------
@app.route("/send", methods=["POST"])
def send():
    global ESP32_IP
    
    if not ESP32_IP:
        return jsonify({"status": "❌ Configura IP primero"})
    
    data = request.json
    msg = data.get("message", "")
    
    try:
        response = requests.post(
            f"http://{ESP32_IP}/print", 
            data={"message": msg}, 
            timeout=2
        )
        if response.status_code == 200:
            return {"status": f"✅ OK: {msg}"}
        else:
            return {"status": f"❌ Error {response.status_code}"}
    except Exception as e:
        return {"status": f"❌ Error: {str(e)}"}

# ---------------------- INICIAR ----------------------
if __name__ == "__main__":
    print("\n=================================")
    print("🚀 XONIDAL SERVER")
    print("=================================")
    print("📡 Local: http://localhost:5050")
    print("🔐 admin:1234")
    print("🌍 Cloudflare: cloudflared tunnel --url http://localhost:5050")
    print("=================================\n")
    app.run(port=5050, host="0.0.0.0")
