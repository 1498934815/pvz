$(TARGET):src=$(strip $($(@)_src))
$(TARGET):out=$(strip $(call fix-name, $(@)))
$(TARGET):flag=$(strip $(call fix-flag, $(out), $($(@)_flag)))
$(TARGET):$(DEP)
	@ echo "$(src) => $@"
	@ $(CC) -o $(out) $(src) $(CC_FLAG) $(flag)
	@ $(STRIP) $(out)
