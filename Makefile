C4GROUP:=c4group
PACK_CMD:=cp -r SRC TARGET && $(C4GROUP) TARGET -p

PACKS:=$(wildcard *.c4?)

.PHONY: all clean

all: $(patsubst %,packed/%,$(PACKS))

packed:
	mkdir packed

packed/%: % packed
	-rm -f $@
	$(subst TARGET,$@,$(subst SRC,$<,$(PACK_CMD)))

clean:
	-rm -rf packed
