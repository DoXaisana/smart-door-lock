from flask import Flask, render_template, request, redirect, url_for, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import serial
import threading
import time
import urllib.parse

# PostgreSQL Configuration
DB_CONFIG = {
    "dbname": "smart_door",
    "user": "your_user",
    "password": "your_password",
    "host": "localhost",
    "port": "5432"
}

# Build the connection string
db_uri = f"postgresql://{DB_CONFIG['user']}:{urllib.parse.quote(DB_CONFIG['password'])}@{DB_CONFIG['host']}:{DB_CONFIG['port']}/{DB_CONFIG['dbname']}"

# Flask App Config
app = Flask(__name__)
app.config['SQLALCHEMY_DATABASE_URI'] = db_uri
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
db = SQLAlchemy(app)

# Serial connection to Arduino
arduino = serial.Serial('COM3', 9600, timeout=1)  # Change COM port as needed
door_status = "UNKNOWN"  # Global shared status

# Database model
class DoorLog(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    event = db.Column(db.String(50), nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)

# Log helper
def log_event(event):
    db.session.add(DoorLog(event=event))
    db.session.commit()

# Serial reading in background
def read_serial():
    global door_status
    while True:
        if arduino.in_waiting > 0:
            line = arduino.readline().decode().strip()
            if line in ["LOCKED", "UNLOCKED"]:
                door_status = line
                log_event(f"Door {line}")
        time.sleep(0.1)

# Start background thread
threading.Thread(target=read_serial, daemon=True).start()

# Create table on startup
with app.app_context():
    db.create_all()

# Routes
@app.route('/')
def index():
    return render_template("index.html", status=door_status)

@app.route('/open', methods=['POST'])
def open_door():
    global door_status
    if door_status == "UNLOCKED":
        arduino.write(b"OPEN\n")
        log_event("Door OPENED")
    return redirect(url_for('index'))

@app.route('/logs')
def logs():
    all_logs = DoorLog.query.order_by(DoorLog.timestamp.desc()).limit(50).all()
    return render_template('logs.html', logs=all_logs)
