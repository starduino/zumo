# stm8-tiny
Sample project using [`tiny`](https://github.com/ryanplusplus/tiny) with STM8S103F3.

## Hardware
### Blue (STM8S103F3P6 Development Board)
https://tenbaht.github.io/sduino/hardware/stm8blue/

### Discovery (STM8-DISCOVERY STM8S105C6T6 Development Board)
https://www.st.com/en/evaluation-tools/stm8s-discovery.html

### Disco-S001 (STM8-SO8-DISCO STM8S001J3M3 Development Board)
https://www.st.com/en/evaluation-tools/stm8-so8-disco.html

### STLink v2 Programmer/Debugger
https://www.adafruit.com/product/2548

## Setup
- Install [`sdcc`](http://sdcc.sourceforge.net/)
- Install [`openocd`](https://github.com/ntfreak/openocd) (may need to build from source to get STM8 support)
- Install [`stm8flash`](https://github.com/vdudouyt/stm8flash)
- Install [`stm8-gdb`](https://stm8-binutils-gdb.sourceforge.io/)

## Use
### Compile
```shell
make
```

### Clean
```shell
make clean
```

### Erase via SWIM
```shell
make erase
```

### Flash via SWIM
```shell
make upload
```

## Resources
- lujji's Bare metal programming: STM8 [part 1](https://lujji.github.io/blog/bare-metal-programming-stm8/), [part 2](https://lujji.github.io/blog/bare-metal-programming-stm8-part2/)
- [I2C ISR example](https://blog.mark-stevens.co.uk/2015/05/stm8s-i2c-master-devices/)
- [Datasheet](https://www.st.com/content/ccc/resource/technical/document/datasheet/42/5a/27/87/ac/5a/44/88/DM00024550.pdf/files/DM00024550.pdf/jcr:content/translations/en.DM00024550.pdf)
- [Reference manual](https://www.st.com/content/ccc/resource/technical/document/reference_manual/9a/1b/85/07/ca/eb/4f/dd/CD00190271.pdf/files/CD00190271.pdf/jcr:content/translations/en.CD00190271.pdf)
