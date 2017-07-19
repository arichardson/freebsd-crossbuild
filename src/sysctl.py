#!/usr/bin/env python3
import argparse
import sys

parser = argparse.ArgumentParser(prog="sysctl",
            description="A fake sysctl for FreeBSD boostrapping")
parser.add_argument("-n", action="store_true")
parser.add_argument("sysctl_name")
args = parser.parse_args()

value_only = args.n
name = args.sysctl_name
if name == "kern.bootfile":
    result = "/boot/kernel/kernel"
else:
    sys.exit("UNKNOWN SYSCTL: " + name)

if value_only:
    print(result)
else:
    print(name, ": ", result, sep="")
