### <u>This project guides how to include FFmpeg library into the code and debug it in VS Code under "WSL: Ubuntu".</u> 

1) Install WSL: Ubuntu running `wsl --install` 

   a) WSL goes along with Windows 11, therefore you need only install appropriate Ubuntu distribution to it, Ubuntu is default Linux distribution on WSL, i.e. no need to explicitly specify that target is Ubuntu 

1) Open VS Code and choose the option in menu on the Start page `Connect to...` (Crtl + Alt + O) and choose the item `Connect to WSL` in the showed pop-up dropdown 

   a) In the result you will see in the left bottom corner of VS Code the clickable sign "WSL: Ubuntu" 

1) Install C/C++ extensions for "WSL: Ubuntu" (`Install in WSL: Ubuntu`) 

   a) For this choose the last item in the left vertical menu (in the side bar) in the VS Code and type "C/C++" to the "Search Extensions in Marketplace" field 

   b) Choose "C/C++ Extension Pack" and click the appropriate "Install in WSL: Ubuntu" blue button 

   c) This will install all 4 needed extensions at once 

1) Open WSL terminal (Ctrl + \`) 

   a) It is possible to open WSL terminal integrated in VS Code 

   b) Add to `settings.json` (Ctrl + ,) next snippet to add WSL terminal to VS Code for convenience: 
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

1) Install C/C++ compiler, needed tools on "WSL: Ubuntu" running next commands in terminal: 

   a) `sudo apt update`, `sudo apt install build-essential gdb`. I faced with some troubles of missing dependencies for gdb and had manually to add and to install rest of them 

   b) Verify installation `gcc --version` 

1) In terminal run `git clone https://github.com/ysrepo/sandbox.git` 

1) Open in VS Code cloned folder (`File > Open Folder > sandbox`) 

1) Remove windows PATH inheritance in "WSL: Ubuntu" to avoid duplicate tools (name collisions) from Windows or other enviroments: 

   a) For example, if windows PATH has MinGW64 included and there is gcc tool, the "WSL: Ubuntu" may take gcc from there instead of calling its own. To avoid such confusion, this case gets fully excluded by this step 

   b) Open your "WSL: Ubuntu" terminal 

   c) Open `/etc/wsl.conf` using your preferred text editor (e.g., nano) 

   d) Add the following lines to the file: 
      ```
      [interop]
      appendWindowsPath = false
      ``` 

   e) Press Ctrl + O then Enter to save, and Ctrl + X to exit Nano 

   f) Completely close your terminal, open Command Prompt or PowerShell in Windows, and shut down WSL to apply the changes: `wsl --shutdown` 

   g) Reopen your Ubuntu (VS Code under WSL: Ubuntu) session and verify the path is no longer inherited: `echo $PATH` 

1) Run `sudo apt install nasm`. This is additional necessary dependency to build FFmpeg from sources 

1) Navigate to sandbox directory and delete FFmpeg folder, then run `git clone https://github.com/FFmpeg/FFmpeg.git` (this will newly create FFmpeg folder with sources from origin repository) 

1) Navigate into the clonned FFmpeg directory with sources and run `git checkout n8.1` (this tag version is checked and works fully compatible with code and is able for debugging) 

1) In `FFmpeg/` directory from terminal run `./configure --enable-shared --enable-static --enable-pic --enable-debug=3 --disable-optimizations --disable-stripping --extra-ldflags="-L/usr/local/lib -Wl,-rpath,/usr/local/lib" --extra-cflags="-Og -fno-omit-frame-pointer -fno-inline"` 

1) Run `make` and wait for compilation with build 

   a) For speed-up sometimes it is possible to run `make -j8` command instead which executes building in parallel threads 

1) Run `make install` 

   a) This will install needed binary FFmpeg files to "WSL: Ubuntu"'s PATH located by `/usr/local/bin`, `/usr/local/lib`, `/usr/local/include` 

1) Set the breakpoint somewhere in main.c on FFmpeg method and run the code 