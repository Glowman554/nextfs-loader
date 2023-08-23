all:
	make -C stage0
	make -C stage1
	make -C stage1_mb

	cat stage0/main.bin stage1/main.bin > loader.bin
	cat stage0/main.bin stage1_mb/main.bin > loader_mb.bin

clean:
	rm *.bin
	make -C stage0 clean
	make -C stage1 clean
	make -C stage1_mb clean