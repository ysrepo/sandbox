1. Create somewhere folder sandbox

2. Open folder sandbox in VS Code 

3. Open git bash in current directory (ctrl+\` and choose corresponding option) and run `git clone https://github.com/FFmpeg/FFmpeg.git`

4. In git bash run `cd FFmpeg/`

5. In git bash run `git checkout n8.1`

6. Create one more folder (for example "test_task") under sandbox next to just created `FFmpeg/` directory

7. Download and install `MSYS2` (it's like minimum Linux VM for Windows, https://www.msys2.org/) choosing proper variant, most likely it is `..x86_64`

8. Install `MSYS2`, most common way is directly to `C:\` which is by default

9. Open settings (`File>Preferences>Settings`) and type in search `terminal.integrated.profile.windows`, then click `Edit in settings.json`

10. Add to settings, save and close them: 
	`
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
    `

11. Close all open integrated terminals and re-open VS Code (preferly always with Admin Rights)

12. Open terminals (ctrl+\`) and open again both `git bash` and `MSYS2 MinGW64` terminals and navigate to `FFmpeg\` directory

13. In `MSYS2 MinGW64` terminal install gcc: `pacman -S mingw-w64-x86_64-gcc`. 
	(Note: this installs the latest release from MSYS2 mirrors, I checked and `FFmpeg n8.1` fully worked compatible with `gcc version 15.2.0`, for this download `mingw-w64-x86_64-gcc-15.2.0-13-any.pkg.tar.zst`, `mingw-w64-x86_64-gcc-15.2.0-13-any.pkg.tar.zst.sig`, `mingw-w64-x86_64-gcc-libs-15.2.0-13-any.pkg.tar.zst`, `mingw-w64-x86_64-gcc-libs-15.2.0-13-any.pkg.tar.zst.sig`, place these 4 files to `PATH_MSYS2/usr/var/cache`, if there is no such folder `cache` then create, open terminal MinGW64, run 'cd /usr/var/cache', and manually downgrade `gcc` to the fully compatible version by running `pacman -U ./mingw-w64-x86_64-gcc-15.2.0-13-any.pkg.tar.zst ./mingw-w64-x86_64-gcc-libs-15.2.0-13-any.pkg.tar.zst`)

14. Check if installation proceed succefully: `gcc -v`. Latter should print info

15. In `MSYS2 MinGW64` terminal install additional dependency for configure FFmpeg: `pacman -S mingw-w64-x86_64-nasm`

16. Run `./configure --enable-shared --enable-static --enable-debug=3 --disable-optimizations --disable-stripping --extra-cflags="-Og -fno-omit-frame-pointer -fno-inline"`

17. Then install make tool: `pacman -S make`

18. Then type just `make` in MSYS2 MinGW64 terminal, run this command and wait for compilation with build (for speed-up sometimes it is possible to run 
`make -j8` command instead which executes building in parallel threads)

19. Now distributed FFmpeg binaries and files are ready for use and taking into account debug flags this version of files fully correspond and satisfy debug distrubition of FFmpeg library. This means this version of files allow to step into the code during debugging for example in VS Code. Next complicated thing is only that these files in FFmpeg folder are scattered in different directories and next step is to manually collect them all together from beneath `FFmpeg/` locations to the correct directory tree which should pass the MSYS2 MinGW64 stucture and further files should be placed to the respective locations under the `MSYS2/mingw64/`. Details are provided below

20. Refer to table in the `mapping_table.txt` located in `copy_binaries/` to repack binaries according to the needs MinGW64 target tree. Or run `cp_files.py` it will copy and bundle the needed files in the correct directory structure by itself

21. Place the prepared files to C:/MSYS2/MinGW64/ ... (this step actually equals to install FFmpeg from Pacman, but manually and with debug possibility featured)

22. Go to VS Code and check if FFmpeg is now accessible running the `ffmpeg` command in terminal, you should see versions info output

23. In VS Code install official C/C++ extentions packs, for me installing C/C++ extention automatically installed 3 more others (C/C++ DevTools, C/C++ Extension Pack, C/C++ Themes)

24. Click settings icon in the right bottom corner on "C/C++" extension, choose "Settings" option, scroll down a little to the `C_Cpp>Default: Compiler Path`, click "Edit in settings.json" and set the `"C_Cpp.default.compilerPath": "C:/msys64/mingw64/bin/gcc.exe"`

25. Now open your main.c in `test_task` folder, then click "Terminal" tab and click "Configure Default Build Task...". Choose in the dropdown "C/C++: gcc.exe build active file". This should generate `.vscode/` folder under the root directory "sandbox" with `tasks.json` file

26. Edit `task.json`, refer to `.vscode/task.json` from this repository

27. Install debuggings tools to MinGW64 `pacman -S --needed base-devel mingw-w64-x86_64-toolchain`

28. In VS Code click "Run" tab, then "Add Configuration...". This should generate clear `launch.json` 

29. Edit `launch.json`, refer to `.vscode/launch.json` from this repository

30. Set the breakpoint let's say on the 47th line in `main.c` located in `test_task/`, launch debug by clicking on run dropdown and choosing "Debug C/C++ File" and step into the code of `avformat_alloc_context()`

31. Enjoy debugging! =)

32. 

