### <u>This README guides how to include the FFmpeg library into the code and how to debug it with VS Code over "MSYS2 UCRT64"</u> 

Target OS: Windows 11, development environment: MSYS2 UCRT64 

1. Clone somewhere (let's call chosen location as PROJECT_PATH) `git clone https://github.com/ysrepo/sandbox.git` 

    a. In the result PROJECT_PATH\sandbox directory appears with subfolders and files from repository, e.g. C:\Users\User\sandbox 

1. Open folder PROJECT_PATH\sandbox in VS Code 

1. Delete FFmpeg folder in PROJECT_PATH\sandbox directory 

1. Run `git update-index --skip-worktree ./FFmpeg/here_files_from_ffmpeg_rep` 

   a. This will untrack from git changes the deleted stub file named `here_files_from_ffmpeg_rep`, which is present only with clarity purpose 

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

    a. The output should contain info about versions what assures that installation proceed successfully and command/binaries is/are accessible 

1. In VS Code install official C/C++ extentions packs, for me installing C/C++ extention automatically installed 3 more others (C/C++ DevTools, C/C++ Extension Pack, C/C++ Themes) 

1. Add `MSYS2_PATH\ucrt64\bin` to Windows %PATH% (e.g. `C:\msys64\ucrt64\bin` in my case) 

    a. Without it on %PATH% gcc.exe gives errors and doesn't compile project even when full path to gcc.exe is set in VS Code configs 

    b. One more note: even after adding `MSYS2_PATH\ucrt64\bin` to the %PATH% I still kept receiving errors during running the `main.c`, putting `MSYS2_PATH\ucrt64\bin` at the top of the %PATH% helped me 

1. Close all open integrated terminals and re-open VS Code 

    a. Note: sometimes on different machines software behaves differently, maybe there is need to run the VS Code with Admin Rights 

1. Edit (replace) `PROJECT_PATH\.vscode\launch.json`, refer to `PROJECT_PATH\000-ucrt64-setup\vscode-configs\launch.json` 

    a. Pay attention to the `"miDebuggerPath"`'s value, in case your MSYS2_PATH differs, then set the correct one for you 

1. Edit (replace) `PROJECT_PATH\.vscode\task.json`, refer to `PROJECT_PATH\000-ucrt64-setup\vscode-configs\tasks.json` 

    a. Pay attention to the `"command"`'s value, in case your MSYS2_PATH differs, then set the correct one for you 

1. Open the `main.c` file (VS Code takes data for configs to execute the program from the currently active file opened in the editor) and run it by clicking in the right top area of the VS Code window on the appropriate button with the dropdown choosing the `Run C/C++ file` option. The output files will be located in the folder `sandbox/dispatch` 

   a. Optionally clean working sets after this step: delete the `dispatch` directory, delete the `main` file to be sure that you begin next running/debugging iterations from the clear state again 

1. To debug: set the breakpoint somewhere on the FFmpeg method in `main.c`, for example, at line 51, i.e., at `main.c:51` 

   a. Start debugging the `main.c` by clicking in the right top area of the VS Code window on the appropriate button with the dropdown choosing the `Debug C/C++ file` option 

   b. The execution should pause in the debug mode at the breakpoint, you can step into the code of `avformat_alloc_context()` 

1. Enjoy debugging! =) 

