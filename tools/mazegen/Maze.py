# -*- coding: cp949 -*-
# Tkinter 테스트

from tkinter import filedialog
from tkinter import *
from datetime import datetime
import random
import json
import os

CANVAS_WIDTH = 650
CANVAS_HEIGHT = 650
DEFAULT_MAP_WIDTH = 20
DEFAULT_MAP_HEIGHT = 20

class Maze:
    width = 10
    height = 10
    cells = []
    hints = []

    def __init__(self):
        pass

    def CreateMaze(self, width, height):
        # initialize
        self.width = width
        self.height = height
        del self.cells[:]
        del self.hints[:]

        for x in range(0,width):
            for y in range(0,height):
                self.cells.append( 15 )

        visited = [0]
        self.TraverseDepthFirst( visited )

    def CreateCleanMaze(self, width, height):
        # initialize
        self.width = width
        self.height = height
        del self.cells[:]
        del self.hints[:]

        for x in range(0,width):
            for y in range(0,height):
                self.cells.append( 15 )

    def CreateHint(self):
        isIceMaze = FALSE
        w = self.width
        h = self.height

        for x in range(0,w):
            for y in range(0,h):
                data = self.GetCell(x,y)
                if data & 16:
                    isIceMaze = TRUE
                    break
            if isIceMaze == TRUE:
                break

        if isIceMaze == FALSE:
            del self.hints[:]
            global startX, startY, endX, endY
            self.hints = self.GetHint(self.cells, self.GetCellCode(startX, startY), self.GetCellCode(endX, endY))

    def GetHint(self, maze, start, end):
        queue = [(start, [start])]
        result = []
        dxy = [(0, -1), (1, 0), (0, 1), (-1, 0)]
        dcode = [1, 2, 4, 8]

        while queue:
            n, path = queue.pop(0)
            if n == end:
                result.append(path)
                break
            else:
                graph = set([])
                for k in range(4):
                    if not (maze[n] & dcode[k]):
                        x, y = self.GetXYFromCellCode(n)
                        x += dxy[k][0]
                        y += dxy[k][1]
                        f = self.GetCellCode(x, y)
                        graph.add(f)

                for m in graph - set(path):
                    queue.append((m, path + [m]))

        if len(result) == 0:
            print('no hint')
            return []

        print('yes hint')
        return result[0]

    def TraverseDepthFirst( self, visited ):

        while len(visited) > 0:
            code = visited[ len(visited)-1 ]    # peeking
            x, y = self.GetXYFromCellCode( code )

            naver = []
            if y > 0 and self.IsNotVisited(x,y-1):               naver.append( 0 )
            if x < self.width-1 and self.IsNotVisited(x+1,y):    naver.append( 1 )
            if y < self.height-1 and self.IsNotVisited(x,y+1):   naver.append( 2 )
            if x > 0 and self.IsNotVisited(x-1,y):               naver.append( 3 )

            # if all neighbour is visited, go back
            if len(naver)==0:
                visited.pop()
                continue

            # choose random direction/length
            direction = naver[ random.randint(0, len(naver)-1) ]
            length = random.randint(1, 2)

            # visit all cell
            dxy = [ (0,-1), (1,0), (0,1), (-1,0) ]
            for i in range(length):
                x2 = x + dxy[direction][0]
                y2 = y + dxy[direction][1]
                if self.IsNotVisited(x2,y2):
                    visited.append( self.GetCellCode(x2,y2) )
                    self.BreakWall(x,y,direction)
                    x,y = (x2,y2)
                else:
                    break

    def BreakWall(self, x, y, direction):
        dxy = [ (0,-1), (1,0), (0,1), (-1,0) ]
        x2 = x + dxy[direction][0]
        y2 = y + dxy[direction][1]

        if direction==0:
            self.SetCell(x,y, self.GetCell(x,y) & ~1)
            self.SetCell(x2, y2, self.GetCell(x2,y2) & ~4)
        elif direction==1:
            self.SetCell(x,y, self.GetCell(x,y) & ~2)
            self.SetCell(x2, y2, self.GetCell(x2,y2) & ~8)
        elif direction==2:
            self.SetCell(x,y, self.GetCell(x,y) & ~4)
            self.SetCell(x2, y2, self.GetCell(x2,y2) & ~1)
        elif direction==3:
            self.SetCell(x,y, self.GetCell(x,y) & ~8)
            self.SetCell(x2, y2, self.GetCell(x2,y2) & ~2)


    def GetCellCode(self, x, y):
        return self.width*y + x

    def GetXYFromCellCode(self, code):
        y = int(code / self.width)
        x = int(code % self.width)
        return (x,y)

    def SetCell(self, x, y, flag):
        self.cells[ self.width*y + x ] = flag

    def GetCell(self, x, y):
        return self.cells[ self.width*y + x ]

    def IsBlocked(self, x, y, direction):
        filter = 1 << direction
        return (self.GetCell(x, y) & filter ) != 0

    def IsNotVisited(self, x, y):
        if x<0 or x>=self.width: return False
        if y<0 or y>=self.height: return False

        return self.GetCell(x,y)==15





