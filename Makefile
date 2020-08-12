SQUAD7DIR = $(shell pwd)/squad7++
PROJECTXDIR = $(shell pwd)/projectx

main:
	cd $(SQUAD7DIR) && make
	cd $(PROJECTXDIR) && make
