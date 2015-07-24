.PHONY all:

all: createDiskinfo createDisklist

createDiskinfo:
	gcc -Wall diskinfo.c commonFunctions.c -o diskinfo

createDisklist:
	gcc -Wall disklist.c commonFunctions.c -o disklist

createDiskget:
	gcc -Wall diskget.c commonFunctions.c -0 diskget

.PHONY clean:

clean:
	-rm diskinfo disklist diskget
