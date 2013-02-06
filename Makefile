PROJECT = xgoldmon

CFLAGS  = -Wall -g $(shell pkg-config --cflags libosmocore)
LDFLAGS = $(shell pkg-config --libs libosmocore)

$(PROJECT): util.o readlog.o logparse.o xgoldmon.o
	$(CC) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o $(PROJECT) $(PROJECT).dSYM
