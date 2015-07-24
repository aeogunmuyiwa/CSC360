.PHONY all:

all: createDiskinfo createDisklist

createDiskinfo:
	gcc -Wall diskinfo.c commonFunctions.c -o diskinfo

createDisklist:
	gcc -Wall disklist.c commonFunctions.c -o disklist

.PHONY clean:

clean:
	-rm diskinfo disklist
