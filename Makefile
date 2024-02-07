SUBDIRS=rx-unix tx-msdos image-msdos

all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@

.PHONY: all $(SUBDIRS)
