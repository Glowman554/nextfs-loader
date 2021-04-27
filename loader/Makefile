all:
	make -C stage0
	make -C stage1

	cat stage0/main.bin stage1/main.bin > loader.bin

clean:
	rm loader.bin
	make -C stage0 clean
	make -C stage1 clean