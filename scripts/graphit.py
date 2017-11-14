import csv
import sys

import matplotlib.pyplot as plt


def file_len(fname):
    with open(fname) as f:
        for i, l in enumerate(f):
            pass
    return i + 1


def main():
    input_filepath = sys.argv[1]
    output_filepath = sys.argv[2]

    x = []
    y1 = []
    y2 = []
    header = None

    with open(input_filepath, 'r') as csvfile:
        plots = csv.reader(csvfile, delimiter=',')
        for (index, row) in enumerate(plots):
            if index == 0:
                header = row
            else:
            # elif (index % sample_rate == 0):
                x.append(int(row[0]))
                y1.append(int(row[1]))
                y2.append(int(row[2]))

    plt.figure(figsize=(10, 7))  # 10" x 10"
    # plt.plot(x,y1, 'ko', label=header[1], linestyle='None', markersize=1)
    plt.plot(x, y1, 'k-', label=header[1])
    # plt.plot(x,y2, 'k+', label=header[2], linestyle='None', markersize=1)
    plt.plot(x, y2, 'r-', label=header[2])
    plt.xlabel(header[0])
    plt.ylabel('Organism count')
    plt.title('Organism counts over time')
    plt.legend()

    print('plotting...')
    # plt.show()  # uncomment for displaying in GUI
    plt.savefig(output_filepath)
    print("Wrote: {}".format(output_filepath))

if __name__ == '__main__':
    main()
