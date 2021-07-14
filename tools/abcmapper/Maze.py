# -*- coding: cp949 -*-
# Tkinter 테스트

from tkinter import filedialog
from tkinter import *
from datetime import datetime
import math
import json
import os

CANVAS_WIDTH = 700
CANVAS_HEIGHT = 900
DEFAULT_MAP_WIDTH = 20
DEFAULT_MAP_HEIGHT = 20

class Maze:
    nodes = []
    edges = []
    hints = []

    def __init__(self):
        pass

    def ClearGraph(self):
        # initialize
        global start, end, selected, recent
        start = 0
        end = 0
        selected = -1
        recent = -1

        del self.nodes[:]
        del self.edges[:]
        del self.hints[:]

    def AutoEdge(self):
        for k in range(0, len(self.edges) - 1):
            self.edges[k].append(k + 1)
            self.edges[k + 1].append(k)

    def UpdateHint(self):
        global start, end

        del self.hints[:]
        self.hints = self.GetHint(self.edges, start, end)

    def GetHint(self, edges, start, end):
        queue = [(start, [start])]
        result = []

        while queue:
            n, path = queue.pop(0)
            if n == end:
                result.append(path)
                break
            else:
                graph = set()
                for f in edges[n]:
                    graph.add(f)

                for m in graph - set(path):
                    queue.append((m, path + [m]))

        if len(result) == 0:
            print('no hint')
            return []

        print('yes hint')
        return result[0]

