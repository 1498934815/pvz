$(TARGET):src=$($(@)_src)
$(TARGET):out=$(@:lib%=lib%.so)
$(TARGET):flag=$(if $(findstring lib, $@), \
	$($(@)_flag) -shared -fPIC, \
	$($(@)_flag))
$(TARGET):
$(TARGET):$(DEP)
	@ echo "$(src) => $@"
	@ $(CC) -o $(out) $(src) $(CC_FLAG) $(flag)
	@ $(STRIP) $(out)
