$(TARGET):uname=$(call undo-name, $@)
$(TARGET):src=$(strip $($(uname)_src))
$(TARGET):out=$(strip $@)
$(TARGET):flag=$(strip $(call fix-flag, $(out), $($(uname)_flag)))
$(TARGET):$(DEP)
	@ echo "$(src) => $@"
	@ $(CC) -o $(out) $(src) $(CC_FLAG) $(flag)
	@ $(STRIP) $(out)
