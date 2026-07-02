1. Install WSL: Ubuntu 
2. Open VS Code and connect to WSL: Ubuntu (Connect to remote development workspaces Crtl+Alt+O)
3. Install C/C++ extensions for WSL: Ubuntu (Install in WSL: Ubuntu)
4. Prepare utils, tools, required pre-installed apts, software for compilation on WSL: Ubuntu
   ... List all needs from (4): gcc, gdb, ffmpeg, etc. ...
6. Prepare gdb (debug tool-command which runs debugging in VS Code under WSL: Ubuntu connection) to run with admin (sudo) rights. 
   For this open bash at `sandbox/` location and make the `gdb-sudo.sh` script executable by running next command in your terminal: `chmod +x gdb-sudo.sh`. 
   Because VS Code runs the debugger in the background, you cannot type your password when sudo prompts you. You must allow your user account to run GDB via sudo without a password. 
   Open the sudoers configuration file securely by running: `sudo visudo`. 
   Add the following line at the very bottom of the file (replace <your_username> with your actual Linux username): `your_username ALL=(ALL) NOPASSWD: /usr/bin/gdb`. 
   Save and close the editor (if using Nano, press Ctrl+O, Enter, then Ctrl+X).
10. Change .vscode configurations (launch.json, tasks.json) taking them from appropriate folder `_vscode_wsl_ubuntu`. Double check the property `"miDebuggerPath"` 
    in `.vscode/launch.json` - it should be `"miDebuggerPath": "${workspaceFolder}/gdb-sudo.sh"` what means that gdb will be executed with sudo when debug started
11. Next steps are nearly the same as in `test_task/README.md` except of ffmpeg location (in WSL: Ubuntu FFmpeg is installed to `\usr\local\lib`, `\usr\local\include`)
<!-- TODO: finish till the end this README according to steps to run debugging in VS Code connected to WSL: Ubuntu -->