class App:
 
    def __init__(self, master):
        frame = Frame(master)
        frame.pack()

        self.canvas = Canvas(frame, dict(width=CANVAS_WIDTH, height=CANVAS_HEIGHT))
        self.canvas.bind("<Key>", self.KeyPressed)
        self.canvas.bind("<Button-1>", self.MouseDown)
        self.canvas.bind("<ButtonRelease-1>", self.MouseUp)
        self.canvas.bind("<B1-Motion>", self.MouseMove)
        self.canvas.pack()

        self.canvas.focus_set()

        item = self.canvas.create_line(0, 0, 100, 100, tags="uno")

        global startX, startY, endX, endY, mode
        startX = 0
        startY = 0
        endX = 0
        endY = 0
        mode = 1

        f = Frame(master)
        f.pack()

        t = Label(f, text="Mode:")
        t.pack(side=LEFT)
        self.t2 = Label(f, text="Maze")
        self.t2.pack(side=LEFT)

        #btnMee = Button(f, text="Mee", command=self.Mee)
        #btnMee.pack(side=LEFT)

        btnLoad = Button(f, text="Load", command=self.Load)
        btnLoad.pack(side=LEFT)

        btnSave = Button(f, text="Save", command=self.Save)
        btnSave.pack(side=LEFT)

        s = Button(f, text="QuickSave", command=self.QSave)
        s.pack( side=LEFT )

        b = Button(f, text="Generate", command=self.Generate)
        b.pack(side=LEFT)

        b2 = Button(f, text="ClearMaze", command=self.GenerateClean)
        b2.pack(side=LEFT)

        # size
        self.strWidth = StringVar()
        self.strHeight = StringVar()
        self.strTime = StringVar()
        self.checkVar = IntVar()

        l = Label(f, text="  size:")
        l.pack( side=LEFT )

        e = Entry(f, dict(textvariable=self.strWidth, width=3) )
        e.pack( side=LEFT)
        self.strWidth.set( str(DEFAULT_MAP_WIDTH) )

        l = Label(f, text="x")
        l.pack( side=LEFT )

        e = Entry(f, dict(textvariable=self.strHeight, width=3) )
        e.pack( side=LEFT)
        self.strHeight.set( str(DEFAULT_MAP_HEIGHT) )

        c = Checkbutton(f, text="time", variable=self.checkVar, command=self.ToggleTime)
        c.pack( side=LEFT)

        e = Entry(f, dict(textvariable=self.strTime, width=3))
        e.pack(side=LEFT)
        self.strTime.set(str(0))

        l3 = Label(f, text=" ")
        l3.pack(side=LEFT)

        b3 = Button(f, text="MakeHint", command=self.CreateHint)
        b3.pack(side=LEFT)

        b4 = Button(f, text="ClearHint", command=self.ClearHint)
        b4.pack(side=LEFT)

        # create maze
        self.Maze = Maze()

        w = int(self.strWidth.get())
        h = int(self.strHeight.get())
        self.Maze.CreateCleanMaze(w,h)
        self.RedrawMaze()

    def Mee(self):
        currentDir = os.getcwd()
        folder = filedialog.askdirectory(initialdir=currentDir)

        print(folder)

        arr = os.listdir(folder)
        for filename in arr:
            print(filename)
            if filename.endswith('.json'):
                filepath = folder + '/' + filename
                print(filepath)
                global startX, startY, endX, endY
                jsondata = open(filepath).read()
                js = json.loads(jsondata)

                width = int(js['size'][0])
                height = int(js['size'][1])
                startX = int(js['start'][0])
                startY = height - int(js['start'][1]) - 1
                endX = int(js['end'][0])
                endY = height - int(js['end'][1]) - 1

                self.strWidth.set(width)
                self.strHeight.set(height)
                self.GenerateClean()

                for y in range(0, self.Maze.height):
                    for x in range(0, self.Maze.width):
                        idx = self.Maze.GetCellCode(x, y)
                        self.Maze.SetCell(x, height - y - 1, int(js['maze'][idx]))

                del self.Maze.hints[:]
                if 'hint' in js:
                    for k in js['hint']:
                        x = k[0]
                        y = height - k[1] - 1
                        f = self.Maze.GetCellCode(x, y)
                        self.Maze.hints.append(f)

                self.strTime.set(str(30))
                self.checkVar.set(1)

                w = width
                h = height

                data = {}
                data['size'] = []
                data['size'].append(w)
                data['size'].append(h)
                data['start'] = []
                data['start'].append(startX)
                data['start'].append(h - startY - 1)
                data['end'] = []
                data['end'].append(endX)
                data['end'].append(h - endY - 1)

                data['maze'] = []
                for y in range(0, h):
                    for x in range(0, w):
                        idx = x + (h - y - 1) * w
                        data['maze'].append(int(self.Maze.cells[idx]))

                if 0 < len(self.Maze.hints):
                    print('hint exist')
                    data['hint'] = []
                    for k in self.Maze.hints:
                        temp = []
                        x, y = self.Maze.GetXYFromCellCode(k)
                        temp.append(x)
                        temp.append(h - y - 1)
                        data['hint'].append(temp)

                if self.checkVar.get() == 1 and 0 < int(self.strTime.get()):
                    data['time'] = int(self.strTime.get())

                savepath = currentDir + '/' + filename
                with open(savepath, 'w') as outfile:
                    json.dump(data, outfile)

    def Load(self):
        currentDir = os.getcwd()
        filename = filedialog.askopenfilename(initialdir=currentDir, title='select file',
                                              filetypes=(('json files', '*.json'), ('text files', '*.txt')))

        if os.path.isfile(filename) == FALSE:
            return

        global startX, startY, endX, endY
        jsondata = open(filename).read()
        data = json.loads(jsondata)

        width = int(data['size'][0])
        height = int(data['size'][1])
        startX = int(data['start'][0])
        startY = height - int(data['start'][1]) - 1
        endX = int(data['end'][0])
        endY = height - int(data['end'][1]) - 1

        self.strWidth.set(width)
        self.strHeight.set(height)
        self.GenerateClean()

        for y in range(0, self.Maze.height):
            for x in range(0, self.Maze.width):
                idx = self.Maze.GetCellCode(x, y)
                self.Maze.SetCell(x, height - y - 1, int(data['maze'][idx]))

        del self.Maze.hints[:]
        if 'hint' in data:
            for k in data['hint']:
                x = k[0]
                y = height - k[1] - 1
                f = self.Maze.GetCellCode(x,y)
                self.Maze.hints.append(f)

        if 'time' in data:
            time = int(data['time'])
            self.strTime.set(str(time))
            self.checkVar.set(1)

        self.RedrawMaze()

        pass

    def Save(self):
        currentDir = os.getcwd()
        filename = filedialog.asksaveasfilename(initialdir=currentDir, title='save file', defaultextension=".json", filetypes=(('json files', '*.json'), ('all files', '.*')))

        if filename == '':
            return

        print(filename)

        global startX, startY, endX, endY

        w = self.Maze.width
        h = self.Maze.height

        data = {}
        data['size'] = []
        data['size'].append(w)
        data['size'].append(h)
        data['start'] = []
        data['start'].append(startX)
        data['start'].append(h - startY - 1)
        data['end'] = []
        data['end'].append(endX)
        data['end'].append(h - endY - 1)

        data['maze'] = []
        for y in range(0, h):
            for x in range(0, w):
                idx = x + (h - y - 1) * w
                data['maze'].append(int(self.Maze.cells[idx]))

        if 0 < len(self.Maze.hints):
            data['hint'] = []
            for k in self.Maze.hints:
                temp = []
                x, y = self.Maze.GetXYFromCellCode(k)
                temp.append(x)
                temp.append(h - y - 1)
                data['hint'].append(temp)

        if self.checkVar.get() == 1 and 0 < int(self.strTime.get()):
            data['time'] = int(self.strTime.get())

        with open(filename, 'w') as outfile:
            json.dump(data, outfile)

        pass

    def QSave(self):
        now = datetime.now()
        filename = '%04d%02d%02d%02d%02d%02d.json' % (now.year, now.month, now.day, now.hour, now.minute, now.second)

        global startX, startY, endX, endY

        w = self.Maze.width
        h = self.Maze.height

        data = {}
        data['size'] = []
        data['size'].append(w)
        data['size'].append(h)
        data['start'] = []
        data['start'].append(startX)
        data['start'].append(h - startY - 1)
        data['end'] = []
        data['end'].append(endX)
        data['end'].append(h - endY - 1)

        data['maze'] = []
        for y in range(0, h):
            for x in range(0, w):
                idx = x + (h - y - 1) * w
                data['maze'].append(int(self.Maze.cells[idx]))

        if 0 < len(self.Maze.hints):
            data['hint'] = []
            for k in self.Maze.hints:
                temp = []
                x, y = self.Maze.GetXYFromCellCode(k)
                temp.append(x)
                temp.append(h - y - 1)
                data['hint'].append(temp)

        if self.checkVar.get() == 1 and 0 < int(self.strTime.get()):
            data['time'] = int(self.strTime.get())

        with open(filename, 'w') as outfile:
            json.dump(data, outfile)

        pass

    def Generate(self):
        w = int(self.strWidth.get())
        h = int(self.strHeight.get())
        self.Maze.CreateMaze(w,h)
        self.checkVar.set(0)
        self.strTime.set(str(0))
        self.RedrawMaze()

    def GenerateClean(self):
        w = int(self.strWidth.get())
        h = int(self.strHeight.get())
        self.Maze.CreateCleanMaze(w,h)
        self.checkVar.set(0)
        self.strTime.set(str(0))
        self.RedrawMaze()

    def CreateHint(self):
        self.Maze.CreateHint()
        self.RedrawMaze()

    def ClearHint(self):
        del self.Maze.hints[:]
        self.RedrawMaze()

    def ToggleTime(self):
        pass

    def StepNext(self):
        pass

    def KeyPressed(self, event):
        global mode
        if event.char == '1':
            mode = 1
            self.t2.config(text='Maze')
        elif event.char == '2':
            mode = 2
            self.t2.config(text='Start')
        elif event.char == '3':
            mode = 3
            self.t2.config(text='End')
        elif event.char == '4':
            mode = 4
            self.t2.config(text='Ice')
        elif event.char == '5':
            mode = 5
            self.t2.config(text='Hint+')
        elif event.char == '6':
            mode = 6
            self.t2.config(text='Hint-')

    def MouseDown(self, event):
        global x1, y1, isDown, mode
        global startX, startY, endX, endY

        self.canvas.focus_set()

        x, y = event.x, event.y
        m = self.Maze
        CW = (CANVAS_WIDTH - 8) / m.width
        CH = (CANVAS_HEIGHT - 8) / m.height
        if CW > CH:
            CW = CH
        else:
            CH = CW

        margin_w = int((CANVAS_WIDTH - CW * m.width) / 2) + 2
        margin_h = int((CANVAS_HEIGHT - CH * m.height) / 2)

        x = x - margin_w
        y = y - margin_h
        x1 = int(x / CW)
        y1 = int(y / CH)

        if mode == 2:
            startX = x1
            startY = y1
            self.RedrawMaze()
            return
        elif mode == 3:
            endX = x1
            endY = y1
            self.RedrawMaze()
            return
        elif mode == 4:
            if self.Maze.GetCell(x1, y1) & 16:
                self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) & ~16)
            else:
                self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) | 16)
            self.RedrawMaze()
            return
        elif mode == 5:
            f = self.Maze.GetCellCode(x1, y1)
            self.Maze.hints.append(f)
            self.RedrawMaze()
            return
        elif mode == 6:
            for k in self.Maze.hints:
                x,y = self.Maze.GetXYFromCellCode(k)
                if x1 == x and y1 == y:
                    self.Maze.hints.remove(k)
                    break
            self.RedrawMaze()
            return

        isDown = TRUE

    def MouseUp(self, event):
        global isDown
        isDown = FALSE

    def MouseMove(self, event):
        global isDown
        if isDown is TRUE:
            x, y = event.x, event.y

            m = self.Maze
            CW = (CANVAS_WIDTH - 8) / m.width
            CH = (CANVAS_HEIGHT - 8) / m.height
            if CW > CH:
                CW = CH
            else:
                CH = CW

            margin_w = int((CANVAS_WIDTH - CW * m.width) / 2) + 2
            margin_h = int((CANVAS_HEIGHT - CH * m.height) / 2)

            x = x - margin_w
            y = y - margin_h
            x2 = int(x / CW)
            y2 = int(y / CH)

            if x1 == x2 and y1 == y2:
                return
            if 1 < abs(x1-x2) + abs(y1-y2):
                return

            isChanged = FALSE

            dxy = [(0, -1), (1, 0), (0, 1), (-1, 0)]
            for dir in range(4):
                xt = x1 + dxy[dir][0]
                yt = y1 + dxy[dir][1]

                if(xt == x2 and yt == y2):
                    if dir == 0:
                        if self.Maze.GetCell(x1, y1) & 1:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) & ~1)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) & ~4)
                            isChanged = TRUE
                        else:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) | 1)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) | 4)
                            isChanged = TRUE
                    elif dir == 1:
                        if self.Maze.GetCell(x1, y1) & 2:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) & ~2)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) & ~8)
                            isChanged = TRUE
                        else:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) | 2)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) | 8)
                            isChanged = TRUE
                    elif dir == 2:
                        if self.Maze.GetCell(x1, y1) & 4:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) & ~4)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) & ~1)
                            isChanged = TRUE
                        else:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) | 4)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) | 1)
                            isChanged = TRUE
                    elif dir == 3:
                        if (self.Maze.GetCell(x1, y1) & 8):
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) & ~8)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) & ~2)
                            isChanged = TRUE
                        else:
                            self.Maze.SetCell(x1, y1, self.Maze.GetCell(x1, y1) | 8)
                            self.Maze.SetCell(x2, y2, self.Maze.GetCell(x2, y2) | 2)
                            isChanged = TRUE

            if isChanged == TRUE:
                isDown = FALSE
                self.RedrawMaze()

    def RedrawMaze(self):
        c = self.canvas
        for i in c.find_all():
            c.delete(i)

        global startX, startY, endX, endY

        m = self.Maze
        CW = (CANVAS_WIDTH-8) / m.width
        CH = (CANVAS_HEIGHT-8) / m.height
        if CW > CH: CW = CH 
        else: CH = CW

        margin_w = int( (CANVAS_WIDTH - CW*m.width) / 2 ) +2
        margin_h = int( (CANVAS_HEIGHT - CH*m.height) / 2 )

        for x in range(m.width):
            for y in range(m.height):
                xx = x * CW + margin_w
                yy = y * CH + margin_h

                if m.IsBlocked(x, y, 4):    c.create_rectangle(xx, yy, xx + CW, yy + CH,
                                                               outline='skyblue', fill='sky blue')
                if m.IsBlocked(x, y, 0):    c.create_line(xx,yy, xx+CW,yy)
                else:
                    c.create_line(xx, yy, xx + CW, yy, width=1, fill="lightgray", dash=(4, 2))
                if m.IsBlocked(x, y, 1):    c.create_line(xx+CW-1,yy, xx+CW-1,yy+CH)
                else:
                    c.create_line(xx+CW-1,yy, xx+CW-1,yy+CH, width=1, fill="lightgray", dash=(4, 2))
                if m.IsBlocked(x, y, 2):    c.create_line(xx,yy+CH-1, xx+CW,yy+CH-1)
                else:
                    c.create_line(xx,yy+CH-1, xx+CW,yy+CH-1, width=1, fill="lightgray", dash=(4, 2))
                if m.IsBlocked(x, y, 3):    c.create_line(xx,yy, xx,yy+CH)
                else:
                    c.create_line(xx,yy, xx,yy+CH, width=1, fill="lightgray", dash=(4, 2))
                if (startX == x and startY == y):  c.create_text(xx+CW*0.7, yy+CH*0.7, fill='red', font='Times 10 italic bold', text='S')
                if (endX == x and endY == y):  c.create_text(xx+CW*0.7, yy+CH*0.7, fill='red', font='Times 10 italic bold', text='E')

        count = 1
        stackarr = []
        for i in range(m.width * m.height):
            stackarr.append(0)

        for k in m.hints:
            x, y = m.GetXYFromCellCode(k)
            xx = x * CW + margin_w
            yy = y * CH + margin_h
            idx = m.width * y + x
            stack = int(stackarr[idx])
            stackarr[idx] += 1
            c.create_text(xx+CW*0.3, yy + CH * 0.2 + CH * 0.3 * stack, fill='black', font='Times 10 bold', text=str(count))
            count += 1

root = Tk()
app = App(root)
root.mainloop()
