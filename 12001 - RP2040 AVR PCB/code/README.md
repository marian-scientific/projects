For RP2040 programs:

```mkdir build
cd build
cmake ..
make```

Then reset the board with boot pulled low. It will appear as USB device. Copy .uf2 file from build dir onto board. It will reset and run.