# DaymOS Roadmap

A living checklist for turning DaymOS from a bootable kernel into a more complete hobby OS.

## Current status

Implemented so far:
- GRUB / Multiboot2 boot flow
- 64-bit long mode entry
- VGA text console output
- VGA hardware cursor tracking
- IDT setup and basic exception handling
- Better exception diagnostics (vector, error code, RIP, CS, RFLAGS, CR2 for page faults)
- PIC remap
- PIT timer running in the background
- PS/2 keyboard IRQ handling
- Basic shell prompt and commands
- COM1 serial logging for boot diagnostics

Current shell commands:
- `help`
- `about`
- `version`
- `ticks`
- `clear`
- `reboot`
- `panic`

## Near-term priorities

### 1. Shell polish
- Improve command parsing structure
- Add `history` and simple command recall
- Improve line editing (left/right arrows, home/end)
- Add a `time` or uptime-style command based on PIT ticks
- Add a `serial on/off` debug toggle if interactive serial output becomes useful again

### 2. Logging and debugging
- Split VGA output and serial logging more cleanly
- Add explicit kernel log helpers instead of mirroring all print output
- Add more structured panic / fault dumps
- Consider QEMU-friendly debug output helpers

### 3. Memory management groundwork
- Detect / capture memory map from bootloader
- Build a physical frame allocator
- Add a simple kernel heap / bump allocator
- Add page mapping helpers
- Strengthen page fault handling

### 4. Input and console improvements
- Better backspace behavior across wrapped lines
- Tabs / control character handling polish
- Optional split between log area and input area
- Keyboard modifiers beyond Shift

## Mid-term goals

### Scheduler / execution
- Timer-driven task switching groundwork
- Kernel threads
- Round-robin scheduler

### Syscalls / user mode
- Ring 3 transition groundwork
- Syscall entry path
- Basic process model

### Storage / filesystem
- Disk access in QEMU
- Simple ramdisk or toy filesystem
- Eventually FAT or ext2-style experiments

## Nice-to-have commands
- `meminfo`
- `uptime`
- `echo`
- `cls` alias for `clear`
- `lsmem` / paging debug helpers

## Notes
- Prefer small, testable bites.
- If a change affects IRQ behavior, verify timer and keyboard both still work before committing.
- Be careful about doing heavy console or serial work directly inside IRQ handlers.
