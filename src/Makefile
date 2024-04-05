
#
#	Makefile options
#

MAKEFLAGS += -j16

#
#	Compilation and linking options
#

CXX							= g++
CXXFLAGS					= -Wall -Werror -Wextra -std=c++17 -pedantic -g
LDFLAGS						= $(shell pkg-config --cflags --libs gtest) -lgtest_main -lgmpxx -lgmp
GCFLAGS						= -fprofile-arcs -ftest-coverage -fPIC
VGFLAGS						= --log-file="valgrind.txt" --track-origins=yes --trace-children=yes --leak-check=full --leak-resolution=med

#
#	Extensions
#

CPP							= .cc
TPP							= .tpp
HEADERS						= .h
OBJ							= .o

#
#	Connecting source file directories
#

VPATH						:= $(shell find . -type d)
vpath %$(CPP) $(VPATH)
vpath %$(HEADERS) $(VPATH)

#
#	Directories of source and object files
#

OBJ_DIR						= ../obj/
SRC_ENIGMA_DIR				= enigma/
SRC_HUFFMAN_DIR				= huffman/
SRC_RSA_DIR					= rsa/
SRC_DES_DIR					= des/
SRC_TESTS_DIR				= tests/
OBJ_ENIGMA_DIR				= $(OBJ_DIR)$(SRC_ENIGMA_DIR)
OBJ_HUFFMAN_DIR				= $(OBJ_DIR)$(SRC_HUFFMAN_DIR)
OBJ_RSA_DIR					= $(OBJ_DIR)$(SRC_RSA_DIR)
OBJ_DES_DIR					= $(OBJ_DIR)$(SRC_DES_DIR)
OBJ_TESTS_DIR				= $(OBJ_DIR)$(SRC_TESTS_DIR)

#
#	Creating folders for object files
#

ALL_OBJ_DIRS				= $(OBJ_ENIGMA_DIR) $(OBJ_HUFFMAN_DIR) $(OBJ_RSA_DIR) $(OBJ_DES_DIR) $(OBJ_TESTS_DIR)
FOLDERS						:= $(foreach dir, $(ALL_OBJ_DIRS), $(shell mkdir -p $(dir)))

#
#	Parsing of source files from directories and subdirectories
#

SRC_ENIGMA					:= $(foreach dir, $(shell find $(SRC_ENIGMA_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_HUFFMAN					:= $(foreach dir, $(shell find $(SRC_HUFFMAN_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_RSA						:= $(foreach dir, $(shell find $(SRC_RSA_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_DES						:= $(foreach dir, $(shell find $(SRC_DES_DIR) -type d), $(wildcard $(dir)/*$(CPP)))
SRC_TESTS					:= $(foreach dir, $(shell find $(SRC_TESTS_DIR) -type d), $(wildcard $(dir)/*$(CPP)))

#
#	Creating object files
#

OBJ_ENIGMA					:= $(patsubst %$(CPP), $(OBJ_ENIGMA_DIR)%$(OBJ), $(notdir $(SRC_ENIGMA)))
OBJ_HUFFMAN					:= $(patsubst %$(CPP), $(OBJ_HUFFMAN_DIR)%$(OBJ), $(notdir $(SRC_HUFFMAN)))
OBJ_RSA						:= $(patsubst %$(CPP), $(OBJ_RSA_DIR)%$(OBJ), $(notdir $(SRC_RSA)))
OBJ_DES						:= $(patsubst %$(CPP), $(OBJ_DES_DIR)%$(OBJ), $(notdir $(SRC_DES)))
OBJ_TESTS					:= $(patsubst %$(CPP), $(OBJ_TESTS_DIR)%$(OBJ), $(notdir $(SRC_TESTS)))
OBJ_ENIGMA_WO_CLI			:= $(filter-out $(OBJ_DIR)enigma/enigma_cli.o, $(OBJ_ENIGMA))
OBJ_HUFFMAN_WO_CLI			:= $(filter-out $(OBJ_DIR)huffman/huffman_cli.o, $(OBJ_HUFFMAN))
OBJ_RSA_WO_CLI				:= $(filter-out $(OBJ_DIR)rsa/rsa_cli.o, $(OBJ_RSA))
OBJ_DES_WO_CLI				:= $(filter-out $(OBJ_DIR)des/des_cli.o, $(OBJ_DES))

#
#	TARGETS
#

all: test

install:
	sudo apt-get install libgmp-dev

enigma: $(OBJ_ENIGMA)
	$(CXX) $(CXXFLAGS) $^ -o enigma_app
	./enigma_app

huffman: $(OBJ_HUFFMAN)
	$(CXX) $(CXXFLAGS) $^ -o huffman_app
	./huffman_app

rsa: $(OBJ_RSA)
	$(CXX) $(CXXFLAGS) $^ -o rsa_app -lgmpxx -lgmp
	./rsa_app

des: $(OBJ_DES)
	$(CXX) $(CXXFLAGS) $^ -o des_app
	./des_app

test: $(OBJ_ENIGMA_WO_CLI) $(OBJ_HUFFMAN_WO_CLI) $(OBJ_RSA_WO_CLI) $(OBJ_DES_WO_CLI) $(OBJ_TESTS)
	$(CXX) $(CXXFLAGS) -O2 $^ -o test $(LDFLAGS)
	./test

valgrind: test
	valgrind $(VGFLAGS) ./test
	! grep -n "ERROR SUMMARY" valgrind.txt | grep -v "0 errors"

clean:
	rm -rf $(OBJ_DIR)
	rm -rf enigma_app
	rm -rf huffman_app
	rm -rf rsa_app
	rm -rf des_app
	rm -rf test
	rm -rf valgrind.txt
	rm -rf report
	rm -rf *.info
	rm -rf *.gc*

$(OBJ_ENIGMA_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_HUFFMAN_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_RSA_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_DES_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJ_TESTS_DIR)%$(OBJ): %$(CPP)
	$(CXX) $(CXXFLAGS) -c $< -o $@

format_set:
	find . -iname "*$(CPP)" -o -iname "*$(HEADERS)" -o -iname "*$(TPP)" | xargs clang-format --style=google -i --verbose

format_check:
	find . -iname "*$(CPP)" -o -iname "*$(HEADERS)" -o -iname "*$(TPP)" | xargs clang-format --style=google -n --verbose

.PHONY: all test clean valgrind format_set format_check enigma huffman rsa des