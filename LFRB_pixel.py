"""
    This program was designed to test an algorithm to solve maze for micromouse.

    This algorithm solves maze on LFRB order. It explores map to reach finish point and returns back to inital state.
    On this process, it records two solutions for the maze. On reversing, the second solution, if it matches with first
    one, then the maze has only one solution, else multiple. At last, two solutions can be compared to find the shortest
    path which may be used for "the run".

    This is a python pseudo-code designed to finally implement on C/C++ in microcontroller.
"""
import cv2

maze = cv2.imread("LFRB_pixelmaze.png")
nmaze = maze

f, r, b, l = 0, 1, 2, 3 # n, e, s, w = 0, 1, 2, 3  ## these are the numbers to indicate which direction the bot is facing

start =  (0,1)
end = (128,127)
facing = r                          ## Enter start and end point and inital "facing" direction  ## initial facing must be set here for later
                                    # use while checking for pixels, in actual micromouse,setting appropriate value for "facing" is of no use

f_color = [100, 100, 100]
b_color = [0, 100, 0]

wall_color = [0,0,0]

def check_forward(cur_pos, facing):
    pos = (cur_pos[0] + facing%2*(1-2*int(facing/2)), cur_pos[1] +  (facing-1)%2*(facing-1))
    return maze[pos[1]][pos[0]] == wall_color

def check_left(cur_pos, facing):
    pos = (cur_pos[0] + (facing-1)%2*(facing-1), cur_pos[1] + (facing-2)%2*(facing-2))
    return maze[pos[1]][pos[0]] == wall_color

def check_right(cur_pos, facing):
    pos = (cur_pos[0] + (facing+1)%2*(1-2*int((facing+1)/2)), cur_pos[1] + facing%2*(1-2*int(facing/2)))
    return maze[pos[1]][pos[0]] == wall_color

def go_forward(cur_pos, facing):
    pos = (cur_pos[0] + facing%2*(1-2*int(facing/2)), cur_pos[1] +  (facing-1)%2*(facing-1))
    return pos, facing

def go_left(cur_pos, facing):
    pos = (cur_pos[0] + (facing-1)%2*(facing-1), cur_pos[1] + (facing-2)%2*(facing-2))
    return pos, (facing+l)%4

def go_right(cur_pos, facing):
    pos = (cur_pos[0] + (facing+1)%2*(1-2*int((facing+1)/2)), cur_pos[1] + facing%2*(1-2*int(facing/2)))
    return pos, (facing+r)%4

def go_back(cur_pos, facing):
    pos = (cur_pos[0]+(facing-2)%2*(facing-2), cur_pos[1]+(facing+1)%2*(1-2*int(facing/2)))
    return pos, (facing+b)%4

def find_path(cur_pos, facing, end_point, color):
    global maze, l, f, r, b
    stack = []
    while cur_pos != end_point:
        _l = check_left(cur_pos, facing).all()
        _f = check_forward(cur_pos, facing).all()
        _r = check_right(cur_pos, facing).all()
        if not _l:
            stack.append(l)
            maze[cur_pos[1]][cur_pos[0]] = color
            cur_pos, facing = go_left(cur_pos, facing)
        elif not _f:
            if not _r:  stack.append(f)
            maze[cur_pos[1]][cur_pos[0]] = color
            cur_pos, facing = go_forward(cur_pos, facing)
        elif not _r:
            stack.append(r)
            maze[cur_pos[1]][cur_pos[0]] = color
            cur_pos, facing = go_right(cur_pos, facing)
        else:
            stack.append(b)
            maze[cur_pos[1]][cur_pos[0]] = [0,0,200]
            cur_pos, facing = go_back(cur_pos, facing)

        if len(stack)>2 and stack[-2] == b and not(_l and _r):
            s1 = stack[-3]
            s3 = stack[-1]
            if (s1==1 and s3 == 3) or (s1==3 and s3==1):
                stack = stack[:-3]
                stack.append(b)
            elif s1+s3==6:
                stack = stack[:-3]
                stack.append(f)
            elif (s1==3 and s3==0) or (s1==0 and s3==3):
                stack = stack[:-3]
                stack.append(r)
            elif s1==s3==0:
                stack = stack[:-3]
                stack.append(b)
            else:
                print(stack,"\n", _l, _f, _r, " : ", cur_pos)
                raise Exception("Illegal value")

        cv2.imshow("maze", cv2.resize(maze, (768, 768), interpolation=cv2.INTER_NEAREST))
        if cv2.waitKey(1) == 27:
            break
    return stack, (facing+2)%4, cur_pos


def run(stack, cur_pos, facing):
    global maze
    while len(stack)!= 0:
        _l = check_left(cur_pos, facing).all()
        _f = check_forward(cur_pos, facing).all()
        _r = check_right(cur_pos, facing).all()

        if not(_l and _r):
            if stack[0]==0:
                cur_pos, facing = go_forward(cur_pos, facing)
                maze[cur_pos[1]][cur_pos[0]] = [255,255,0]
            elif stack[0]==1:
                cur_pos, facing = go_right(cur_pos, facing)
                maze[cur_pos[1]][cur_pos[0]] = [255,255,0]
            elif stack[0]==3:
                cur_pos, facing = go_left(cur_pos, facing)
                maze[cur_pos[1]][cur_pos[0]] = [255,255,0]
            else:
                raise Exception("Illegal direction during maze-run")
            stack.pop(0)
        else:
            cur_pos, facing = go_forward(cur_pos, facing)
            maze[cur_pos[1]][cur_pos[0]] = [255,255,0]

        cv2.imshow("maze", cv2.resize(maze, (768, 768), interpolation=cv2.INTER_NEAREST))
        if cv2.waitKey(1) == 27:
            break
    cv2.imshow("maze", cv2.resize(maze, (768, 768), interpolation=cv2.INTER_NEAREST))
    cv2.waitKey(5000)


def main():
    global maze, nmaze, facing
    cur_pos = start
    facing = r
    f_stack, facing, cur_pos = find_path(cur_pos, facing, end, f_color)
    print("First path : \n length : ", len(f_stack), "\n", f_stack)
    b_stack, facing, cur_pos = find_path(cur_pos, facing, start, b_color)
    b_stack = [(b_stack[len(b_stack)-1-i]+2)%4 if b_stack[len(b_stack)-1-i] != 0 else b_stack[len(b_stack)-1-i] for i in range(len(b_stack))]
    print("Retraced path : \n length : ", len(b_stack), "\n", b_stack)
    if len(f_stack)<=len(b_stack):
        run(f_stack, cur_pos, facing)
    else:
        run(b_stack, cur_pos, facing)

if __name__ == "__main__":
    main()
