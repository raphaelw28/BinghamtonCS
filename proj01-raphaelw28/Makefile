test : warehouse order1.txt order2.txt order3.txt
	./warehouse order1.txt
	./warehouse order2.txt
	./warehouse order3.txt	
	
warehouse : packer.c
	gcc -Wall -g -o warehouse warehouse.c packer.c
	
warehouse_debug : packer.c
	gcc -Wall -DSHOWBENCH -g -o warehouse warehouse.c packer.c 
	
clean :
	-rm warehouse