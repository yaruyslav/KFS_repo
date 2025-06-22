import matplotlib.pyplot as plt
import matplotlib.animation as animation
import random
import math


SIZE_FIELD = (100, 100)
NUM_NODES = 5
INIT_PHEROM = 2
NUM_MOVES = 40
ALPHA = 1
BETA = 1
EVAPORATION = 0.8

class Node:
    def __init__(self, el1, el2):
        self.point = [el1, el2]

class Way:
    def __init__(self, arr1, arr2, num):
        self.connect = [arr1, arr2]
        self.distance = num
        self.pherom = 0


def get_dist(p1, p2):
    coord1 = p1.point
    coord2 = p2.point
    return math.sqrt(pow((coord2[0]-coord1[0]),2)+pow((coord2[1]-coord1[1]),2))

def init_nodes(nodes):
    for i in range(NUM_NODES):
        r1 = random.randint(0, SIZE_FIELD[0])
        r2 = random.randint(0, SIZE_FIELD[1])
        if [r1, r2] not in [obj.point for obj in nodes]:
            nodes.append(Node(r1, r2))
        else:
            for num in range(50):
                r1 = random.randint(0, SIZE_FIELD[0])
                r2 = random.randint(0, SIZE_FIELD[1])
                if [r1, r2] not in [obj.point for obj in nodes]:
                    nodes.append(Node(r1, r2))
                    break
    return True

def init_ways(ways, nodes):
    i = 0
    while i < len(nodes)-1:
        j = i + 1
        while j < len(nodes):
            ways.append(Way(nodes[i], nodes[j], get_dist(nodes[i], nodes[j])))
            j = j + 1
        i = i + 1

def init_pheromones(ways):
    for el in ways:
        el.pherom = INIT_PHEROM


def compare_ways(el1, el2):
    if el1[0].point==el2[0].point and el1[1].point==el2[1].point:
        return True
    elif el1[0].point==el2[1].point and el1[1].point==el2[0].point:
        return True
    return False

def get_chances(p1, points, ways):
    chanches = []
    sum = 0
    for el1 in points:
        way = None
        for el2 in ways:
            if compare_ways(el2.connect, [p1, el1]):
                way = el2
        if way is not None:
            var = pow(way.pherom,ALPHA)*pow(1.0/way.distance,BETA)
            chanches.append(var)
            sum = sum + var
        else:
            chanches.append(0)
    return [float(obj)/float(sum) for obj in chanches]

def get_next_points(point, ways):
    arr = []
    for el in ways:
        if point.point==el.connect[0].point:
            arr.append(el.connect[1])
        elif point.point==el.connect[1].point:
            arr.append(el.connect[0])
    return arr

variable_s = 0
def delete_way(ways, p1, p2):
    global variable_s
    variable_s = variable_s + 1
    for i in range(len(ways)):
        cmp1 = ways[i].connect[0].point
        cmp2 = ways[i].connect[1].point
        if (p1.point==cmp1 and p2.point==cmp2) or (p1.point==cmp2 and p2.point==cmp1):
            ways.pop(i)
            return
    return

variable_a = 0
def delete_allways(ways, p1):
    global variable_a
    variable_a = variable_a + 1
    new_arr = []
    for i in range(len(ways)):
        cmp1 = ways[i].connect[0].point
        cmp2 = ways[i].connect[1].point
        if (p1.point==cmp1) or (p1.point==cmp2):
            new_arr.append(ways[i])

    for el1 in new_arr:
        for i in range(len(ways)):
            if el1.connect[0].point==ways[i].connect[0].point and el1.connect[1].point==ways[i].connect[1].point:
                ways.pop(i)
                break
    return

def compare(p1, p2):
    if p1.point == p2.point:
        return True
    return False

def get_way(p1, p2, ways):
    for el in ways:
        if compare_ways([p1,p2], el.connect):
            return el
    return None



