import socket
import json

UDP_IP = "192.168.4.1"
UDP_PORT = 4210
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
   
packet = {
        "Thrust":100,
        "Roll":0,
        "Pitch":0,
        "Yaw":0 
        }

packet = json.dumps(packet)
packet.encode()

sock.sendto(packet, (UDP_IP, UDP_PORT))