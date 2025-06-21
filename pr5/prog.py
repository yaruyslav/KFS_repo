import matplotlib.pyplot as plt
import random
import copy


NUM_CHROMOSOME = 10
CHROMOSOME = (0,2)
IDEAL = [1,2,2,0,0,0,0,1,1,2]
NUM_INDIVIDUAL = 20
MAX_GENERATION = 1000
MUTATION_CH =  0.1
CROSSOVER_CH = 0.6

class Individual:
    def __init__(self, arr):
        self.arr = arr

class With_fitness:
    def __init__(self, ind, val):
        self.ind = ind
        self.fit = val

def rand_chromos():
    chromosomes = []
    for i in range(NUM_CHROMOSOME):
        chromosomes.append(random.randint(CHROMOSOME[0],CHROMOSOME[1]))
    return chromosomes

def ind_list_init(ind_list):
    ind_list.clear()
    for i in range(NUM_INDIVIDUAL):
        ind_list.append(Individual(rand_chromos()))

def rand_without(num):
    ran = random.randint(CHROMOSOME[0],CHROMOSOME[1])
    while ran==num:
        ran = random.randint(CHROMOSOME[0],CHROMOSOME[1])
    return ran

def mutation(ind):
    r_num = random.randint(0,NUM_CHROMOSOME-1)
    ind[r_num] = rand_without(ind[r_num])

def crossover(ind1, ind2):
    r_num = random.randint(0,NUM_CHROMOSOME-1)

    for i in range(NUM_CHROMOSOME):
        if i>=r_num:
            buf = ind1[i]
            ind1[i] = ind2[i]
            ind2[i] = buf

def fitness(ind):
    val = 0
    i = 0
    while i < NUM_CHROMOSOME:
        if ind[i]==IDEAL[i]:
            val = val + 1
        i = i + 1
    return val

def tournament(inp_list):
    wth_fitnum_list = []
    for el in inp_list:
        wth_fitnum_list.append(With_fitness(el, fitness(el.arr)))

    sorted_arr = sorted(wth_fitnum_list, key=lambda x: x.fit, reverse=True)
    return [obj.ind for obj in sorted_arr]


def main():
    ind_list = []
    ind_list_init(ind_list)

    print("\tПочаткові індивіди:")
    num = 0
    for el in [obj.arr for obj in ind_list]:
        print(str(num)+") ", el)
        num = num + 1

    iteration = 0
    best_ind = None
    while iteration < MAX_GENERATION:
        iteration = iteration + 1
        num = 0
        for el in [obj.arr for obj in ind_list]:
            if num>2:
                if (NUM_INDIVIDUAL%2==1 and (num+1)%2==0) or (NUM_INDIVIDUAL%2==0 and (num+1)%2==1):
                    arr = [obj.arr for obj in ind_list]
                    if (float(random.randint(0,10))/10)<CROSSOVER_CH:
                        crossover(arr[num], arr[num+1])
            if (float(random.randint(0,10))/10)<MUTATION_CH:
                mutation(el)
            num = num + 1

        ind_list = tournament(ind_list)
        if (best_ind is None) or (fitness(ind_list[0].arr) > fitness(best_ind.arr)):
            best_ind = copy.deepcopy(ind_list[0])
        if fitness(best_ind.arr) == NUM_CHROMOSOME:
            break

    print("\tКінцеві дані:")
    print("Ітерації:", iteration)
    print("\tІндивіди")
    num = 0
    for el in [obj.arr for obj in ind_list]:
        print(str(num)+") ", el)
        num = num + 1

    fig, ax = plt.subplots()

    ax.plot(best_ind.arr, label='Найкращий відібраний')
    ax.plot(IDEAL, label='Найкращий індивід')
    ax.text(0.5,0,"Ітерації: "+str(iteration)+", із збігом: "+str(float(fitness(best_ind.arr)/NUM_CHROMOSOME*100))+"%", fontsize=15)

    ax.legend()

    plt.show()


if __name__=='__main__':
    main()