def main():
    nodes_list = []
    ways_list = []

    value1 = init_nodes(nodes_list)
    if not value1:
        print("wrong initialisation!")
        return
    init_ways(ways_list, nodes_list)
    init_pheromones(ways_list)

    print("\tПочаткові дані:")
    print("\tТочки:")
    buf = [obj.point for obj in nodes_list]
    for i in range(len(buf)):
        print(str(i+1)+")",buf[i])

    print("\tШляхи:")
    buf = [[obj.connect[0].point, obj.connect[1].point, obj.distance, obj.pherom] for obj in ways_list]
    for i in range(len(buf)):
        print(str(i+1)+")",buf[i][0],"-",buf[i][1]," має відстань ",f"{buf[i][2]:.2f}"," та феромони ",f"{buf[i][3]:.2f}")

    def print_last():
        nonlocal ways_list
        print("\tКінцеві дані шляхів:")
        buf = [[obj.connect[0].point, obj.connect[1].point, obj.distance, obj.pherom] for obj in ways_list]
        for i in range(len(buf)):
            print(str(i + 1) + ")", buf[i][0], "-", buf[i][1], " має феромони ",f"{buf[i][3]:.2f}")

    def init_func():
        nonlocal data1, data2, data3, data4
        data1.set_data(data1.get_data())
        data2.set_data(data2.get_data())
        data3.set_data(data3.get_data())
        data4.set_text(data4.get_text())
        return [data1, data2, data3, data4]

    save_iter = 0
    arr_best = []
    state = 0
    def update(frame):
        nonlocal arr_best, state, save_iter
        nonlocal data1, data2, data3, data4
        nonlocal arr_init_x, arr_init_y, arr_fw_x, arr_fw_y
        if frame >= NUM_MOVES:
            print_last()
            data4.set_text(data4.get_text()+"\nНайкраща ітерація "+str(save_iter)+" із шляхом "+f"{arr_best[0]:.2f}"+" одн.")
            return [data1, data2, data3, data4]

        aviable_ways = ways_list.copy()
        full_way = []
        full_way.append(random.choice(nodes_list))

        len_avi_w = len(aviable_ways)
        sum_ways = 0
        for j in range(len_avi_w):
            curr_point = full_way[j]
            aviable_points = get_next_points(curr_point, aviable_ways)
            if len(aviable_points)!=1:
                for k in range(len(aviable_points)):
                    if aviable_points[k].point==full_way[0].point:
                        aviable_points.pop(k)
                        break

            arr_chances = get_chances(curr_point, aviable_points, aviable_ways)

            r_num = 0
            if len(aviable_points) > 1:
                num = random.random()
                sum = 0
                for i in range(len(arr_chances)):
                    sum = sum + arr_chances[i]
                    if num < sum:
                        r_num = i
                        break

            rand_point = aviable_points[r_num]

            curr_way = get_way(curr_point, rand_point, aviable_ways)
            if curr_way is not None:
                curr_way.pherom = curr_way.pherom + 1
                sum_ways = sum_ways + curr_way.distance
            else:
                print("curr way is none!")

            full_way.append(rand_point)
            if full_way[0].point == curr_point.point:
                delete_way(aviable_ways, curr_point, rand_point)
            else:
                delete_allways(aviable_ways, curr_point)

            if compare(full_way[0], rand_point):
                break

        for el in ways_list:
            el.pherom = el.pherom * EVAPORATION

        arr_fw_x = [obj.point[0] for obj in full_way]
        arr_fw_y = [obj.point[1] for obj in full_way]

        data1.set_data(arr_init_x, arr_init_y)
        data2.set_data([arr_fw_x[0]], [arr_fw_y[0]])
        data3.set_data(arr_fw_x, arr_fw_y)
        data4.set_text("Ітерація: "+str(frame+1)+", із шляхом: "+f"{sum_ways:.2f}"+" одиниць")

        if frame < 5:
            arr_best.insert(0,sum_ways)
        elif arr_best[0]>=sum_ways:
            arr_best.insert(0, sum_ways)
            arr_best.pop()
            save_iter = frame + 1

        state = 0
        last = None
        if len(arr_best)==5:
            for el in arr_best:
                if last is None:
                    last = el
                elif last==el:
                    state = 1
                else:
                    state = 0
                    break
        if state:
            nonlocal anim
            print_last()
            data4.set_text(data4.get_text()+"\nНайкраща ітерація "+str(save_iter)+" із шляхом "+f"{arr_best[0]:.2f}"+" одн.")
            anim.event_source.stop()

        return [data1, data2, data3, data4]

    arr_init_x = [obj.point[0] for obj in nodes_list]
    arr_init_y = [obj.point[1] for obj in nodes_list]
    arr_fw_x = []
    arr_fw_y = []

    fig, ax = plt.subplots()

    plt.title = "hello"
    plt.grid(which='major')
    plt.grid(which='minor', linestyle=':')
    ax.set_xlim(-5, SIZE_FIELD[0]+5)
    ax.set_ylim(-5, SIZE_FIELD[1]+5)

    data1, = ax.plot(arr_init_x, arr_init_y, 'o', markersize=5)
    data2, = ax.plot([arr_init_x[0]], [arr_init_y[0]], 'o', color='red', markersize=5)
    data3, = ax.plot(arr_fw_x, arr_fw_y, '-')
    data4 = ax.text(1,1,"Ітерація: "+str(0)+", із шляхом: "+str(0)+" одиниць", fontsize=13)

    anim = animation.FuncAnimation(
        fig=fig,
        func=update,
        init_func=init_func,
        frames=NUM_MOVES+1,
        interval=500,
        repeat=False,
    )

    plt.show()


if __name__=="__main__":
    main()
