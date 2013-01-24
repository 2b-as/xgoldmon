PROJECT = xgoldmon

CFLAGS  = -Wall -g $(shell pkg-config --cflags libosmocore)
LDFLAGS = $(shell pkg-config --libs libosmocore)

$(PROJECT): util.o readlog.o logparse.o 

.PHONY: clean
clean:
	rm -rf *.o $(PROJECT) $(PROJECT).dSYM
