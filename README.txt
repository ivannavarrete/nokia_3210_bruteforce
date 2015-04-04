This program is used to brute force the 'security code' on a nokia 3210
phone. This is done by stepping through each key, generating the corresponding
pulses on the appropriate ports.

The security code on the 3210 is 5 digits. The total number of keys is then:

      10 + 10^2 + 10^3 + 10^4 + 10^5 = 111110

There is nothing sacred with the 3210. It could be replaced by any phone,
or pretty much anything else for that matter. All this program does is
generates pulses on ports. Got nothin to do with phones whatsoever.

Implementation notes:
Since this program is transferred to the MCU at every restart, it uses only
the 256 bytes of RAM that the bootstrap code downloads. Note that we need some
space for the stack at the top of the RAM. Thus, the code plus stack must not
exceed 256 bytes.
