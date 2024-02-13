# OpenFanHub

This project aims to provide plug and play solution for controlling fans and reading temperatures

## TODO
 * Finish PCB
   * Currently working on a barebone blue pill
   * Probably remove the option for two different µC options

## Features
 * Connection for 18 fans
    * 3 Fans form one group
    * 6 independently controllable
    * only one fan in each group reports rpm
    * 3 pin fans are supported but not regulated (I tried it, but ended but beeing to complicated since I plan on using 4 pin anyways)
 * 4 thermistor inputs
 * compatible with the corsair commander pro driver of the kernel
 * Compatible with the "blue pill" boards or an stm32f103c8tx in an LQFP48 package
    * The goal is to make is easy to solder and LQFP48 can be hard

