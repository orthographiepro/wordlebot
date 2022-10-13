import random
import numpy
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


file = open("5_letter_words.txt", 'r')
statistics = []
for c in range(26):
    statistics.append(pair(chr(c+97), 0))


for line in file:
    for c in line:
        if c == "\n":
            continue
        statistics[ord(c) - 97].second += 1

print(statistics)

statistics = sort_by_values(statistics)

print(statistics)

out = open("stats.txt", "w")
out.write(str(statistics))
out.write("\n\n")
for p in statistics:
    out.write("\'"+str(p.first) + "\', ")
out.close()

# plt.bar(statistics.values())
# plt.show()

file.close()
