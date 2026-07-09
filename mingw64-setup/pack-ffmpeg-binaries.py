#!/usr/bin/env python3

from pathlib import Path
import shutil

ffmpeg_dir =  str(Path.cwd().parent / "FFmpeg")
target_dir = str(Path.cwd() / "mingw64-ffmpeg-build")

print(ffmpeg_dir)
print(target_dir)

with open("ffmpeg-paths.txt") as textfile1, open("mingw64-paths.txt") as textfile2: 
    for ffmpeg_path, corresponding_mingw64_path in zip(textfile1, textfile2):
        ffmpeg_path = ffmpeg_path.strip()[1:]
        f = open(ffmpeg_dir + ffmpeg_path, 'r')
        corresponding_mingw64_path = corresponding_mingw64_path.strip()[1:]
        if not Path(target_dir + corresponding_mingw64_path).exists():
            # if the directory is not present 
            # then create it
            Path(target_dir + corresponding_mingw64_path).mkdir(parents=True, exist_ok=True)
        shutil.copy(ffmpeg_dir + ffmpeg_path, target_dir + corresponding_mingw64_path)
