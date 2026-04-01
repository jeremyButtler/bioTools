LD=cc
CFLAGS= -O0 -std=c89 -g -Werror -Wall -Wextra -Wpedantic -Wundef -Waggregate-return -Wwrite-strings -Wstrict-prototypes -Wpointer-arith -Wshadow -Wunreachable-code -Winit-self
O=o.test

build: testMkPng fontLoadTest fontHorzDrawTest fontVertDrawTest fontCFileTest fontFromCFileTest alphbetTest

testMkPng: testMkPng.$O
	$(LD) -o testMkPng ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngDraw.$O testMkPng.$O
testMkPng.$O:	testMkPng.c ../pngDraw.$O ../../genLib/endLine.h
	$(CC) -c -o testMkPng.$O $(CFLAGS) testMkPng.c

fontLoadTest: fontLoadTest.$O
	$(LD) -o fontLoadTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngFont.$O fontLoadTest.$O
fontLoadTest.$O:	fontLoadTest.c ../pngFont.$O
	$(CC) -c -o fontLoadTest.$O $(CFLAGS) fontLoadTest.c

fontHorzDrawTest: fontHorzDrawTest.$O
	$(LD) -o fontHorzDrawTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngFont.$O fontHorzDrawTest.$O
fontHorzDrawTest.$O:	fontHorzDrawTest.c ../pngFont.$O
	$(CC) -c -o fontHorzDrawTest.$O $(CFLAGS) fontHorzDrawTest.c

fontVertDrawTest: fontVertDrawTest.$O
	$(LD) -o fontVertDrawTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngFont.$O fontVertDrawTest.$O
fontVertDrawTest.$O:	fontVertDrawTest.c ../pngFont.$O
	$(CC) -c -o fontVertDrawTest.$O $(CFLAGS) fontVertDrawTest.c

fontCFileTest: fontCFileTest.$O
	$(LD) -o fontCFileTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngFont.$O fontCFileTest.$O
fontCFileTest.$O:	fontCFileTest.c ../pngFont.$O
	$(CC) -c -o fontCFileTest.$O $(CFLAGS) fontCFileTest.c

fontFromCFileTest: fontFromCFileTest.$O
	$(LD) -o fontFromCFileTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngFont.$O fontFromCFileTest.$O ../defaultFont.$O
fontFromCFileTest.$O:	fontFromCFileTest.c ../defaultFont.$O ../pngFont.$O
	$(CC) -c -o fontFromCFileTest.$O $(CFLAGS) fontFromCFileTest.c
../defaultFont.$O:	../defaultFont.c ../defaultFont.h
	$(CC) -c -o ../defaultFont.$O $(CFLAGS) ../defaultFont.c

alphbetTest: alphbetTest.$O
	$(LD) -o alphbetTest ../../genLib/ulCp.$O ../../genLib/base10str.$O ../../genLib/endin.$O ../../genLib/checkSum.$O ../mkPng.$O ../pngFont.$O alphbetTest.$O
alphbetTest.$O:	alphbetTest.c ../pngFont.$O
	$(CC) -c -o alphbetTest.$O $(CFLAGS) alphbetTest.c


../pngDraw.$O: ../pngDraw.h ../pngDraw.c ../mkPng.$O
	$(CC) -c -o ../pngDraw.$O $(CFLAGS) ../pngDraw.c
../pngFont.$O: ../pngFont.h ../pngFont.c ../mkPng.$O ../../genLib/base10str.$O ../../genLib/ulCp.$O
	$(CC) -c -o ../pngFont.$O $(CFLAGS) ../pngFont.c



../mkPng.$O: ../mkPng.h ../mkPng.c ../../genLib/checkSum.$O ../../genLib/64bit.h
	$(CC) -c -o ../mkPng.$O $(CFLAGS) ../mkPng.c

../../genLib/checkSum.$O: ../../genLib/checkSum.c ../../genLib/checkSum.h ../../genLib/endin.$O
	$(CC) -c -o ../../genLib/checkSum.$O $(CFLAGS) ../../genLib/checkSum.c

../../genLib/endin.$O: ../../genLib/endin.c ../../genLib/endin.h
	$(CC) -c -o ../../genLib/endin.$O $(CFLAGS) ../../genLib/endin.c

../../genLib/ulCp.$O: ../../genLib/ulCp.c ../../genLib/ulCp.h
	$(CC) -c -o ../../genLib/ulCp.$O $(CFLAGS) ../../genLib/ulCp.c

../../genLib/base10str.$O: ../../genLib/base10str.c ../../genLib/base10str.h
	$(CC) -c -o ../../genLib/base10str.$O $(CFLAGS) ../../genLib/base10str.c



cleanAll: clean cleanBin

clean:
	if [ -f ../../genLib/ulCp.$O ]; then rm ../../genLib/ulCp.$O; fi;
	if [ -f ../../genLib/base10str.$O ]; then rm ../../genLib/base10str.$O; fi;
	if [ -f ../../genLib/endin.$O ]; then rm ../../genLib/endin.$O; fi;
	if [ -f ../../genLib/checkSum.$O ]; then rm ../../genLib/checkSum.$O; fi;
	if [ -f ../mkPng.$O ]; then rm ../mkPng.$O; fi;
	if [ -f ../pngDraw.$O ]; then rm ../pngDraw.$O; fi;
	if [ -f ../pngFont.$O ]; then rm ../pngFont.$O; fi;
	if [ -f ../defaultFont.$O ]; then rm ../defaultFont.$O; fi;
	if [ -f testMkPng.$O ]; then rm testMkPng.$O; fi;
	if [ -f fontLoadTest.$O ]; then rm fontLoadTest.$O; fi;
	if [ -f fontHorzDrawTest.$O ]; then rm fontHorzDrawTest.$O; fi;
	if [ -f fontVertDrawTest.$O ]; then rm fontVertDrawTest.$O; fi;
	if [ -f fontCFileTest.$O ]; then rm fontCFileTest.$O; fi;
	if [ -f fontFromCFileTest.$O ]; then rm fontFromCFileTest.$O; fi;
	if [ -f alphbetTest.$O ]; then rm alphbetTest.$O; fi;

cleanBin:
	if [ -f testMkPng ]; then rm testMkPng; fi;
	if [ -f fontLoadTest ]; then rm fontLoadTest; fi;
	if [ -f fontHorzDrawTest ]; then rm fontHorzDrawTest; fi;
	if [ -f fontVertDrawTest ]; then rm fontVertDrawTest; fi;
	if [ -f fontCFileTest ]; then rm fontCFileTest; fi;
	if [ -f fontFromCFileTest ]; then rm fontFromCFileTest; fi;
	if [ -f alphbetTest ]; then rm alphbetTest; fi;
