from random import random,randint,uniform 

def generate_tests():
    points_x = []
    n = randint(100, 128)
    k = randint(0, n)

    print(str(n) + " " + str(k))
    
    for i in range(n):
        while 1:
            x = uniform(0, 100)
            t = (x, 0)
            if t not in points_x:
                points_x.append(t)
                break

    points_x.sort()
    
    points_y = []

    for i in range(n):
        while 1:
            y = uniform(0, 100)
            t = (0, y)
            if t not in points_y:
                points_y.append(t)
                break

    points_y.sort()
    points_y = points_y[::-1]

    points = []

    for i in range(n):
        t = (points_x[i][0], points_y[i][1])
        points.append(t)

    #print(points)
    
    for i in range(n):
        t = points[i]
        print("%.12f %.12f" % (t[0], t[1]))
        #print(str(t[0]) + " " + str(t[1]))

if __name__ == '__main__':
    generate_tests()
