import sys
import socket
import time
TCP_IP = '192.168.1.254'
TCP_PORT = 65000
BUFFER_SIZE = 1024
param = []
i=0
from collections import defaultdict, Counter

all_entries = defaultdict(list)
main_counter = Counter()
car_1 = "BlueCar"
car_2 = "GreenCar"
#s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#s.bind((TCP_IP,TCP_PORT))
#s.listen(1)

#print 'Listening for client...'

#conn, addr = s.accept()
#print 'Connection address:', addr
current_bucket=None
# file = open ("data", "w") 
try:
    while 1:
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((TCP_IP,TCP_PORT))
        s.listen(1)
        while 1: 
                print ('Listening for client...')
                conn, addr = s.accept()
                print ('Connection address:', addr)
                data = conn.recv(BUFFER_SIZE)
                message = data.strip().decode()
                all_entries[message].append(time.time_ns())

                print(all_entries)
                
                # if current_bucket is None:
                #     current_bucket = time.time_ns()//200e6
                #     print(f"adding {message}")
                #     main_counter.update([message])
                # if time.time_ns()//200e6 == current_bucket:
                #     main_counter.update([message])
                #     print(f"adding {message}")
                #     print(main_counter)
                #     laps = [k for k,v in main_counter.items() if v >=3]
                #     if laps:
                #         print (f"Lap for {laps}")
                
                # else:
                #     current_bucket = None
                #     main_counter.clear()
                
                
                # print(f"{all_entries=}")    
                # print(f"{counts=}")

                
                    
                # if data == "\r" :
                #         conn.close()
                #         print ("Received all the data")
                #         i=0
                #         for x in param:
                #                 print (x)
                #         #break
                # elif data:
                #         print ("received data: ", data)
                #         param.insert(i,data)
                #         i+=1
                        #print "End of transmission"
        s.close()
finally:
     s.close()