import cv2, websocket
import threading

class Glasses:
    
    def __init__(self,address="ws://192.168.4.1:8888", video_add=0, start_streaming=True):
        self.ws = websocket.WebSocket()
        #self.ws.connect("ws://192.168.4.1:8888")
        self.ws.connect(address)
        self.img=''

        #self.cam = cv2.VideoCapture(0)  # webcam
        #self.cam = cv2.VideoCapture("/home/jatin/Videos/kungfu.mkv")  # place video file
        self.cam = cv2.VideoCapture(video_add)  # class_param

        self.streaming_thread= threading.Thread(target=self.stream)
        if start_streaming:
            self.streaming_thread.start()

    def stream(self):
        print("Streaming started...")
        while True:
            _ , self.img = self.cam.read()
            resize = cv2.resize(self.img, (250, 180)) 
            is_success, im_buf_arr = cv2.imencode(".jpg", resize)
            self.ws.send(im_buf_arr.tobytes())
