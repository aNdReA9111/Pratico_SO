#!/usr/bin/env python3.11

import subprocess as sub
import sys

if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else '.'
    dict = {}
    
    ls_output = sub.run(["ls", "-l", path], capture_output=True, text=True).stdout.split("\n")
    for i in range(1, len(ls_output)-1):
        
        entry = ls_output[i].split(" ")
        print(entry)
        
        if(entry[0][0] == '-'):
            if(entry[0] not in dict.keys()):
                dict[entry[0]] = [entry[-1]]
            else:
                dict[entry[0]].append(entry[-1])
                
    for (key, value) in dict.items():
        sub.run(["mkdir", key[1:]])
        for v in value:
            sub.run(["ln", "-s", v, key[1:] + "/" + v])
