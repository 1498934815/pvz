define reg_rule
$(foreach m,$(1), \
	$(eval TARGET := $(m)) \
	$(eval OUT := $($(m)_out)) \
	$(eval SRC := $(strip $($(m)_src))) \
	$(eval FLAG := $(strip $($(m)_flag))) \
	$(eval DEP := $($(m)_src) \
	$($(m)_dep) \
	$(inc) $(MAKEFILE_LIST)) \
	$(eval include build/reg_rule.mk))
endef
rmodules := $(foreach m, $(MODULE), $($(m)_out))
.SUFFIXES:
all:$(MODULE)
$(call reg_rule, $(MODULE))
.PHONY:clean
clean:
	-@ rm -rf $(rmodules)
