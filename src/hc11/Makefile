AS11 = asXX

OBJFILES = nokia.s19

all: $(OBJFILES)

nokia.s19: nokia.asm
	@$(AS11) -lo nokia.asm

clean:
	rm -f *.s19 *.lst
