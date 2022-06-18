#!/usr/local/bin/python3
import time, datetime

if __name__ == "__main__":
    while (True):
        print("\r",str(datetime.datetime.utcnow()),end="",flush = True)
        time.sleep(0.01)
