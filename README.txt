Compile from windows, you will need:
* WSL
* VirtualBox (Installed to C:/Program Files/Oracle/VirtualBox)

Run build.bat from windows for a full build.

Running build.sh will only compile the kernel and create the disk.raw file.
The build.bat file invokes WSL to call build.sh, and then converts the disk.raw to a disk.vdi file for use with VirtualBox.
