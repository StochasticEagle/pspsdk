MODULES := $(sort $(patsubst %/Makefile.test,%,$(wildcard */Makefile.test)))

.PHONY: all clean list $(MODULES)

all: $(MODULES)

$(MODULES):
	$(MAKE) -C $@ -f Makefile.test all

clean:
	@set -e; for module in $(MODULES); do $(MAKE) -C "$$module" -f Makefile.test clean; done

list:
	@printf '%s\n' $(MODULES)
