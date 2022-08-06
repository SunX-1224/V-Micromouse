import pygame
import cv2

WIN = pygame.display.set_mode((774,774))
BG = (100,100,100)
maze = pygame.image.load("linemaze.png").convert_alpha()

lnm = cv2.imread("linemaze.png")

class Bot:

    def __init__(self, pos, facing):
        self.pos = pos
        self.facing = 0
        self.size = (24,24)
        self.surf = pygame.Surface(self.size)
        self.snsr_pos = [(0,6),(6,0),(12,0),(18,0),(24, 6)]
        self.sensor_value = [0,0,0,0,0]
        self.speed = 6
        self.on = True
        self.map = []

        self.update_sensor(facing)
        self.check()


    def draw(self, win):
        win.blit(self.surf, self.pos)

    def check(self):
        gsp = [(self.pos[0]+self.snsr_pos[i][0], self.pos[1]+self.snsr_pos[i][1]) for i in range(len(self.snsr_pos))]
        for i,pos in enumerate(gsp):
            if (lnm[pos[1]][pos[0]] == [0,0,0]).all():
                self.sensor_value[i] = 1
            else:
                self.sensor_value[i] = 0

    def update_sensor(self, turn):
        self.surf.fill((100, 100, 100))
        self.facing = (self.facing+turn)%4
        nsensor = []
        for i, sensor in enumerate(self.snsr_pos):
            if turn == 1:
                sensor = (24-sensor[1], sensor[0])
            elif turn == 2:
                sensor = (24-sensor[0], 24-sensor[1])
            elif turn == 3:
                sensor = (sensor[1], 24-sensor[0])
            nsensor.append(sensor)
            pygame.draw.rect(self.surf, (0,0,255), (sensor[0]-2*(6<=sensor[0]<=18)-4*(sensor[0]==24), sensor[1]-4*(sensor[1]==24), 4, 4))
        self.snsr_pos = nsensor

    def update_pos(self):
        self.pos = (self.pos[0] + self.facing%2*(1-2*int(self.facing/2))*self.speed, self.pos[1] +  (self.facing-1)%2*(self.facing-1)*self.speed)

    def pid(self):
        if self.sensor_value[1] == 1:
            self.pos = (self.pos[0]+(self.facing-1)%2-2*(self.facing==0), self.pos[1]+self.facing%2-2*(self.facing==1))
        elif self.sensor_value[3] == 1:
            self.pos = (self.pos[0] - (self.facing - 1) % 2 + 2 * (self.facing == 0), self.pos[1] - self.facing % 2 + 2 * (self.facing == 1))


    def __mapping(self):
        if self.sensor_value == [1, 1, 1, 1, 1]: self.on = False
        if not self.on: return
        self.check()
        if self.sensor_value[0] == 1:
            self.update_sensor(3)
            self.map.append(3)
        elif self.sensor_value[1] == 1 or self.sensor_value[2] == 1 or self.sensor_value[3] == 1:
            self.update_sensor(0)
            self.pid()
            if self.sensor_value[4] == 1: self.map.append(0)
            self.update_pos()
        elif self.sensor_value[4] == 1:
            self.update_sensor(1)
            self.map.append(1)
        else:
            self.update_sensor(2)
            self.map.append(2)

    def map_maze(self, WIN, speed):
        clock = pygame.time.Clock()
        run = True
        while run:
            for event in pygame.event.get():
                if event.type == pygame.QUIT or not self.on:
                    run = False

                if event.type == pygame.KEYDOWN:
                    dx = (event.key == pygame.K_d) * self.speed - (event.key == pygame.K_a) * self.speed
                    dy = (event.key == pygame.K_s) * self.speed - (event.key == pygame.K_w) * self.speed
                    self.pos = (self.pos[0] + dx, self.pos[1] + dy)

            WIN.fill(BG)
            WIN.blit(maze, (0, 0))
            self.__mapping()
            self.draw(WIN)
            pygame.display.update()
            clock.tick(speed)

    def search(self):
        while 2 in self.map:
            index = self.map.index(2)
            s1 = self.map[index-1]
            s3 = self.map[index+1]
            self.map.pop(index)
            self.map.pop(index)
            if (s1 == 1 and s3 == 3) or (s1 == 3 and s3 == 1) or (s1 == s3 == 0):
                self.map[index-1] = 2
            elif (s1 == 3 and s3 == 0) or (s1 == 0 and s3 == 3):
                self.map[index-1] = 1
            elif s1 == s3 == 3:
                self.map[index-1] = 0
            else:
                print(s1, s3, index)
                raise Exception("Illegal value")

    def __run(self):
        if len(self.map)==0: return
        if not self.on: return
        self.check()
        if not(self.sensor_value[0]==0 and self.sensor_value[4]==0):
            s = self.map.pop(0)
            if s==0:
                self.pid()
                self.update_sensor(0)
                self.update_pos()
            elif s==1:
                self.update_sensor(1)
            elif s==3:
                self.update_sensor(3)
            else:
                print(s)
                raise Exception("Illegal direction during maze run")

        else:
            self.pid()
            self.update_sensor(0)
            self.update_pos()


    def run(self, WIN, speed):
        clock = pygame.time.Clock()
        run = True
        while run:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    run = False
            WIN.fill(BG)
            WIN.blit(maze, (0,0))
            self.__run()
            self.draw(WIN)
            pygame.display.update()
            clock.tick(speed)


def main():
    speed = 150
    start = (350,1)
    facing = 2
    bot = Bot(start, facing)
    bot.map_maze(WIN, speed)
    bot.on = True
    bot.pos = start
    bot.facing = 0
    bot.snsr_pos = [(0,6),(6,0),(12,0),(18,0),(24, 6)]
    bot.update_sensor(facing)
    bot.check()
    speed = 120
    print(bot.map)
    bot.search()
    print(bot.map)
    bot.run(WIN, speed)

    pygame.quit()

if __name__ == "__main__":
    main()
