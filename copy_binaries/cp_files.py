#!/usr/bin/env python3

import os
import shutil

from pathlib import Path

ffmpeg_str =  str(Path.cwd().parent / "FFmpeg")
target_loc = str(Path.cwd() / "cp_build")

print(ffmpeg_str)
print(target_loc)

with open("ffmpeg locations.txt") as textfile1, open("mingw64 locations.txt") as textfile2: 
    for x, y in zip(textfile1, textfile2):
        x = x.strip()[1:] #.replace('/', '\\')
        f = open(ffmpeg_str+x, 'r')
        y = y.strip()[1:] #.replace('/', '\\')
        if not os.path.exists(target_loc+y):
            # if the directory is not present 
            # then create it.
            os.makedirs(target_loc+y)
        shutil.copy(ffmpeg_str+x, target_loc+y)
