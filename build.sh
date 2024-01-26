
# Move to the source directory
cd ./kernel/

# Compile the kernel
make

# Copy the kernel to the bin/boot directory
cp ./build/kernel.elf ../bin/boot/kern.bin

# Move to the bin directory
cd ../bin

# Run mkbootimg check to display bootboots analysis on the kernel.
../mkbootimg check ./boot/kern.bin

# Run mkbootimg to create the disk.raw file.
../mkbootimg ./mkbootimage.json ./disk.raw
