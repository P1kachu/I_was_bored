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
import contextlib
import shutil
import lzma

OUTPUT_DIR = "qee_output"
AVOID=['txt',
        'jpeg',
        'jpg',
        'png',
        'html',
        'ascii text']

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

def handle_7z(f):
    print_verbose("{0} Handling 7z file ({1})".format(green("[+]"), f))
    try:
        p = sub.call(['7z', 'x', '-y', '-o' + OUTPUT_DIR, f],stdout=sub.PIPE,stderr=sub.PIPE)
        print("{0} Extraction done: {1}".format(green('[+]'), f))
    except Exception as e:
        print("{0} Extraction fail: {1}".format(red("[-]"), f))
        print_verbose("{0} Error: {1}".format(red("[-]"), e))
        return 1
    return 0


def handle_tarxzgzip(f):
    print_verbose("{0} Handling TAR/XZ/GZIP file ({1})".format(green("[+]"), f))
    try:
        tar = tarfile.open(f)
        tar.extractall(OUTPUT_DIR)
        tar.close()
        print("{0} Extraction done: {1}".format(green('[+]'), f))
    except Exception as e:
        # Some files are tar.xz, others are only .xz
        # Handling of .xz here
        try:
            with lzma.open(f) as f2:
                out = open('{0}/{1}_extracted'.format(OUTPUT_DIR, f), 'wb')
                out.write(f2.read())
        except Exception as e2:
            print("{0} Extraction fail: {1}".format(red("[-]"), f))
            print_verbose("{0} Error: {1}".format(red("[-]"), e2))
            return 1
    return 0


def stats(nb_of_files, nb_of_fails, nb_of_unknowns):
    print("")
    success = nb_of_files - nb_of_fails - nb_of_unknowns
    p = success / nb_of_files * 100
    print("[ ] Archives opened: {0}/{1} ({2}%)".format(success, nb_of_files, p))
    print("[ ] Files with unkown extensions: {0}".format(nb_of_unknowns))


def cut_output(output):
    if len(output) > 80:
        output = output[:80] + "..."

    return output

def decompress(args):

    # Every file or only one
    if args.file:
        files = [args.file]
    else:
        files = [f for f in os.listdir('.') if os.path.isfile(f)]

    # Output directory
    if args.override:
        shutil.rmtree(OUTPUT_DIR)
    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)
    else:
        print("{0} Directory '{1}' already exists.".format(red('[-]'), OUTPUT_DIR))
        print("{0} Relaunch with '-o' to override".format(red('[-]')))
        exit(-1)

    print_verbose("[ ] Extracting {0} files to {1}".format(len(files), OUTPUT_DIR))
    print_verbose("")

    nb_of_fails    = 0
    nb_of_unknowns = 0
    for f in files:
        p = sub.Popen(['file', f],stdout=sub.PIPE,stderr=sub.PIPE)
        output, errors = p.communicate()
        output = parse_file_output(output).lower()
        output = cut_output(output)

        # ADD EXTENSIONS HERE
        # Could factorise this,
        # but easier to read like
        # that
        if [x for x in AVOID if x in output]:
            print_verbose("[ ] Not an archive, skipping ({0})".format(f))
            continue
        if "tar" in  output:
            nb_of_fails += handle_tarxzgzip(f)
        elif "xz" in output:
            nb_of_fails += handle_tarxzgzip(f)
        elif "gzip" in output:
            nb_of_fails += handle_tarxzgzip(f)
        elif "zip" in output:
            nb_of_fails += handle_7z(f)
        else:
            nb_of_unknowns += 1
            print_verbose("[ ] {0}: Unkown file type ({1})".format(f, output))

        print_verbose("-" * 82)

    stats(len(files), nb_of_fails, nb_of_unknowns)


if __name__ in "__main__":
    parser = argparse.ArgumentParser(description="")
    parser.add_argument('-s', '--silent', help='Sets verbose mode off', action='store_true')
    parser.add_argument('-f', '--file', help='file to extract')
    parser.add_argument('-o', '--override', help='override if necessary', action='store_true')
    args = parser.parse_args()

    decompress(args)


