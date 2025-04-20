import serial
import time

LORA_PORT = '/dev/serial0'
BAUDRATE = 9600

def send_command(ser, cmd, wait=1.0):
    full_cmd = cmd + '\r\n'
    ser.write(full_cmd.encode())
    print(f"[Command] {cmd}")
    time.sleep(wait)
    while ser.in_waiting:
        resp = ser.readline().decode().strip()
        print(f"[Response] {resp}")

def main():
    try:
        ser = serial.Serial(LORA_PORT, BAUDRATE, timeout=1)
        time.sleep(2)  

        send_command(ser, 'AT+OPMODE=1')           
        send_command(ser, 'ATZ', wait=2.0)         
        send_command(ser, 'AT+ADDRESS=2')          
        send_command(ser, 'AT+BAND=865000000')            
        send_command(ser, 'AT+PARAMETER=9,7,1,12') 

        print("LoRa Receiver Ready! Listening...")

        while True:
            if ser.in_waiting:
                incoming = ser.readline().decode().strip()
                if incoming:
                    print(f"[LoRa] {incoming}")

    except serial.SerialException as e:
        print(f"Serial error: {e}")
    except KeyboardInterrupt:
        print("Exiting...")
    finally:
        ser.close()

if __name__ == "__main__":
    main()

