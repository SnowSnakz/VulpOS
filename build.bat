@echo off

:configuration
    rem Path to VirtualBox install location
    set VBoxPath=C:\Program Files\Oracle\VirtualBox
    set VBoxMachine=VulpOS 

:build-script
    rem Build the kernel.
    wsl ./build.sh

    rem Store the old value of PATH so that it can be reverted when done.
    set OLD_PATH=%PATH%

    rem Ensure that VirtualBox is available on PATH.
    set PATH=%PATH%;%VBoxPath%

    rem Move to the bin directory.
    cd bin

    rem Detach disk.vdi from the virtual machine. 
    rem This is required because the disk Guid will change after the new disk is built.
    VBoxManage storageattach %VBoxMachine% --storagectl "AHCI" --port 0 --medium none

    rem Delete the disk.vdi file if it exists, otherwise the VBoxManage command below will fail.
    del disk.vdi

    rem Conver the raw disk image to a VDI image.
    VBoxManage convertfromraw disk.raw disk.vdi --format=VDI

    rem Delete disk.raw file, it's too large and unnecessary to keep.
    del disk.raw
    
    rem Re-attach disk.vdi to the virtual machine.
    VBoxManage storageattach %VBoxMachine% --storagectl "AHCI" --port 0 --type=hdd --medium disk.vdi

    rem Return to the parent directory.
    cd ../

    rem Restore the previous value of PATH.
    set PATH=%OLD_PATH%

    rem Clean up variables
    set OLD_PATH=
    set VBoxPath=
    set VBoxMachine=

@echo on
