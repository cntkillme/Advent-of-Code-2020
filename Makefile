.PHONY: all clean

export BIN := $(PWD)/bin
export INCLUDE := $(PWD)/include
export CXX := g++ --std=c++17 -O3 -Wall -Werror -Wpedantic -I$(INCLUDE)/

all: $(BIN)
	$(MAKE) -C src

clean:
	$(MAKE) -C src clean
	rm -rf $(BIN)

$(BIN):
	mkdir $(BIN)
