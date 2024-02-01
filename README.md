# Electric_OS [![License](https://img.shields.io/badge/licence-MIT-green)](https://github.com/Kaj9296/Electric_OS/LICENCE) 
  
A simple operating system inspired by whetever will work.  

OBS! This operating system is no longer being worked on since i have started work on a new operating system that can be found [here](https://github.com/Kaj9296/Asym).

# About  
Boot Loader: GNU-EFI  
Arch: x86_64  
   
Main Goal:   
To create a simple and completely useless operating system, because i can.  
  
## Build instructions   

Use the ```make all``` command to compile the OS.  

You can also use the ```make run``` command or the "run.bat" file to run the OS in qemu.  
If your insane enough to try and run on it real hardware simply flash the .img file in the bin directory to a USB.

## Next steps 

Implement grandfather system for processes.
Implement startchild and startsibling commands.
Fix shutdown general protection fault.  
Write more comments.  
Create own stack in the kernel.  
