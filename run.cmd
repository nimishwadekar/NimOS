qemu-system-x86_64 -machine q35 -cpu qemu64 -bios bin/OVMF.fd -m 128 -drive file=bin/NimOS.img,format=raw -rtc clock=host -serial file:log/serial.log
