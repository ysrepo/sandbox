### <u>This README guides how to include the FFmpeg library into the code and how to debug it with VS Code over "MSYS2 MinGW64"</u>

Target OS: Windows 11, target environment: MSYS2 MinGW64

1. Clone somewhere (let's call chosen location as PROJECT_PATH) `git clone https://github.com/ysrepo/sandbox.git`
    
    a. In the result you will have PROJECT_PATH\sandbox directory with subfolders and files from repository, e.g. C:\Users\User\sandbox

1. Open folder sandbox in VS Code 

1. Delete FFmpeg directory

1. Open git bash in sandbox/ directory (Ctrl+\`) and run `git clone https://github.com/FFmpeg/FFmpeg.git`

1. In git bash run `cd FFmpeg/`

1. In git bash run `git checkout n8.1`

1. Download and install `MSYS2` (it's like minimum Linux VM for Windows, https://www.msys2.org/, I worked with  `..x86_64`)

1. Install `MSYS2`, most common way is directly to `C:\` which is by default

1. In VS Code open settings (`File>Preferences>Settings`) and type in search `terminal.integrated.profile.windows`, then click `Edit in settings.json`

1. Add to settings, save and close them: 
	```
    "MSYS2 MinGW64": {
        "path": "C:\\msys64\\usr\\bin\\bash.exe",
        "label": "MSYS2",
        "args": ["--login", "-i"],
        "env": {
            "MSYSTEM": "MINGW64",
            "CHERE_INVOKING": "1",
            "MSYS2_PATH_TYPE": "inherit"
        }
    }
    ```

1. Close all open integrated terminals and re-open VS Code (preferly always with Admin Rights)

1. Open terminals (Ctrl + \`) and open again both `git bash` and `MSYS2 MinGW64` terminals and navigate to `FFmpeg\` directory

1. In `MSYS2 MinGW64` terminal install gcc: `pacman -S mingw-w64-x86_64-gcc`. 

	a. Note: this command installs the latest release from MSYS2 mirrors. I checked and `FFmpeg n8.1` fully worked compatible with `gcc version 15.2.0`. 
    
    b. To install version 15.2.0 of gcc download next binaries: `mingw-w64-x86_64-gcc-15.2.0-13-any.pkg.tar.zst`, `mingw-w64-x86_64-gcc-15.2.0-13-any.pkg.tar.zst.sig`, `mingw-w64-x86_64-gcc-libs-15.2.0-13-any.pkg.tar.zst`, `mingw-w64-x86_64-gcc-libs-15.2.0-13-any.pkg.tar.zst.sig` from this repository `sandbox/mingw64-setup/additional-setup-binaries`. 
    
    c. Place these 4 files to `PATH_MSYS2/usr/var/cache`, if there is no such folder `cache` then manually create it. 
    
    d. Open terminal MinGW64, run 'cd /usr/var/cache', and manually downgrade `gcc` to the fully compatible version by running `pacman -U ./mingw-w64-x86_64-gcc-15.2.0-13-any.pkg.tar.zst ./mingw-w64-x86_64-gcc-libs-15.2.0-13-any.pkg.tar.zst`

1. Check if installation proceed succefully: `gcc -v`. Latter should print info

1. In `MSYS2 MinGW64` terminal install additional dependency for the configure command: `pacman -S mingw-w64-x86_64-nasm`

1. Run `./configure --prefix=/mingw64 --enable-shared --enable-static --enable-debug=3 --disable-optimizations --disable-stripping --extra-cflags="-Og -fno-omit-frame-pointer -fno-inline"`

1. Then install make tool: `pacman -S make`

1. Run `make` command in MSYS2 MinGW64 terminal and wait for compilation with build (for speed-up sometimes it is possible to run `make -j8` command instead which executes building in parallel threads)

1. Run `make install`

    a. Now distributed FFmpeg binaries and files are ready for use and taking into account debug flags this version of files fully correspond and satisfy debug distrubition of FFmpeg library. This means this version of files allow to step into the code during debugging for example in VS Code. Next complicated thing is only that these files in FFmpeg folder are scattered in different directories and next step is to manually collect them all together from beneath `FFmpeg/` locations to the correct directory tree which should pass the MSYS2 MinGW64 stucture and further files should be placed to the respective locations under the `MSYS2/mingw64/`. Details are provided below

    b. Refer to table in the `mapping_table.txt` located in `copy_binaries/` to repack binaries according to the needs MinGW64 target tree. Or run `cp_files.py` it will copy and bundle the needed files in the correct directory structure by itself

    c. Place the prepared files to C:/MSYS2/MinGW64/ ... (this step actually equals to install FFmpeg from Pacman, but manually and which are with debug possibility featured)

1. Go to VS Code and check if FFmpeg is now accessible running the `ffmpeg` command in terminal, you should see versions info output

1. In VS Code install official C/C++ extentions packs, for me installing C/C++ extention automatically installed 3 more others (C/C++ DevTools, C/C++ Extension Pack, C/C++ Themes)

1. Click settings icon in the right bottom corner on "C/C++" extension, choose "Settings" option, scroll down a little to the `C_Cpp>Default: Compiler Path`, click "Edit in settings.json" and set the `"C_Cpp.default.compilerPath": "C:/msys64/mingw64/bin/gcc.exe"`

1. Edit `.vscode/launch.json`, refer to `mingw64-setup/vscode-configs/launch.json` from this repository

1. Edit `.vscode/task.json`, refer to `mingw64-setup/vscode-configs/tasks.json` from this repository

1. Install debuggings tools to MinGW64 `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`

1. In VS Code click "Run" tab, then "Add Configuration...". This should generate clear `launch.json` 

1. Set the breakpoint let's say on the 47th line in `main.c` located in `test_task/`, launch debug by clicking on run dropdown and choosing "Debug C/C++ File" and step into the code of `avformat_alloc_context()`

1. Enjoy debugging! =)

1. 

