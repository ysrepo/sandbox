### <u>This project guides how to include the FFmpeg library into the code and how to debug it with VS Code upon "WSL: Ubuntu"</u> 

1) Install "WSL: Ubuntu" running in cmd `wsl --install` 

   a) WSL goes along with Windows 11, therefore you need only install the appropriate Ubuntu distribution to it, Ubuntu is default Linux distribution on WSL, i.e. no need to explicitly specify that target is Ubuntu 

1) Open VS Code and choose the option in the menu on the Start page `Connect to...` (Crtl + Alt + O) and choose the item `Connect to WSL` in the showed pop-up dropdown 

   a) In the result you will see in the left bottom corner of VS Code the clickable sign "WSL: Ubuntu" 

1) Install C/C++ extensions for "WSL: Ubuntu" (`Install in WSL: Ubuntu`): 

   a) For this choose the last item in the left vertical menu (in the sidebar) in VS Code and type "C/C++" in the "Search Extensions in Marketplace" field 

   b) Choose "C/C++ Extension Pack" and click the appropriate "Install in WSL: Ubuntu" blue button 

   c) This will install 4 needed extensions at once 

1) Open terminal (Ctrl + \`) 

   a) It is possible to open exactly WSL terminal integrated in VS Code: 

      - Open `settings.json` (<Ctrl + ,> and click the small Open Settings (JSON) icon (it looks like a file with a small turn-arrow) located in the top-right corner of the window) 

      - Add the next snippet to `settings.json` to add the WSL terminal to VS Code for convenience: 
         ```
         "terminal.integrated.profiles.linux": {
            "bash": {
                  "path": "bash",
                  "icon": "terminal-bash"
            },
            "WSL": {
                  "path": "/mnt/c/windows/System32/wsl.exe",
                  "args": []
            }
         }
         ``` 

1) Install C/C++ compiler and needed tools on "WSL: Ubuntu" by running next commands in terminal: 

   a) Run `sudo apt update`, `sudo apt install build-essential gdb` 

   b) Verify installation `gcc --version` (I've checked with gcc 15.2.0 it worked fully compatibable) 

1) Check if your current directory in the WSL terminal is `/home/user/` (it is usually displayed as `~`) and run `git clone https://github.com/ysrepo/sandbox.git` 

1) Open in VS Code cloned folder (`File > Open Folder > /home/user/sandbox/`) 

1) Remove Windows PATH inheritance in "WSL: Ubuntu" to avoid duplicate tools (name collisions) from Windows or other enviroments: 

   a) For example, if Windows PATH has MinGW64 included and there is gcc tool, the "WSL: Ubuntu" may take gcc from there instead of calling its own. To avoid such confusion, this case gets fully excluded by this step 

   b) Open your "WSL: Ubuntu" terminal 

   c) Open `/etc/wsl.conf` (`sudo nano /etc/wsl.conf` if permission for writing wasn't granted) 

   d) Add the following lines to the file: 
      ```
      [interop]
      appendWindowsPath = false
      ``` 

   e) Press Ctrl + O then Enter to save, and Ctrl + X to exit Nano 

   f) Completely close (kill) your terminal, VS Code and open Command Prompt or PowerShell in Windows, and shut down WSL to apply the changes: `wsl --shutdown` 

   g) Reopen your Ubuntu (VS Code under WSL: Ubuntu as in step 2.) session and verify the path is no longer inherited: `echo $PATH` 

   h) Verify if the displayed $PATH contains `/usr/local/lib`, otherwise manually add `/usr/local/lib` to the $PATH. It is a necessary requirement to start debugging. If `/usr/local/lib` isn't present in the current $PATH then: 

      - run `nano ~/.bashrc` or (`sudo nano ~/.bashrc` if permission for writing wasn't granted) 

      - scroll to the very bottom of the file 

      - append the following line `export PATH="$PATH:/path/to/your/directory"` 

      - press Ctrl + O then Enter to save, and Ctrl + X to exit the editor 

      - apply the changes immediately to your current terminal session by running `source ~/.bashrc` 

1) Run `sudo apt install nasm`. This is an additional necessary dependency to build FFmpeg from sources 

1) Delete the FFmpeg directory (located on `sandbox/FFmpeg` containing a stub file named "here_files_from_ffmpeg_rep") which is cloned from the sandbox repository 

1) Then run `git clone https://github.com/FFmpeg/FFmpeg.git` (this will newly create the FFmpeg folder at the place of the old one you just deleted and will clone sources from the origin repository) 

1) In the terminal navigate into the cloned FFmpeg directory with sources and run `git checkout n8.1` (this tag version is checked and works fully compatible with code and is able for debugging) 

1) Here in the `FFmpeg/` directory from the terminal further run `./configure --enable-shared --enable-static --enable-pic --enable-debug=3 --disable-optimizations --disable-stripping --extra-ldflags="-L/usr/local/lib -Wl,-rpath,/usr/local/lib" --extra-cflags="-Og -fno-omit-frame-pointer -fno-inline"` 

1) Run `make` and wait for compilation with build 

   a) For speed-up sometimes it is possible to run the `make -j8` command instead which executes building in parallel threads 

1) Run `make install` 

   a) If it displays a message that says "install: Permission denied" try again with sudo `sudo make install` 

   b) This will install needed binary FFmpeg files to "WSL: Ubuntu"'s PATH located by `/usr/local/bin`, `/usr/local/lib`, `/usr/local/include` 

1) Set the breakpoint somewhere on the FFmpeg method in `test_task/main.c` and step into the code 