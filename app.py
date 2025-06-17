from flask import Flask, render_template, request, jsonify
from flask_sqlalchemy import SQLAlchemy
from datetime import datetime
import urllib.parse

# Flask App
app = Flask(__name__)

# --- PostgreSQL Configuration ---
DB_CONFIG = {
    "dbname": "your_database",
    "user": "your_user",
    "password": "your_password",
    "host": "localhost",
    "port": "5432"
}

# Assemble SQLAlchemy URI
db_uri = f"postgresql://{DB_CONFIG['user']}:{urllib.parse.quote(DB_CONFIG['password'])}@{DB_CONFIG['host']}:{DB_CONFIG['port']}/{DB_CONFIG['dbname']}"
app.config['SQLALCHEMY_DATABASE_URI'] = db_uri
app.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

# SQLAlchemy Init
db = SQLAlchemy(app)

# --- DoorLog Model ---
class DoorLog(db.Model):
    id = db.Column(db.Integer, primary_key=True)
    event = db.Column(db.String(50), nullable=False)
    timestamp = db.Column(db.DateTime, default=datetime.utcnow)

# --- Create Tables Automatically ---
with app.app_context():
    db.create_all()

# --- Routes ---
@app.route('/')
def index():
    return render_template('index.html')

@app.route('/logs')
def logs():
    all_logs = DoorLog.query.order_by(DoorLog.timestamp.desc()).all()
    return render_template('logs.html', logs=all_logs)

@app.route('/log_event', methods=['POST'])
def log_event():
    event = request.json.get('event')
    if event:
        new_log = DoorLog(event=event)
        db.session.add(new_log)
        db.session.commit()
        return jsonify({'status': 'success'}), 201
    return jsonify({'status': 'error'}), 400

if __name__ == '__main__':
    app.run(debug=True)
