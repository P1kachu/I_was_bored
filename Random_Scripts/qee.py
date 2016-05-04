#!/usr/bin/python3

################################################################################
#
# QEE - Quickly Extract Everything
# P1kachu 2016
# Small utility to extract every archives file at once in a folder (WIP)
#
################################################################################

import os
import argparse
import tarfile
import subprocess as sub
import shutil

OUTPUT_DIR = "qee_output"


def red(string):
    return "\033[31;1m{0}\033[0m".format(string)

def green(string):
    return "\033[32;1m{0}\033[0m".format(string)


def parse_file_output(output):
    output = output.decode("utf-8").rstrip()
    semicolon = output.find(':') + 2
    output = output[semicolon:]
    return output


def print_verbose(string):
    if not args.silent:
        print(string)


def handle_tarxz(f):
    print_verbose("{0} Handling TAR/XZ file ({1})".format(green("[+]"), f))
    try:
        tar = tarfile.open(f)
        tar.extractall(OUTPUT_DIR)
        tar.close()
    except:
        print("{0} failed to open {1}".format(red("[-]"), f))

def decompress(args):
    if args.file:
        files = args.file
    else:
        files = [f for f in os.listdir('.') if os.path.isfile(f)]

    if args.override:
        shutil.rmtree(OUTPUT_DIR)


    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)
    else:
        print("{0} Directory '{1}' already exists. Rename it or delete it".format(
            red('[-]'), OUTPUT_DIR))
        exit(-1)


    for f in files:
        p = sub.Popen(['file', f],stdout=sub.PIPE,stderr=sub.PIPE)
        output, errors = p.communicate()
        output = parse_file_output(output).lower()[:80]


               # ADD EXTENSIONS HERE
        if "tar" in  output:
            handle_tarxz(f)
        elif "xz" in output:
            handle_tarxz(f)
        else:
            print_verbose("[ ] Unkown file type: {0}".format(output))



if __name__ in "__main__":
    parser = argparse.ArgumentParser(description="")
    parser.add_argument('-s', '--silent', help='Sets verbose mode off', action='store_true')
    parser.add_argument('-f', '--file', help='file to extract')
    parser.add_argument('-o', '--override', help='override if necessary', action='store_true')
    args = parser.parse_args()

    decompress(args)


