"""Original course code (October 2024), exported from a Jupyter notebook without changes.

Requires: pip install serialduino
"""
from serialduino import Arduino
import time

# Set up the serial connection
ser = Arduino(port="COM3", baudrate=9600, timeout=0.1)  # Adjust the port based on your system
time.sleep(2)  # Give some time for the serial connection to initialize

def log_data(event_type, data):
    with open("alarm_log.txt", "a") as f:
        if data == "":
            f.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - {event_type}\n")
        else:
            f.write(f"{time.strftime('%Y-%m-%d %H:%M:%S')} - {event_type}: {data}\n")
            
while True:
    line = ser.readline().strip()  # Read from the serial port
    if line:
        print(f"Received: {line}")
        
        if "ALARM_ACTIVATED" in line:
            activation_time = line.split(":")[1]
            print(f"Alarm activated at {activation_time} seconds.")
            log_data("ALARM_ACTIVATED", "")
        
        elif "RFID_AUTHORIZED" in line:
            uid = line.split(":")[1:]
            print(f"RFID authorized, UID: {uid}")
            log_data("RFID_AUTHORIZED", uid)
        
        elif "ALARM_DISABLED" in line:
            print("Alarm disabled by owner (RFID).")
            log_data("ALARM_DISABLED", "Owner (RFID)")
        
        elif "INTRUDER_ALERT" in line:
            intruder_time = line.split(":")[1]
            print(f"Intruder detected. Red phase lasted for {intruder_time} seconds.")
            log_data("INTRUDER_ALERT", "")
            
        elif "INTRUDER_GONE" in line:
            intruder_time = line.split(":")[1]
            print(f"Intruder detected. Red phase lasted for {intruder_time} seconds.")
            log_data("INTRUDER_GONE", intruder_time)

    time.sleep(0.1)  # Small delay to avoid overwhelming the CPU

