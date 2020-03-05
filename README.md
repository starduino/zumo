# starduino/zumo
Zumo bot developed using a Starduino.

## Target Hardware
https://www.pololu.com/product/2508

## Programmer
### ST-Link v2 Programmer/Debugger
- https://www.adafruit.com/product/2548

## Setup
- Install [`sdcc`](http://sdcc.sourceforge.net/)
- Install [`openocd`](https://github.com/ntfreak/openocd) (may need to build from source to get STM8 support)
- Install [`stm8flash`](https://github.com/ryanplusplus/stm8flash)
- Install [`stm8-gdb`](https://stm8-binutils-gdb.sourceforge.io/)
- Copy `./udev/70-st-link.rules` to `/etc/udev/` and run `udevadm control --reload-rules`

## Use
### Compile
```shell
make -f target.mk
```

### Clean
```shell
make -f target.mk clean
```

### Erase via SWIM
```shell
make -f target.mk erase
```

### Flash via SWIM
```shell
make -f target.mk upload
```

## Test
```shell
make -f test.mk
```

## Pinout
| STM8 Pin | Zumo Pin | Peripheral | Function               |
| -------- | -------- | ---------- | ---------------------- |
| PA3      | N/A      | GPIO       | Heartbeat LED          |
| PE1      | SCL      | I2C SCL    | I2C SCL                |
| PE2      | SDA      | I2C SDA    | I2C SDA                |
| PC2      | 13       | GPIO       | Zumo Heartbeat LED     |
| PD0      | 8        | GPIO       | Left motor direction   |
| PE3      | 7        | GPIO       | Right motor direction  |
| PD3      | 10       | TIM2_CH2   | Left motor power       |
| PD4      | 9        | TIM2_CH1   | Right motor power      |
| PC3      | 4        | TIM1_CH1   | Left line sensor       |
| PC4      | 5        | TIM1_CH2   | Right line sensor      |
| PD2      | 6        | TIM3_CH1   | Buzzer                 |
| PB3      | ?        | AN2        | Left distance sensor   |
| N/A      | N/A      | AN4        | Center distance sensor |
| PB2      | ?        | AN3        | Right distance sensor  |
| PC7      | 12       | GPIO       | Configuration pin      |

## Resources
- [STM8S/STM8AF Reference Manual](https://www.st.com/resource/en/reference_manual/cd00190271.pdf)
- [STM8S207xx Datasheet](https://www.st.com/resource/en/datasheet/stm8s207rb.pdf)
