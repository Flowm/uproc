#!/usr/bin/env python3

import numpy as np
import argparse
import matplotlib.pyplot as plt
import csv


def main():
    cmd = argparse.ArgumentParser()
    cmd.add_argument("inputfile", nargs="+")
    cmd.add_argument("-o", "--outputfile")
    cmd.add_argument("-s", "--linesize", type=int, default=64)

    args = cmd.parse_args()

    plt.yscale('log')
    plt.xscale('log', basex=2)

    for ifile in args.inputfile:
        x = []
        y = []

        with open(ifile) as csvfile:
            plots = csv.reader(csvfile, delimiter=',')
            for row in plots:
                if not row[0].startswith("BM"):
                    continue

                x.append(int(row[0].split("/")[1]) * args.linesize)
                y.append(float(row[2]))

        plt.plot(x, y, label=ifile)

    plt.xlabel('block size')
    plt.ylabel('cpu time [ns]')
    plt.title('cache size benchmarking')
    plt.legend(loc="upper left")

    if not args.outputfile:
        plt.show()
    else:
        plt.savefig(args.outputfile, dpi=200)


if __name__ == "__main__":
    main()
