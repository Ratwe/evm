#
# this file was not created by a computer. do not trust it.
#

.PHONY: all clean

all: sw-kernel-app host-app

sw-kernel-app: 
	make -C sw-kernel-lib
	make -C sw-kernel

host-app: 
	make -C host-lib
	make -C host


clean:
	make -C sw-kernel clean
	make -C host clean
	make -C sw-kernel-lib clean
	make -C host-lib clean