class App:

    def __init__(self, master):
        frame = Frame(master)
        frame.pack()

        self.canvas = Canvas(frame, dict(width=CANVAS_WIDTH, height=CANVAS_HEIGHT))
        self.canvas.bind("<Key>", self.KeyPressed)
        self.canvas.bind("<Up>", self.KeyUp)
        self.canvas.bind("<Down>", self.KeyDown)
        self.canvas.bind("<Left>", self.KeyLeft)
        self.canvas.bind("<Right>", self.KeyRight)
        self.canvas.bind("<Button-1>", self.MouseDownL)
        self.canvas.bind("<Button-3>", self.MouseDownR)
        self.canvas.bind("<B1-Motion>", self.MouseMove)
        self.canvas.pack()

        self.canvas.focus_set()

        global start, end, mode, selected, recent
        start = 0
        end = 0
        mode = 1
        selected = -1
        recent = -1

        f = Frame(master)
        f.pack()

        t = Label(f, text="Mode:")
        t.pack(side=LEFT)
        self.t2 = Label(f, text="Select")
        self.t2.pack(side=LEFT)

        btnLoadBG = Button(f, text="LoadBG", command=self.LoadBG)
        btnLoadBG.pack(side=LEFT)

        btnLoad = Button(f, text="Load", command=self.Load)
        btnLoad.pack(side=LEFT)

        btnSave = Button(f, text="Save", command=self.Save)
        btnSave.pack(side=LEFT)

        b = Button(f, text="Clear", command=self.ClearFunc)
        b.pack(side=LEFT)

        b2 = Button(f, text="Hint", command=self.HintFunc)
        b2.pack(side=LEFT)

        self.t3 = Label(f, text='0')
        self.t3.pack(side=LEFT)
        t4 = Label(f, text=' x ')
        t4.pack(side=LEFT)
        self.t5 = Label(f, text='0')
        self.t5.pack(side=LEFT)

        btnAuto = Button(f, text="Auto", command=self.AutoFunc)
        btnAuto.pack(side=LEFT)

        btnMee = Button(f, text="MEE", command=self.Mee)
        btnMee.pack(side=LEFT)

        self.BackgroundImage = PhotoImage()
        self.BackgroundImage.blank()

        # create maze
        self.Maze = Maze()

        self.Maze.ClearGraph()
        self.RedrawMaze()

    def LoadBG(self):
        currentDir = os.getcwd()
        filename = filedialog.askopenfilename(initialdir=currentDir, title='select file',
                                              filetypes=(('png files', '*.png'), ('jpeg files', '*.jpg')))

        if os.path.isfile(filename) == FALSE:
            return

        self.BackgroundImage.blank()
        self.BackgroundImage = PhotoImage(file=filename)

        self.RedrawMaze()

        pass

    def Load(self):
        currentDir = os.getcwd()
        filename = filedialog.askopenfilename(initialdir=currentDir, title='select file',
                                              filetypes=(('json files', '*.json'), ('text files', '*.txt')))

        if os.path.isfile(filename) == FALSE:
            return

        global start, end
        jsondata = open(filename).read()
        data = json.loads(jsondata)

        self.ClearFunc()

        start = int(data['start']) - 1
        end = int(data['end']) - 1

        for k in range(0, len(data['nodes'])):
            pos = []
            pos.append(data['nodes'][k]['x'])
            pos.append(900 - data['nodes'][k]['y'])
            self.Maze.nodes.append(pos)

        for k in range(0, len(data['edges'])):
            adjs = []
            for f in data['edges'][k]['adjs']:
                adjs.append(f - 1)
            self.Maze.edges.append(adjs)

        if 'hint' in data:
            for k in data['hint']:
                self.Maze.hints.append(k - 1)

        self.RedrawMaze()

        pass

    def Save(self):
        currentDir = os.getcwd()
        filename = filedialog.asksaveasfilename(initialdir=currentDir, title='save file', defaultextension=".json", filetypes=(('json files', '*.json'), ('all files', '.*')))

        if filename == '':
            return

        print(filename)

        global start, end

        data = {}
        data['start'] = int(start + 1)
        data['end'] = int(end + 1)

        data['nodes'] = []
        for k in range(0, len(self.Maze.nodes)):
            node = {}
            node['idx'] = k + 1
            node['x'] = self.Maze.nodes[k][0]
            node['y'] = 900 - self.Maze.nodes[k][1]

            data['nodes'].append(node)

        data['edges'] = []
        for k in range(0, len(self.Maze.edges)):
            edge = {}
            edge['idx'] = k + 1
            edge['adjs'] = []
            for f in self.Maze.edges[k]:
                edge['adjs'].append(f + 1)

            data['edges'].append(edge)

        if 0 < len(self.Maze.hints):
            data['hint'] = []
            for k in self.Maze.hints:
                data['hint'].append(k + 1)

        with open(filename, 'w') as outfile:
            json.dump(data, outfile)

        pass

    def Mee2(self):
        currentDir = os.getcwd()
        folder = filedialog.askdirectory(initialdir=currentDir)

        print(folder)

        arr = os.listdir(folder)
        for filename in arr:
            print(filename)
            if filename.endswith('.json'):
                filepath = folder + '/' + filename
                print(filepath)
                jsondata = open(filepath).read()
                js = json.loads(jsondata)

                if 'alphabet' in js:
                    js = {}
                    js['null'] = ''

                    savepath = currentDir + '/' + filename
                    with open(savepath, 'w') as outfile:
                        json.dump(js, outfile)

    def Mee(self):
        currentDir = os.getcwd()
        folder = filedialog.askdirectory(initialdir=currentDir)

        print(folder)

        data = {}
        data['folder'] = 'maze/classic/'
        data['normal'] = 116
        data['hard'] = 236
        data['stages'] = []
        idxstart = 1

        arr = os.listdir(folder)
        for filename in arr:
            print(filename)
            if filename.endswith('.json'):
                filepath = folder + '/' + filename
                print(filepath)
                jsondata = open(filepath).read()
                js = json.loads(jsondata)

                if 'alphabet' in js:
                    d = {}
                    d['index'] = idxstart
                    d['alphabet'] = TRUE

                    data['stages'].append(d)
                else:
                    d = {}
                    d['index'] = idxstart
                    d['animal'] = 0

                    data['stages'].append(d)

                idxstart = idxstart + 1

        savepath = currentDir + '/' + 'output.json'
        with open(savepath, 'w') as outfile:
            json.dump(data, outfile)

    def ClearFunc(self):
        self.Maze.ClearGraph()
        self.RedrawMaze()

    def HintFunc(self):
        self.Maze.UpdateHint()
        self.RedrawMaze()

    def AutoFunc(self):
        self.Maze.AutoEdge()
        self.RedrawMaze()

    def ToggleTime(self):
        pass

    def StepNext(self):
        pass

    def KeyPressed(self, event):
        global mode, isEdge, selected, recent
        if event.char == '1':
            mode = 1
            selected = -1
            recent = -1
            self.t2.config(text='Select')
        elif event.char == '2':
            mode = 2
            selected = -1
            recent = -1
            self.t2.config(text='Node+')
        elif event.char == '3':
            mode = 3
            selected = -1
            recent = -1
            self.t2.config(text='Node-')
        elif event.char == '4':
            mode = 4
            selected = -1
            recent = -1
            self.t2.config(text='Edge+')
        elif event.char == '5':
            mode = 5
            selected = -1
            recent = -1
            self.t2.config(text='Edge-')

        if 0 <= selected:
            global start, end
            if event.char == 'q':
                start = selected
            elif event.char == 'w':
                end = selected

        self.RedrawMaze()
        isEdge = FALSE

    def KeyUp(self, event):
        global selected
        if 0 <= selected:
            self.Maze.nodes[selected][1] = self.Maze.nodes[selected][1] - 1
            self.t3.config(text=str(self.Maze.nodes[selected][0]))
            self.t5.config(text=str(self.Maze.nodes[selected][1]))
            self.RedrawMaze()

    def KeyDown(self, event):
        global selected
        if 0 <= selected:
            self.Maze.nodes[selected][1] = self.Maze.nodes[selected][1] + 1
            self.t3.config(text=str(self.Maze.nodes[selected][0]))
            self.t5.config(text=str(self.Maze.nodes[selected][1]))
            self.RedrawMaze()

    def KeyLeft(self, event):
        global selected
        if 0 <= selected:
            self.Maze.nodes[selected][0] = self.Maze.nodes[selected][0] - 1
            self.t3.config(text=str(self.Maze.nodes[selected][0]))
            self.t5.config(text=str(self.Maze.nodes[selected][1]))
            self.RedrawMaze()

    def KeyRight(self, event):
        global selected
        if 0 <= selected:
            self.Maze.nodes[selected][0] = self.Maze.nodes[selected][0] + 1
            self.t3.config(text=str(self.Maze.nodes[selected][0]))
            self.t5.config(text=str(self.Maze.nodes[selected][1]))
            self.RedrawMaze()

    def MouseDownL(self, event):
        self.canvas.focus_set()

        global mode

        x, y = event.x, event.y

        if mode == 1:
            dist = 9999
            sel = -1
            for k in range(0, len(self.Maze.nodes)):
                off_x = abs(self.Maze.nodes[k][0] - x)
                off_y = abs(self.Maze.nodes[k][1] - y)
                offset = math.sqrt(off_x * off_x + off_y * off_y)
                if offset < dist and offset <= 5:
                    sel = k
                    dist = offset

            global selected
            if selected != sel:
                selected = sel

                self.t3.config(text=str(self.Maze.nodes[selected][0]))
                self.t5.config(text=str(self.Maze.nodes[selected][1]))
                self.RedrawMaze()

        elif mode == 2:
            self.Maze.nodes.append([x, y])
            self.Maze.edges.append([])
            self.RedrawMaze()

        elif mode == 3:
            dist = 9999
            sel = -1
            for k in range(0, len(self.Maze.nodes)):
                off_x = abs(self.Maze.nodes[k][0] - x)
                off_y = abs(self.Maze.nodes[k][1] - y)
                offset = math.sqrt(off_x * off_x + off_y * off_y)
                if offset < dist and offset <= 5:
                    sel = k
                    dist = offset

            if sel != -1:
                for k in self.Maze.edges[sel]:
                    self.Maze.edges[k].remove(sel)
                for k in self.Maze.edges:
                    for f in range(0, len(k)):
                        if sel < k[f]:
                            k[f] = k[f] - 1

                del self.Maze.nodes[sel]
                del self.Maze.edges[sel]

                self.RedrawMaze()

        elif mode == 4:
            global recent, isEdge

            dist = 9999
            sel = -1
            for k in range(0, len(self.Maze.nodes)):
                off_x = abs(self.Maze.nodes[k][0] - x)
                off_y = abs(self.Maze.nodes[k][1] - y)
                offset = math.sqrt(off_x * off_x + off_y * off_y)
                if offset < dist and offset <= 5:
                    sel = k
                    dist = offset

            if sel != -1 and not sel == recent:
                if isEdge == FALSE:
                    recent = sel
                    isEdge = TRUE
                else:
                    if sel not in self.Maze.edges[recent]:
                        self.Maze.edges[recent].append(sel)
                    if recent not in self.Maze.edges[sel]:
                        self.Maze.edges[sel].append(recent)
                    recent = sel

                self.RedrawMaze()

        elif mode == 5:
            dist = 9999
            sel = -1
            for k in range(0, len(self.Maze.nodes)):
                off_x = abs(self.Maze.nodes[k][0] - x)
                off_y = abs(self.Maze.nodes[k][1] - y)
                offset = math.sqrt(off_x * off_x + off_y * off_y)
                if offset < dist and offset <= 5:
                    sel = k
                    dist = offset

            if sel != -1:
                for k in self.Maze.edges[sel]:
                    self.Maze.edges[k].remove(sel)
                del self.Maze.edges[sel][:]

                self.RedrawMaze()

        pass

    def MouseDownR(self, event):
        global recent, isEdge
        recent = -1
        isEdge = FALSE
        self.RedrawMaze()

    def MouseMove(self, event):
        global mode, recent, isEdge

        x, y = event.x, event.y

        if mode == 4 and isEdge == TRUE:
            dist = 9999
            sel = -1
            for k in range(0, len(self.Maze.nodes)):
                off_x = abs(self.Maze.nodes[k][0] - x)
                off_y = abs(self.Maze.nodes[k][1] - y)
                offset = math.sqrt(off_x * off_x + off_y * off_y)
                if offset < dist and offset <= 5:
                    sel = k
                    dist = offset

            if sel != -1 and not sel == recent:
                if sel not in self.Maze.edges[recent]:
                    self.Maze.edges[recent].append(sel)
                if recent not in self.Maze.edges[sel]:
                    self.Maze.edges[sel].append(recent)
                recent = sel

                self.RedrawMaze()

    def RedrawMaze(self):
        c = self.canvas
        for i in c.find_all():
            c.delete(i)

        global start, end, selected, recent

        m = self.Maze

        c.create_image(0, 0, image=self.BackgroundImage, anchor=NW)
        for k in range(0, len(m.nodes)):
            node = m.nodes[k]
            x, y = node[0], node[1]
            if k == start :
                c.create_oval(x - 4, y - 4, x + 4, y + 4, width=2, outline="blue")
                c.create_oval(x-100,y-100,x+100,y+100,width=1,outline='black')
            elif k == end :
                c.create_oval(x - 4, y - 4, x + 4, y + 4, width=2, outline="red")
            else :
                c.create_oval(x - 3, y - 3, x + 3, y + 3, width=1, outline="dimgray")

            if k == selected and 0 <= selected :
                c.create_oval(x - 5, y - 5, x + 5, y + 5, width=2, outline="thistle")
            if k == recent and 0 <= recent :
                c.create_oval(x - 5, y - 5, x + 5, y + 5, width=2, outline="green")

            c.create_text(x - 7, y - 7, fill='black', font='Times 10 bold', text=str(k))

        for k in range(0, len(m.edges)):
            adjs = m.edges[k]
            for f in adjs:
                x0, y0 = m.nodes[k][0], m.nodes[k][1]
                x1, y1 = m.nodes[f][0], m.nodes[f][1]
                if abs(y0 - y1) < abs(x0 - x1) :
                    if k < f:
                        c.create_line(x0, y0 - 3, x1, y1 - 3, width=1, fill="darkgray")
                    else:
                        c.create_line(x0, y0 + 3, x1, y1 + 3, width=1, fill="darkgray")
                else:
                    if k < f:
                        c.create_line(x0 - 3, y0, x1 - 3, y1, width=1, fill="darkgray")
                    else:
                        c.create_line(x0 + 3, y0, x1 + 3, y1, width=1, fill="darkgray")

        for k in range(0, len(m.hints)):
            h = m.hints[k]
            x, y = m.nodes[h][0], m.nodes[h][1]
            c.create_text(x + 7, y - 7, fill='blue', font='Times 10 bold', text=str(k))

root = Tk()
app = App(root)
root.mainloop()
