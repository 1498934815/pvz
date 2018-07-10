define islib
$(findstring lib, $(1))
endef
define fix-name
$(strip $(1:lib%=lib%.so))
endef
define undo-name
$(strip $(1:%.so=%))
endef
define fix-flag
$(if $(call islib, $1), \
	$(2) -fPIC -shared, \
	$(2))
endef
define reg_rule
$(foreach m,$(1), \
	$(eval TARGET := $(m)) \
	$(eval DEP := $($(call undo-name, $(m))_src) $(inc) $(MAKEFILE_LIST)) \
	$(eval include build/reg_rule.mk))
endef
rmodules := $(foreach m, $(MODULE), $(call fix-name, $(m)))
all:$(rmodules)
$(call reg_rule, $(rmodules))
.PHONY:clean
clean:
	-@ rm -rf $(rmodules)
