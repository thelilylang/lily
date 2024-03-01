#!/bin/python3

import os
import sys
from glob import glob
from termcolor import colored
from dataclasses import dataclass


@dataclass
class OptionConfig:
    no_exit: bool


FILES = [
    y for x in os.walk("./tests/samples") for y in glob(os.path.join(x[0], "*.lily"))
]
COMMAND = "./bin/Debug/lilyc"
FILES_EXCLUDED = ["./tests/samples/package/a.lily", "./tests/samples/package/b.lily"]


def print_summary(success: int, failed: int) -> None:
    sys.stdout.write(
        f"\r\x1b[31mfailed: {failed}\x1b[0m, \x1b[32msuccess: {success}\x1b[0m, \x1b[30mtotal: {len(FILES) - len(FILES_EXCLUDED)}\x1b[0m"
    )
    sys.stdout.flush()


def parse_argv() -> OptionConfig:
    no_exit = False

    if len(sys.argv) > 1:
        for arg in sys.argv[1:]:
            match arg:
                case "--no-exit":
                    no_exit = True
                case "-h" | "--help":
                    print(
                        """Usage: ./scripts/test_samples.py [options]

Options:

  --no-exit        Force to not exit(1), even if there are failed tests
  -h, --help       Print help"""
                    )
                    exit(0)
                case other:
                    print(f"error: wrong argument: `{arg}`")
                    exit(1)

    return OptionConfig(no_exit)


if __name__ == "__main__":
    option_config = parse_argv()
    success = 0
    failed = 0

    print_summary(success, failed)

    for file in FILES:
        if file in FILES_EXCLUDED:
            continue

        status = os.system(COMMAND + " " + file)

        if status == 0:
            success += 1
        else:
            failed += 1

        print_summary(success, failed)

    if not option_config.no_exit:
        if failed > 0:
            exit(1)

    print("")
