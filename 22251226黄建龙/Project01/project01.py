from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *

import math
import numpy as np
import random
import glfw

# camera
camera_position_ = np.array([0.0, 0.0, 2.0])
camera_front_ = np.array([0.0, 0.0, -1.0])
camera_up_ = np.array([0.0, 1.0, 0.0])
camera_orientation_ = np.zeros(3)
Front_ = np.array([0.0, 0.0, -1.0])
yaw_ = -90.0
pitch_ = 0.0
fov_ = 45.0

# mouse state
firstMouse_ = True
lastX_ = 400.0
lastY_ = 300.0

deltaTime = 0.1

# pyramid
px = 0.0
py = 0.5
pz = -1.0
H = 0.5
R = 0.1

def normalize(x):
    x /= np.linalg.norm(x)
    return x

def display():
    global camera_up_, camera_front_, camera_position_, Front_, camera_orientation_
    global deltaTime
    global pitch_, yaw_, fov_
    global R, H, px, py, pz

    vertices = np.array([[px, 0.0 + H, pz],  # P0
                         [px + R, 0.0, pz],  # P1
                         [px + 0.5 * R, 0.0, pz - 0.5 * R * math.sqrt(3)],  # P2
                         [px - 0.5 * R, 0.0, pz - 0.5 * R * math.sqrt(3)],  # P3
                         [px - R, 0.0, pz],  # P4
                         [px - 0.5 * R, 0.0, pz + 0.5 * R * math.sqrt(3)],  # P5
                         [px + 0.5 * R, 0.0, pz + 0.5 * R * math.sqrt(3)]  # P6
                         ])

    faces = np.array([[0, 1, 2],
                      [0, 2, 3],
                      [0, 3, 4],
                      [0, 4, 5],
                      [0, 5, 6],
                      [0, 6, 1],
                      ])

    # Reset
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
    glLoadIdentity()


    # Reset the cameras
    camera_front_ = Front_ + camera_orientation_
    gluLookAt(camera_position_[0], camera_position_[1], camera_position_[2],
              camera_front_[0],   camera_front_[1],   camera_front_[2],
              camera_up_[0],      camera_up_[1],      camera_up_[2])

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
    for i in range(1, 7):
        glVertex3fv(vertices[i])
    glEnd()

    glutSwapBuffers()


def pressKey(key, x, y):
    global eyex, eyey, eyez
    global camera_up_, camera_front_, camera_position_
    global deltaTime
    global Front_

    cameraSpeed = 0.5 * deltaTime
    if key == b'w' or key == b'W':
        camera_position_[2] -= cameraSpeed
        Front_ = camera_position_ + np.array([0.0, 0.0, -3.0])
    if key == b's' or key == b'S':
        camera_position_[2] += cameraSpeed
        Front_ = camera_position_ + np.array([0.0, 0.0, -3.0])
    if key == b'A' or key == b'a':
        camera_position_[0] -= cameraSpeed
        Front_ = camera_position_ + np.array([0.0, 0.0, -3.0])
    if key == b'D' or key == b'd':
        camera_position_[0] += cameraSpeed
        Front_ = camera_position_ + np.array([0.0, 0.0, -3.0])

    if key == b'c' or key == b'C':
        global faceColors
        for i in range(6):
            for j in range(3):
                facesColors[i][j] = random.random()


def mouse_callback(xpos, ypos):
    global firstMouse_, lastY_, lastX_
    global yaw_, pitch_
    global camera_front_, camera_orientation_
    if firstMouse_:
        lastX_ = xpos
        lastY_ = ypos
        firstMouse_ = False

    _xoffset = xpos - lastX_
    _yoffset = lastY_ - ypos
    lastX_ = xpos
    lastY_ = ypos

    _sensitivity = 0.1
    _xoffset *= _sensitivity
    _yoffset *= _sensitivity

    yaw_ += _xoffset
    pitch_ += _yoffset

    if pitch_ > 89.0:
        pitch_ = 89.0
    if pitch_ < -89.0:
        pitch_ = -89.0

    _front = np.zeros(3)
    _front[0] = math.cos(math.radians(yaw_)) * math.cos(math.radians(pitch_))
    _front[1] = math.sin(math.radians(pitch_))
    _front[2] = math.sin(math.radians(yaw_)) * math.cos(math.radians(pitch_))
    camera_orientation_ = normalize(_front)



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
    global fov_
    glViewport(0, 0, w, h)
    glMatrixMode(GL_PROJECTION)
    glLoadIdentity()
    gluPerspective(fov_, w / h, 0.01, 100.0)
    glMatrixMode(GL_MODELVIEW)
    glLoadIdentity()


# Initialize global variables
w, h = 800, 600
u, v = 100, 100

facesColors = np.zeros((6, 3))

glutInit()
glutInitDisplayMode(GLUT_RGB |
                    GLUT_DOUBLE |
                    GLUT_DEPTH)

glutInitWindowSize(w, h)
glutInitWindowPosition(u, v)
glutCreateWindow("Project 01")
initRendering()
glutReshapeFunc(reshape, w, h)
glutKeyboardFunc(pressKey)
glutPassiveMotionFunc(mouse_callback)
glutDisplayFunc(display)
glutIdleFunc(idle)
glutMainLoop()