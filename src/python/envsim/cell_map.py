#!/usr/bin/env python

import os
import sys
import copy

import numpy as np


class Cell(object):
    def __init__(self, x, y):
        self.id = -1
        self.position = np.array([x, y])
        self.occupied = False

    def print_info(self):
        print "{}: {}, {}, {}".format("Cell info", self.id, self.position, self.occupied)


class Map(object):
    def __init__(self, x, y):
        self.size = np.array([x, y])
        self.cells = np.array([], dtype=object)
        self.cells.resize((x, y))

        for yi in range(0, y):
            for xi in range(0, x):
                id = yi * x + xi
                self.cells[xi, yi] = Cell(xi, yi)
                self.cells[xi, yi].id = id

    def print_info(self):
        print "{}: {}".format("Map info", self.size)

        for yi in range(0, self.size[1]):
            for xi in range(0, self.size[0]):
                self.cells[xi, yi].print_info()
