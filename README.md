# PTDriverFirmware
C++ code for the PTDriver board

This is designed to read a PT100 RTD and drive a DAC to output the voltage that would be presented to an SKR controller board.
Out is driven by an op-amp, with the current contributed by the SKR's 4.7K in the feedback loop.  This lets me use the accurate
and high temp PT100 saensor without having to hack into the somewhat broken support for PT100 sensors in Marlin firmware.

The board supports two PT100.  I have in mind using this to control the hotends on an IDEX printer I will use to print Nylon and dissolvable support material.  But this code is for the old single channel version of the board.

TODO: port this firmware to the new two channel board ( [github](https://github.com/wallaceowen/PTDriverFirmware) )

