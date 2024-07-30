#!/usr/bin/env python3

import sys
import subprocess
import os

def get_sha1sum(file):
    sha1sum = subprocess.check_output(['sha1sum', file])
    return sha1sum.split()[0].decode('utf-8')

def is_file(file):
    return os.path.isfile(file)

def get_files(dir):
    process = subprocess.run(["find", dir, "-maxdepth", "1"], capture_output=True, text=True)
    files_and_dirs = process.stdout.splitlines()
    files = []
    for f in files_and_dirs:
        if is_file(f):
            files.append(f)
    return files

def remove_file(file):
    os.remove(file)


if __name__ == '__main__':
    if len(sys.argv) != 3:
        print('Usage: python es3.py <dir1> <dir2>')
        sys.exit(1)

    dir1 = sys.argv[1]
    dir2 = sys.argv[2]

    files1 = get_files(dir1)
    files2 = get_files(dir2)


    dict1 = {}
    dict2 = {}
    to_delete = []

    for f in files1:
        sha1 = get_sha1sum(f)
        if(sha1 not in dict1.keys()):
            dict1[sha1] = [f]
        else:
            dict1[sha1].append(f)

    for f in files2:
        sha1 = get_sha1sum(f)
        if(sha1 in dict1.keys()):
            to_delete.append(sha1)
            remove_file(f)

    for hash in to_delete:
        for f in dict1[hash]:
            remove_file(f)
            dict1[hash].remove(f)
            if len(dict1[hash]) == 0:
                dict1.pop(hash)
            