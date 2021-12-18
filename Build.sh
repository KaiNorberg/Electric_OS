#!/bin/sh
cd gnu-efi
make bootloader
cd ..
make os
make buildimg