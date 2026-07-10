### <u>This README guides how to include the FFmpeg library into the code and how to debug it with VS Code over "MSYS2 UCRT64"</u> 

Target OS: Windows 11, development environment: MSYS2 UCRT64 

1. Clone somewhere (let's call chosen location as PROJECT_PATH) `git clone https://github.com/ysrepo/sandbox.git` 

    a. In the result PROJECT_PATH\sandbox directory appears with subfolders and files from repository, e.g. C:\Users\User\sandbox 

1. Open folder PROJECT_PATH\sandbox in VS Code 

1. Delete FFmpeg folder in PROJECT_PATH\sandbox directory 

1. Open git bash in PROJECT_PATH\sandbox directory (Ctrl + \`) and run `git clone https://github.com/FFmpeg/FFmpeg.git` 

1. In git bash run `cd FFmpeg` 

1. In git bash run `git checkout n8.1` 

1. Download and install `MSYS2` (it's like minimum Linux VM for Windows, https://www.msys2.org/, I worked with  `..x86_64`) 

1. Install `MSYS2` 

    a. Usually it installs directly to `C:\` by default, let's call it (or another manually chosen MSYS2 installation directory) as MSYS2_PATH, e.g. C:\msys64 is MSYS2_PATH 

1. In VS Code open settings (`File>Preferences>Settings`) and type in search `terminal.integrated.profile.windows`, then click `Edit in settings.json` 

1. Add to settings: 
	```
    "MSYS2 UCRT64": {
        "path": "C:\\msys64\\usr\\bin\\bash.exe",
        "args": [
            "--login",
            "-i"
        ],
        "env": {
            "MSYSTEM": "UCRT64",
            "CHERE_INVOKING": "1",
            "MSYS2_PATH_TYPE": "inherit"
        }
    }
    ``` 

    a. When in your case path differs then replace "path" value to your MSYS2_PATH 

1. Close all open integrated terminals and re-open VS Code 

    a. Note sometimes on different machines software behaves differently, maybe there is need to run the VS Code with Admin Rights 

1. Open terminals (Ctrl + \`) and open `MSYS2 UCRT64` terminal in VS Code and navigate to `cd PROJECT_PATH\FFmpeg` 

1. In `MSYS2 UCRT64` terminal run `pacman -Syuu` to update packages 

1. In `MSYS2 UCRT64` terminal install compiling, building and developer tools: `pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain`. 

1. Check whether installation proceed succefully: `gcc --version`. Latter should print info 

1. In `MSYS2 UCRT64` terminal install additional dependency for the configure command: `pacman -S mingw-w64-ucrt-x86_64-nasm` 

1. Run `./configure --prefix=/ucrt64 --enable-shared --enable-static --enable-debug=3 --disable-optimizations --disable-stripping --extra-cflags="-Og -fno-omit-frame-pointer -fno-inline"` 

1. Run `make` and wait for compilation with build 

   a. For speed-up sometimes it is possible to run the `make -j8` command instead which executes building in parallel threads 

1. Run `make install` 

1. Run the `ffmpeg` command in terminal

    a. The output should contain info about versions what assures that installation procced successfully and command/binaries is/are accessible 

1. In VS Code install official C/C++ extentions packs, for me installing C/C++ extention automatically installed 3 more others (C/C++ DevTools, C/C++ Extension Pack, C/C++ Themes) 

1. Add `MSYS2_PATH\ucrt64\bin` to Windows %PATH% (e.g. `C:\msys64\ucrt64\bin` in my case) 

    a. Without it on %PATH% gcc.exe gives errors and doesn't compile project even when full path to gcc.exe is set in VS Code configs 

1. Edit (replace) `PROJECT_PATH\.vscode\launch.json`, refer to `PROJECT_PATH\000-ucrt64-setup\vscode-configs\launch.json` 

1. Edit (replace) `PROJECT_PATH\.vscode\task.json`, refer to `PROJECT_PATH\000-ucrt64-setup\vscode-configs\tasks.json` 

1. Set the breakpoint let's say on the 47th line in `main.c` located in `PROJECT_PATH\test_task\`, launch debug by clicking on run dropdown and choosing "Debug C/C++ File" and step into the code of `avformat_alloc_context()` 

1. Enjoy debugging! =) 

1. 

