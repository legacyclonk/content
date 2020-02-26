C4GROUP:=c4group
ZIP=zip
PACK_CMD:=cp -r SRC TARGET && $(C4GROUP) TARGET -p
ZIP_CMD:=$(ZIP) -n c4f:c4g:c4d:c4s -j TARGET SRCS

PACKS:=$(wildcard *.c4?)
PACKED_PACKS:=$(patsubst %,packed/%,$(PACKS))
ARCHIVE:=packed/lc_content.zip
LICENSES:=clonk_content_license.txt clonk_trademark_license.txt

.PHONY: all clean

all: $(PACKED_PACKS) $(ARCHIVE)

packed/.exists: Makefile
	@test -f packed/.exists || (mkdir -p packed && touch packed/.exists)

packed/%: % packed/.exists
	$(RM) -r $@
	$(subst TARGET,$@,$(subst SRC,$<,$(PACK_CMD)))

$(ARCHIVE): $(PACKED_PACKS) $(LICENSES)
	$(RM) $@
	$(subst TARGET,$@,$(subst SRCS,$^,$(ZIP_CMD)))

clean:
	$(RM) -r packed
