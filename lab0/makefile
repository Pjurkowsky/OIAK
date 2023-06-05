all: hello writeText compareText

hello: hello.s
	as hello.s -o hello.o
	ld hello.o -o hello

writeText: writeText.s
	as writeText.s -o writeText.o
	ld writeText.o -o writeText

compareText: compareText.s
	as compareText.s -o compareText.o
	ld compareText.o -o compareText
