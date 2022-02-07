import cv2
import websocket # pip3 install websocket-client
import time
import traceback

# Area of target display (here 240x240 ST7789)
#connect to ssid " Glasses " with pass "12345678"

ws = websocket.WebSocket()
ws.connect("ws://192.168.4.1:8888")


#cam = cv2.VideoCapture("/home/jatin/Videos/kungfu.mkv")  # place video file
cam = cv2.VideoCapture(0)  # webcam
start_time=0

def stream_out():

    print("Streaming started...")
    start_time=time.time()
    
    try:
        while True:
            ret, img = cam.read()
            resize = cv2.resize(img, (250, 180)) 
            is_success, im_buf_arr = cv2.imencode(".jpg", resize)
            byte_im = im_buf_arr.tobytes()
            ws.send(byte_im)
    except Exception as e:
        print("error occured at time: ",( time.time()- start_time))
        print(e)
        traceback.print_exc()

        
stream_out()
