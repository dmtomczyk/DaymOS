#!/usr/bin/env bash

set -euo pipefail
REPO_ROOT=$(pwd | sed 's|/c/|C:/|' | sed 's|/|\\|g')

function run_in_new_window() {
    local title="$1"
    local cmd="$2"

    # This works well under Cmder/ConEmu
    start "" "C:\Program Files\Git\git-bash.exe" --cd="$REPO_ROOT" -c "$cmd; read -p 'Press enter to close...'"
}

function main_menu() {
    clear
    echo "=== Boot Manager ==="
    echo "[1] Boot QEMU (chainloader: os.img)"
    echo "[2] Boot QEMU (GRUB ISO)"
    echo "[3] Launch Bochs (chainloader)"
    echo "[4] Exit"
    echo

    read -p "Select an option: " choice

    case $choice in
        1)
            echo "Launching QEMU with chainloader (os.img)..."
            run_in_new_window "QEMU Chainboot" " qemu-system-x86_64 -drive format=raw,file=dist/x86_64/os.img -d int,cpu_reset -s -S -no-reboot -machine smm=off -debugcon stdio"
            ;;
        2)
            echo "Launching QEMU with GRUB (kernel.iso)..."
            run_in_new_window "QEMU GRUB" " qemu-system-x86_64 -boot d -cdrom dist/x86_64/kernel.iso -m 512M -no-reboot -no-shutdown -d int,cpu_reset -s"
            ;;
        3)
            echo "Launching Bochs..."
            run_in_new_window "Bochs Debugger" "bochs.exe -debugger -f bochsrc.bxrc"
            ;;
        4)
            echo "Exiting Boot Manager."
            exit 0
            ;;
        *)
            echo "Invalid choice. Try again."
            ;;
    esac
}

# Loop forever until user exits
while true; do
    main_menu
    sleep 1
done
