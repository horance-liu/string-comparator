# StringComparator Reference

## Install CUT

#### Requirement

- CMake 2.8(>=)
- CLang or GCC 4.8(>=)

#### Online Install

```bash
curl -fsSL https://raw.github.com/ccock/cut/master/install.sh | sh
```

#### Manual Install

```bash
$ git clone https://github.com/horance-liu/cut.git
$ cd cut
$ make out && cd out
$ cmake .. 
$ make 
$ sudo make install
```

## Run Tests

```bash
$ cd string-comparator/cpp
$ mkdir build
$ cd build 
$ cmake .. 
$ make 
$ ./string-comparator-test
```
