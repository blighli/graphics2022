from model import *
import glm


class Scene:
    def __init__(self, app):
        self.app = app
        self.objects = []
        self.load()
        # skybox
        self.skybox = AdvancedSkyBox(app)
        self.is_rotated = False

    def add_object(self, obj):
        self.objects.append(obj)

    def load(self):
        app = self.app
        add = self.add_object

        # floor
        n, s = 20, 2
        for x in range(-n, n, s):
            for z in range(-n, n, s):
                add(Cube(app, pos=(x, -s, z)))

        # columns
        for i in range(9):
            add(Cube(app, pos=(15, i * s, -9 + i), tex_id=2))
            add(Cube(app, pos=(15, i * s, 5 - i), tex_id=2))

        # cat
        self.cat = Cat(app, pos=(0, -1, -10))
        add(self.cat)

        # # wolf
        # add(Wolf(app, pos=(0, -1, 10)))

        # moving cube
        self.moving_cube = MovingCube(app, pos=(0, 6, 8), scale=(3, 3, 3), tex_id=1)
        add(self.moving_cube)

    def update(self):
        self.moving_cube.rot.xyz = self.app.time

        keys = pg.key.get_pressed()
        pos_z = self.cat.pos[2]
        pos_y = self.cat.pos[1]
        pos_x = self.cat.pos[0]
        scale_x = self.cat.scale[0]
        scale_y = self.cat.scale[1]
        scale_z = self.cat.scale[2]

        if keys[pg.K_i]:
            pos_z += 0.1
            self.cat.pos = (self.cat.pos[0], self.cat.pos[1], pos_z)
        if keys[pg.K_j]:
            pos_z -= 0.1
            self.cat.pos = (self.cat.pos[0], self.cat.pos[1], pos_z)
        if keys[pg.K_k]:
            pos_x += 0.1
            self.cat.pos = (pos_x, self.cat.pos[1], self.cat.pos[2])
        if keys[pg.K_l]:
            pos_x -= 0.1
            self.cat.pos = (pos_x, self.cat.pos[1], self.cat.pos[2])
        if keys[pg.K_UP]:
            pos_y += 0.1
            self.cat.pos = (self.cat.pos[0], pos_y, self.cat.pos[2])
        if keys[pg.K_DOWN]:
            pos_y -= 0.1
            self.cat.pos = (self.cat.pos[0], pos_y, self.cat.pos[2])
        if keys[pg.K_y]:
            self.cat.rot.y += 0.1
        if keys[pg.K_x]:
            self.cat.rot.x += 0.1
        if keys[pg.K_z]:
            self.cat.rot.z += 0.1


        if keys[pg.K_u]:
            scale_x -= 0.03
            scale_y -= 0.03
            scale_z -= 0.03
            self.cat.scale = (scale_x, scale_y, scale_z)

        if keys[pg.K_o]:
            scale_x += 0.03
            scale_y += 0.03
            scale_z += 0.03
            self.cat.scale = (scale_x, scale_y, scale_z)

