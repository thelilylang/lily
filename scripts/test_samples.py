#!/bin/python

import os
import sys
from glob import glob
from termcolor import colored

FILES = [y for x in os.walk("./tests/samples") for y in glob(os.path.join(x[0], '*.lily'))]
COMMAND = "./build/Debug/lily compile"

def print_summary(success: int, failed: int) -> None:
    sys.stdout.write(f"\r\x1b[31mfailed: {failed}\x1b[0m, \x1b[32msuccess: {success}\x1b[0m, \x1b[30mtotal: {len(FILES)}\x1b[0m")
    sys.stdout.flush()

if __name__ == "__main__":
    success = 0
    failed = 0

    print_summary(success, failed)

    for file in FILES:
        status = os.system(COMMAND + " " + file)

        if status == 0:
            success += 1
        else:
            failed += 1

        print_summary(success, failed)

    print("")