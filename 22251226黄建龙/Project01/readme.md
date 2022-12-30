Programs run on Python 3.8.3  Windows 10 21H2

1. pip install -r requirements.txt
1. run project01.py




#### Some suggestions for installing pyopengl and  pyopengl_accelerate packages:

##### Mac OS X / Linux:

1. If working with a native Python, please use pip command as

​        pip install PyOpenGL PyOpenGL_accelerate

2. If working with a Conda-based package (e.g., Anaconda, Miniconda), please use conda command as

​       conda install PyOpenGL PyOpenGL_accelerate



**Windows 7 / 8 / 10 / 11**:

1. To avoid missing GLUT .dll file, you are highly recommended to work with a native Python, and access the following url:

   - https://www.lfd.uci.edu/~gohlke/pythonlibs/#pyopengl
     and download the following two .whl files
   - PyOpenGL‑3.1.6‑cp**AB**‑cp**AB**‑**win_amd64**.whl
   - PyOpenGL_accelerate‑3.1.6‑cp**AB**‑cp**AB**‑**win_amd64**.whl
     where **A** is the Python's major version, and **B** the minor version (e.g., if the version is 3.8.1, then AB is 38). If your Python is 32-bit, please replace **win_amd64** with **win32**.

2. Having the two .whl files, use

   pip install path_to_the_whl_file.whl





##### 交互方式

WASD        控制摄像头移动

鼠标移动     控制摄像头可视角度
