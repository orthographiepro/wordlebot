import random
import numpy as np
from matplotlib import pyplot as plt
import random as rd

class pair:
    def __init__(self, a, b) -> None:
        self.first = a
        self.second = b

    def __str__(self) -> str:
        return " " + str(self.first) + ":" + str(self.second) + " "

    def __repr__(self) -> str:
        return " " + str(self.first) + ":" + str(self.second) + " "


def sort_by_values(list) -> list:
    if len(list) == 1:
        return list
    
    lo = list[0: len(list)//2]
    hi = list[len(list)//2: ]

    lo = sort_by_values(lo)
    hi = sort_by_values(hi)

    newlist = []

    while len(lo) >= 1 or len(hi) >= 1:
        if len(lo) >= 1 and len(hi) >= 1:
            if lo[0].second >= hi[0].second:
                tmp = lo.pop(0)
            else:
                tmp = hi.pop(0)
        elif len(lo) == 0:
                tmp = hi.pop(0)
        else:
            tmp = lo.pop(0)
        newlist.append(tmp)

    return newlist


def checksum(statistics: dict):
    sum = 0
    for val in statistics:
        sum += statistics[val]
    print(sum, sum / 5)


def analyze_in_total():    
    file = open("5_letter_words.txt", 'r')
    statistics = []
    for c in range(26):
        statistics.append(pair(chr(c+97), 0))

    for line in file:
        for c in line:
            if c == "\n":
                continue
            statistics[ord(c) - 97].second += 1

    file.close()

    out = open("stats_total.txt", "w")
    out.write(str(statistics))
    out.write("\n\n")
    for p in statistics:
        out.write("\'"+str(p.first) + "\', ")
    out.close()

    statistics = sort_by_values(statistics)
    return statistics


def analyze_by_position():
    file = open("5_letter_words.txt", 'r')
    statistics = np.zeros((26, 6))
    
    for line in file:
        for i in range(len(line) - 1): 
            statistics[ord(line[i]) - 97, i] += 1
            statistics[ord(line[i]) - 97, 5] += 1 # total
    

    labels = []
    for x in range(26):
        labels.append(chr(x+97))

    figure, a = plt.subplots(6, 1)

    for i in range(6):
        a[i].bar(labels, statistics[:, i])
    plt.show()
    file.close()

    out = open("stats_byPosition.txt", "w")
    for i in range(6):
        if i == 5:
            out.write("Sorted by total frequency: ")
        else:
            out.write("Sorted by frequency in line "+str(i)+": ")
        pair_array = []
        for c in range(26):
            pair_array.append(pair(chr(c+97), statistics[c, i]))
        pair_array = sort_by_values(pair_array)
        out.write("{")
        for p in pair_array:
            out.write("\'"+str(p.first) + "\', ")
        out.write("\b\b}\n\n")
    out.close()

    return statistics


# main:
analyze_by_position()