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


# is_awake() checks if the user is idle and tracks idle time
def is_awake():
    global counter
    previousTime = 0
    while True:
        if on:
            currentTime = win32api.GetLastInputInfo()
            if (previousTime == currentTime):
                counter += 0.2
            else:
                counter = 0
            previousTime = currentTime
            time.sleep(0.2)


# awakening() opens a message box when the user has been idle for 2 mins
def awakening():
    while True:
        if on:
            time.sleep(1)
            #print("awakening counter: " + str(counter))
            if counter > 120:
                ctypes.windll.user32.MessageBoxW(0, "WAKE UP", "WAKE UPPPPP", 0)


# seriously_wake_up() plays an alarm when the user has been idle for 2.5 mins
def seriously_wake_up():
    global counter
    while True:
        if on:
            timer = 1
            timercounter = 1
            while counter > 150:
                winsound.Beep(1500, max(1000-timercounter // 5*100, 200))
                time.sleep(timer)
                if timer > 0.20:
                    timer = 1 -  (timercounter // 5) * 0.21
                    timercounter += 1


# toggle() controls the button on the GUI
def toggle():
    global on
    if t_btn.config('text')[-1] == 'Turn Off':
        t_btn.config(text = 'Turn On')
        on = False
    else:
        t_btn.config(text = 'Turn Off')
        on = True
        


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
p2.start()
p3.start()
