# stm8-tiny
Sample project using [`tiny`](https://github.com/ryanplusplus/tiny) with STM8.

## Hardware
### Blue (STM8S103F3P6 Development Board)
https://tenbaht.github.io/sduino/hardware/stm8blue/

### Black (STM8S105K4T6 Development Board)
https://github.com/TG9541/stm8ef/wiki/STM8S-Value-Line-Gadgets#stm8s105k4t6-breakout-board

### Discovery (STM8-DISCOVERY STM8S105C6T6 Development Board)
https://www.st.com/en/evaluation-tools/stm8s-discovery.html

Note: don't use the built-in ST-Link because `openocd` doesn't like it.

### ST-Link v2 Programmer/Debugger
https://www.adafruit.com/product/2548

## Setup
- Install [`sdcc`](http://sdcc.sourceforge.net/)
- Install [`openocd`](https://github.com/ntfreak/openocd) (may need to build from source to get STM8 support)
- Install [`stm8flash`](https://github.com/vdudouyt/stm8flash)
- Install [`stm8-gdb`](https://stm8-binutils-gdb.sourceforge.io/)

## Use
### Compile
```shell
make -f <target>.mk
```

### Clean
```shell
make <target>.mk clean
```

### Erase via SWIM
```shell
make <target>.mk erase
```

### Flash via SWIM
```shell
make <target>.mk upload
```

## Resources
- [STM8S/STM8AF Reference Manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/9a/1b/85/07/ca/eb/4f/dd/CD00190271.pdf/files/CD00190271.pdf/jcr:content/translations/en.CD00190271.pdf)
- [STM8S103F3 Datasheet](https://www.st.com/resource/en/datasheet/stm8s103f3.pdf)
- [STM8S105K4 Datasheet](https://www.st.com/resource/en/datasheet/stm8s105k4.pdf)
- [STM8S105C6 Datasheet](https://www.st.com/resource/en/datasheet/stm8s105c6.pdf)
- lujji's Bare metal programming: STM8 [part 1](https://lujji.github.io/blog/bare-metal-programming-stm8/), [part 2](https://lujji.github.io/blog/bare-metal-programming-stm8-part2/)
