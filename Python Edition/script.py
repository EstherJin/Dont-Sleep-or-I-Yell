import time
import win32api
import winsound
import ctypes
import threading
from threading import Thread
import tkinter as tk
import PIL
from PIL import Image, ImageTk

previousTime = 0
currentTime = 1
counter = 0
on = False


def is_awake():
    global counter
    previousTime = 0
    while True:
        if on:
            currentTime = win32api.GetLastInputInfo()
            if (previousTime == currentTime):
                counter += 1
            else:
                counter = 0
            #print(counter)
            previousTime = currentTime
            time.sleep(1)

def awakening():
    while True:
        if on:
            time.sleep(1)
            #print("awakening counter: " + str(counter))
            if counter > 120:
                #print("hello")
                ctypes.windll.user32.MessageBoxW(0, "WAKE UP", "WAKE UPPPPP", 0)


def seriously_wake_up():
    while True:
        if on:
            time.sleep(1)
            #print("wake up counter: " + str(counter))
            if counter > 150:
                #print("get those earplugs")
                winsound.Beep(1500, 1000)           


def toggle():
    global on
    if t_btn.config('text')[-1] == 'Turn Off':
        t_btn.config(text = 'Turn On')
        on = False
    else:
        t_btn.config(text = 'Turn Off')
        on = True
        

    #print(on)

root = tk.Tk()
root.configure(background = '#cce5ff')
w = tk.Label(root, text="Don't Sleep or I Yell",
             font = ("MS Sans Serif", 30, "bold"), background = '#cce5ff')
w.pack()
img = ImageTk.PhotoImage(Image.open("OCTOCAT.png"))
panel = tk.Label(root, image = img, background = '#cce5ff')
panel.pack(side = "bottom", fill = "both", expand = "yes")
t_btn = tk.Button(text = "Turn On", width = 12, command = toggle)
t_btn.pack(pady=5)


p1 = Thread(target = is_awake)
p2 = Thread(target = awakening)
p3 = Thread(target = seriously_wake_up)

p1.start()
#print("p1")
p2.start()
p3.start()
