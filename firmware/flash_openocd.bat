openocd-0.8.0.exe -f interface/stlink-v2.cfg -f board/stm32f4discovery.cfg -c init -c targets -c "halt" -c "flash write_image erase build/rusefi.elf" -c "verify_image firmware.elf" -c "reset run" -c shutdown
