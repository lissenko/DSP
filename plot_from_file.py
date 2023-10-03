import sys
import matplotlib
import matplotlib.pyplot as plt

def produce_plot(filename):
    with open(filename, 'r') as file:
        arr = file.read().strip().split()
        arr = [float(value) for value in arr]
        plt.plot(arr)
        plt.ylim(-1, 1)
        plt.show()

if __name__ == '__main__':
    if len(sys.argv) != 2:
        print("Error: give name of the file")
    else:
        filename = sys.argv[1]
        produce_plot(filename)
