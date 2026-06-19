import os
import shutil

ffmpeg_str = "C:\\Users\\User\\Documents\\sandbox\\FFmpeg\\"
current_loc = "C:\\Users\\User\\Documents\\cp_build\\MSYS2\\MinGW64\\"

with open("ffmpeg locations.txt") as textfile1, open("mingw64 locations.txt") as textfile2: 
    for x, y in zip(textfile1, textfile2):
        x = x.strip()[2:].replace('/', '\\')
        f = open(ffmpeg_str+x, 'r')
        y = y.strip()[2:].replace('/', '\\')
        if not os.path.exists(current_loc+y):
            # if the directory is not present 
            # then create it.
            os.makedirs(current_loc+y)
        shutil.copy(ffmpeg_str+x, current_loc+y)
