.POSIX:
.SUFFIXES:
CC = gcc
CXX = g++
# OpenMP requires gcc. On macOS, gcc seems to link to clang, resulting in a compile error
CFLAGS = -Wall -g -O3 -fopenmp 
CXXFLAGS = -Wall -g -O2 -std=c++11 -pthread -march=native
LDFLAGS =
LDLIBS = -lgmp -lm
SHARED_OBJ = convolution.o global-common.o table.o table_fixed.o walsh-transform.o

.PHONY: all
all: aes saturnin spongent identity parse-and-convolve 

parse-and-convolve: parse-and-convolve.o csvparser.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

aes: aes-hamming aes-box aes-differential-trail aes-linear-trail aes-five-active-sboxes aes-sbox aes-trail-core

aes-hamming: aes-tools/aes-hamming-weight.o aes-tools/aes-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS)

aes-box: aes-tools/aes-box-weight.o aes-tools/aes-common.o $(SHARED_OBJ) mds-common.o
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS)

aes-trail-core: aes-tools/aes-trail-core.o aes-tools/aes-common.o $(SHARED_OBJ) mds-common.o
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS)

aes-sbox: aes-tools/aes-sbox.o aes-tools/aes-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS)

aes-differential-trail: aes-tools/aes-differential-trail-weight.o aes-tools/aes-common.o $(SHARED_OBJ) mds-common.o
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS)

aes-linear-trail: aes-tools/aes-linear-trail-weight.o aes-tools/aes-common.o $(SHARED_OBJ) mds-common.o
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS)

aes-five-active-sboxes: aes-tools/aes-five-active-sboxes.o aes-tools/aes-common.o $(SHARED_OBJ) 
	$(CC) $(LDFLAGS) -o aes-tools/$@ $^ $(LDLIBS) -fopenmp

identity: identity-box

identity-box: identity-tools/identity-box-weight.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o identity-tools/$@ $^ $(LDLIBS)

spongent: spongent-hamming spongent-box spongent-sbox spongent-differential-trail spongent-linear-trail spongent-differential \
	  spongent-correlation spongent-trail-core spongent-differential-unrounded

spongent-hamming: spongent-tools/spongent-hamming-weight.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-box: spongent-tools/spongent-box-weight.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-sbox: spongent-tools/spongent-sbox.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-differential-trail: spongent-tools/spongent-differential-trail-weight.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-trail-core: spongent-tools/spongent-trail-core.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-linear-trail: spongent-tools/spongent-linear-trail-weight.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-differential: spongent-tools/spongent-differential-probability.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-differential-unrounded: spongent-tools/spongent-differential-probability-unrounded.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

spongent-correlation: spongent-tools/spongent-correlation.o spongent-tools/spongent-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o spongent-tools/$@ $^ $(LDLIBS)

saturnin: saturnin-hamming saturnin-box saturnin-sbox saturnin-differential-trail saturnin-differential-trail-three-rounds \
	  saturnin-linear-trail saturnin-differential saturnin-correlation saturnin-trail-core saturnin-linear-trail-three-rounds \
	  saturnin-differential-unrounded

saturnin-hamming: saturnin-tools/saturnin-hamming-weight.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-box: saturnin-tools/saturnin-box-weight.o saturnin-tools/saturnin-common.o $(SHARED_OBJ) mds-common.o
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-sbox: saturnin-tools/saturnin-sbox.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-differential-trail: saturnin-tools/saturnin-differential-trail-weight.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-differential-unrounded: saturnin-tools/saturnin-differential-probability-unrounded.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-trail-core: saturnin-tools/saturnin-trail-core.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-differential-trail-three-rounds: saturnin-tools/saturnin-differential-trail-weight-three-rounds.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS) -fopenmp

saturnin-linear-trail-three-rounds: saturnin-tools/saturnin-linear-trail-weight-three-rounds.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS) -fopenmp

saturnin-linear-trail: saturnin-tools/saturnin-linear-trail-weight.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-differential: saturnin-tools/saturnin-differential-probability.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

saturnin-correlation: saturnin-tools/saturnin-correlation.o saturnin-tools/saturnin-common.o $(SHARED_OBJ)
	$(CC) $(LDFLAGS) -o saturnin-tools/$@ $^ $(LDLIBS)

.PHONY: clean
clean:
	rm -rf bin *.o aes-tools/*.o aes-tools/aes-hamming aes-tools/aes-box aes-tools/aes-differential-trail aes-tools/aes-linear-trail aes-tools/aes-sbox \
	aes-tools/aes-five-active-sboxes aes-tools/aes-trail-core \
	identity-tools/*.o identity-tools/identity-box \
	spongent-tools/*.o spongent-tools/spongent-hamming spongent-tools/spongent-box spongent-tools/spongent-sbox spongent-tools/spongent-differential-trail \
	spongent-tools/spongent-linear-trail spongent-tools/spongent-differential spongent-tools/spongent-correlation spongent-tools/spongent-trail-core \
	spongent-tools/spongent-differential-unrounded \
	saturnin-tools/*.o saturnin-tools/saturnin-hamming saturnin-tools/saturnin-box saturnin-tools/saturnin-sbox saturnin-tools/saturnin-differential-trail \
	saturnin-tools/saturnin-differential-trail-three-rounds saturnin-tools/saturnin-linear-trail saturnin-tools/saturnin-differential \
	saturnin-tools/saturnin-correlation saturnin-tools/saturnin-trail-core saturnin-tools/saturnin-linear-trail-three-rounds \
	saturnin-tools/saturnin-differential-unrounded \
	parse-and-convolve
.PHONY: install
install: aes saturnin spongent
	mkdir -p bin
	cp aes-tools/aes-hamming aes-tools/aes-box aes-tools/aes-differential-trail aes-tools/aes-linear-trail aes-tools/aes-sbox \
	aes-tools/aes-five-active-sboxes aes-tools/aes-trail-core \
	identity-tools/identity-box \
	spongent-tools/spongent-hamming spongent-tools/spongent-box spongent-tools/spongent-sbox spongent-tools/spongent-differential-trail \
	spongent-tools/spongent-differential spongent-tools/spongent-linear-trail spongent-tools/spongent-correlation spongent-tools/spongent-trail-core \
	spongent-tools/spongent-differential-unrounded \
	saturnin-tools/saturnin-hamming saturnin-tools/saturnin-box saturnin-tools/saturnin-sbox saturnin-tools/saturnin-differential-trail \
	saturnin-tools/saturnin-differential-trail-three-rounds saturnin-tools/saturnin-differential saturnin-tools/saturnin-linear-trail \
	saturnin-tools/saturnin-correlation saturnin-tools/saturnin-trail-core saturnin-tools/saturnin-linear-trail-three-rounds \
	saturnin-tools/saturnin-differential-unrounded \
	parse-and-convolve \
	bin

.SUFFIXES: .cpp .c .o
%.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@
%.o : %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
