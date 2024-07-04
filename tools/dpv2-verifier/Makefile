
all: sock_addr_testmod.ko connect_prog.o print_addr

sock_addr_testmod.ko:
	$(MAKE) -C src/sock_addr_testmod host

connect_prog.o:
	
	clang -target bpf -Wall -O2 -I/usr/include/x86_64-linux-gnu/ -c src/connect4_prog.c -o connect4_prog.o

print_addr:
	gcc src/print_addr.c -o print_addr

clean:
	$(MAKE) -C src/sock_addr_testmod clean
	rm -f ./print_addr
	rm -f ./connect4_prog.o

