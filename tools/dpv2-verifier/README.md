## Prerequisites

### Install Build Tools

#### Ubuntu
```bash
$ apt install build-essential clang
```

#### RHEL
```bash
$ yum groupinstall "Development Tools"
$ yum install clang
```

### Install libbpf Headers
```bash
$ git clone https://github.com/libbpf/libbpf.git
$ cd libbpf
$ make install_headers
```

## Build
```bash
$ make
```

## Run
```bash
$ sudo ./run.sh
```
