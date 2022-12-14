from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

import math
import numpy as np
import random

def display():
    global R, H, px, py, pz
    global isPressX, isPressY, isPressZ
    global degx, degy, degz

    vertices = np.array([       [px,             0.0 + H,      pz],                                # P0
                                [px + R,         0.0,    pz],                                # P1
                                [px + 0.5 * R,   0.0,    pz - 0.5 * R * math.sqrt(3)],       # P2
                                [px - 0.5 * R,   0.0,    pz - 0.5 * R * math.sqrt(3)],       # P3
                                [px - R,         0.0,    pz],                                # P4
                                [px - 0.5 * R,   0.0,    pz + 0.5 * R * math.sqrt(3)],       # P5
                                [px + 0.5 * R,   0.0,    pz + 0.5 * R * math.sqrt(3)]        # P6
                                ])

    faces = np.array([          [0, 1, 2],
                                [0, 2, 3],
                                [0, 3, 4],
                                [0, 4, 5],
                                [0, 5, 6],
                                [0, 6, 1],
                                ])

    # Reset
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()

    # Reset the camera
    gluLookAt(1.0, -0.5, 1.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0)

    # check whether or not receving a key input: x, y or z
    if isPressX == True:
        glTranslatef(0.0, 0.0, -3.0)
        glRotate(degx, 1.0, 0.0, 0.0)
        degx += 1.0/10

    if isPressY == True:
        glTranslatef(0.0, 0.0, -3.0)
        glRotate(degy, 0.0, 1.0, 0.0)
        degy += 1.0/10

    if isPressZ == True:
        glTranslatef(0.0, 0.0, -1.0)
        glRotate(degz, 0.0, 0.0, 1.0)
        degz += 1.0/10

    # Draw the six side faces
    glBegin(GL_POLYGON)
    for i in range(6):
        glColor3f(facesColors[i][0], facesColors[i][1], facesColors[i][2])
        face = faces[i]
        glVertex3fv(vertices[face[0]])
        glVertex3fv(vertices[face[1]])
        glVertex3fv(vertices[face[2]])
    glEnd()

    # draw the base
    glBegin(GL_POLYGON)
    glColor3f(1.0, 1.0, 1.0)
    for i in range(1,7):
        glVertex3fv(vertices[i])
    glEnd()

    glutSwapBuffers()


def pressKey(key, x, y):
    global R, H
    global isPressX, isPressY, isPressZ
    if key == b'r' or key == b'R':
        R += 0.01
    if key == b'h' or key == b'H':
        H -= 0.01
        if H <= 0.0:
            H = 0.0
    if key == b'x' or key == b'X':
        if isPressX == False:
            isPressX = True
        else:
            isPressX = False
    if key == b'y' or key == b'Y':
        if isPressY == False:
            isPressY = True
        else:
            isPressY = False
    if key == b'z' or key == b'Z':
        if isPressZ == False:
            isPressZ = True
        else:
            isPressZ = False
    if key == b'c' or key == b'C':
        global faceColors
        for i in range(6):
            for j in range(3):
                facesColors[i][j] = random.random()


def idle():
    display()


def initRendering():
    glEnable(GL_DEPTH_TEST)
    # glEnable(GL_LIGHTING)
    # glEnable(GL_LIGHT0)
    global faceColors
    for i in range(6):
        for j in range(3):
            facesColors[i][j] = random.random()


def reshape(w, h):
    glViewport(0, 0, w, h)

    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(35.0, w / h, 0.01, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()

# read params from txt file
f = open("pyramid-params.txt", "r")
params = f.read()
params = params.replace('\n', '').split(",")
params_list = []
params_list.extend(params)

# Initialize global variables
w, h = 400, 400
u, v = 100, 100
px = float(params_list[0])
py = float(params_list[1])
pz = float(params_list[2])
H = float(params_list[3])
R = float(params_list[4])
isPressX = False
isPressY = False
isPressZ = False
degx = 0.0
degy = 0.0
degz = 0.0
facesColors = np.zeros((6, 3))

glutInit()
glutInitDisplayMode(GLUT_RGB |
                    GLUT_DOUBLE |
                    GLUT_DEPTH)

glutInitWindowSize(w, h)
glutInitWindowPosition(u, v)
glutCreateWindow("Hexagonal Pyramid")

initRendering()

glutReshapeFunc(reshape, w, h)
glutDisplayFunc(display)
glutIdleFunc(idle)
glutKeyboardFunc(pressKey)
glutMainLoop()