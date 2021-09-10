from Adafruit_IO import Client, Feed, Data
from time import sleep
import serial
import time
import sys

AIO_User = "ALV19392"
AIO_Key = "aio_GtEa42EssXeaDuDNTFeLgpvm4LUP"
aio = Client(AIO_User,AIO_Key)


digital_feed = aio.feeds('temperatura') #colocar el nombre del feed
digital_feed2 = aio.feeds('humedad') #colocar el nombre del feed
digital_feed3 = aio.feeds('contador') #colocar el nombre del feed

P = ["","",""] #array para guardar los valores

puerto = serial.Serial()
puerto.baudrate = 9600 #Baudrate igual que el del PIC
puerto.timeout = 3 #tiempo de espera para recibir un dato cualquiera
puerto.port = 'COM4' #Puerto usado para el serial
puerto.open()
print('Puerto activado con exito')

while(1):
    with puerto: #para que tome la com serial
        for i in range(3):
            P[i]=puerto.readline().strip() #divide los dos datos enviados
    aio.send_data(digital_feed2.key, int(P[0])) #enviar el dato al feed que buscamos
    aio.send_data(digital_feed.key, int(P[1])) #enviar el dato al feed que buscamos
    aio.send_data(digital_feed3.key, int(P[2])) #enviar el dato al feed que buscamos
    puerto.close()
    digital_data = aio.receive('contador2').value #leer el dato del feed especificado
    puerto.open()
    puerto.write(chr(int(digital_data)).encode())
    time.sleep(10)
