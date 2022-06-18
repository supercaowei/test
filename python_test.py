#!/usr/bin/python3
# -*- coding: UTF-8 -*-
import os, sys, argparse, re

if __name__ == "__main__":
    dir = './TestModules'
    for file in os.listdir(dir):
        print(dir + ", " + file)
        path = os.path.join(dir, file)
        if os.path.isdir(path):
            print(path + " is a dir")
        elif os.path.isfile(path):
            print(path + " is a file")
        else:
            print(path + " is a ?")
