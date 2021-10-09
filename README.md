# TokyoOS
TokyoOS is an operating system implementation written from scratch. It has been designed to run on UEFI systems running on 64-bit Intel x86 processors. It follows the *bootboot* protocol for bootloading.

## Features
TokyoOS is a monotasking OS, complete with user space, system calls to perform privileged actions, a limited C library, and a shell for user interaction. The capabilities of the OS also include a virtual file system, loading ELF64 executable files into memory, and inter-process communication using shared memory.

The OS also has a few drivers built-in, namely drivers for:
- Graphics Mode Display
- 16550 UART (Universal Asynchronous Receiver/Transmitter) Serial Port
- ACPI (Advanced Configuration and Power Interface)
- PCI (Peripheral Component Interconnect)
- AHCI (Advanced Host Controller Interface) for SATA Disks
- PS/2 Keyboard
- PIT (Programmable Interval Timer)
- RTC (Real Time Clock)
- ext2 File System (Read-only)

## The Shell
![Shell](https://i.imgur.com/NrG8dOE.png)

The shell is the interface provided by the OS for the user to perform actions using the OS. Supported commands are:
- `cls` : Clears the screen.
- `help` : Displays information about a command.
- `run` : Runs an executable file.
- `shutdown` : Shuts down the computer.
- `time` : Displays the current time.

---

## Directories:
- `bin/` : Contains the compiled kernel, the UEFI binary, and the final disk image.
- `disk/` : This directory structure is used to create the disk image, as specified in `utils/mkbootimg.json`. More info in the **Configuration** section below.
- `lib/` : Compiled, relocatable object files.
- `log/` : Execution logs.
- `src/` : Source code for the kernel and the C library.
- `usr/` : User programs go here.
- `utils/` : Utilities for creating the disk image.

## Files:
- `build.sh` : Run to compile the OS all at once.
- `config` : General configuration of the system the OS will run on. More info in the **Configuration** section below.
- `Makefile` : Makefile used to compile the OS.
- `run.cmd` : Execute to run the OS (on Windows).

## Configuration
Two files can be edited to change the configuration of the OS:
1. `config`:
   - `screen` : The screen resolution
   - `kernel` : The location of the kernel in the `initrd` (initial ramdisk). **Do Not Change.**
   - `timezone` : The local time zone. Format is `[+-]hours:minutes`

2. `utils/mkbootimg.json`: The only parameters here that can be modified are the partitions excluding the `boot` partition. The type of all other user partitions must also be `ext2` if they need to be read using the OS. In the OS, the partition `boot` is given volume label `A:`, while the other partitions are given labels from `C:` onwards, up till `Z:`. This also limits the number of user partitions to 24. The `size`, `name`, and `directory` parameters of each user partition can be freely modified.

## Usage
### Requirements:
1. For running the OS:
   - `qemu-system-x86_64` : The virtual machine to run the OS on.
2. For modifying the OS / adding user programs:
   - A utility that runs Makefiles.
   - `gcc` : To compile and link C and C++ code.
   - `nasm` : To assemble x86 assembly code.

### Adding a user program:
1. Go to the following section of the Makefile:

![User Files](https://i.imgur.com/Kg1wuik.png)

2. Duplicate the 3 `USR0` lines, and in it, replace all occurences of `USR0_` with `USR1_`.
3. Add the required source file names to `USR1_SRC` and the executable file name to `USR1_ELF`.
4. Move to the following section of the file:

![Make Rules](https://i.imgur.com/Qxv5elc.png)

5. Duplicate the `user0` line and replace all `0`s with `1`s.
6. Move to the following section of the file:

![Link Rules](https://i.imgur.com/vmxYimW.png?1)

7. Duplicate the 3 lines for `linkUser0` lines and replace all `0`s with `1`s, and save the file.
8. Open `build.sh`.
9. Add the line `make user1` anywhere between the lines `make shell` and `make`, and save the file. 

The above steps can be followed for any number of user programs.

### Compiling the OS (only required if modified):
- Run the file `build.sh` after making any required modifications.

### Running the OS:
NOTE: Make sure to compile first.
- On Linux: Run the command `make run` on the terminal.
- On Windows: Execute the `run.cmd` file.
