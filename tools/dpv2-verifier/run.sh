#!/bin/sh

set -eu

CGROUP=sock_addr_verifier
PROG_NAME=sock_addr_verifier

setup()
{
	mkdir /sys/fs/cgroup/$CGROUP
	echo $$ > /sys/fs/cgroup/$CGROUP/cgroup.procs
	bpftool prog load connect4_prog.o /sys/fs/bpf/$PROG_NAME
	bpftool cgroup attach /sys/fs/cgroup/$CGROUP cgroup_inet4_connect pinned /sys/fs/bpf/$PROG_NAME
	insmod src/sock_addr_testmod/sock_addr_testmod.ko
}

cleanup()
{
	bpftool cgroup detach /sys/fs/cgroup/$CGROUP cgroup_inet4_connect pinned /sys/fs/bpf/$PROG_NAME || true
	echo $$ > /sys/fs/cgroup/cgroup.procs || true
	rmdir /sys/fs/cgroup/$CGROUP || true
	rm /sys/fs/bpf/$PROG_NAME || true
	rmmod sock_addr_testmod || true
}

green='\033[0;32m'
red='\033[0;31m'
clear='\033[0m'
expected_addr="192.168.0.2:80"

trap cleanup EXIT 2 3 6 15
setup
addr=$(./print_addr)

if [ "$addr" = "$expected_addr" ]; then
	echo "${green}PASS${clear}"
else
	echo "${red}FAIL${clear}: $addr != $expected_addr"
fi
