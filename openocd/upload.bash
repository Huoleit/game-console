#!/usr/bin/bash
openocd -s openocd \
					-f stlink.cfg \
					-c "transport select hla_swd" \
					-f stm32f1x.cfg \
					-c "program vscode-cmake-build/PongGame.elf verify reset exit"                   