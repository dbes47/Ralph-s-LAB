import tkinter as tk
from tkinter import ttk

def do_it():
    if submit_btn['text'] == 'Click Me':
        submit_btn.configure(text="Clicked")
        #test_label.configure(text="I want you click that button")

    else:
        submit_btn.configure(text="Click Me")
        #test_label.configure(text="button above has been clicked")



win = tk.Tk()
win.title("GUI STUDY")
test_label=ttk.Label(win,text="this is PM2.5")
test_label.grid(column=1,row=0)
submit_btn=ttk.Button(win,text="Click Me",command=do_it)
submit_btn.grid(column=1,row=1)

name=tk.StringVar()
textBox=ttk.Entry(win,width=12,textvariable=name)
textBox.grid(column=1,row=2)

submit_btn.focus()


win.mainloop()


