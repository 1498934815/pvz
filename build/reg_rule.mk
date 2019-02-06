$(OUT):src:=$(SRC)
$(OUT):flag:=$(FLAG)
$(TARGET):$(OUT)
$(OUT):$(DEP)
	@ echo "$@ <= $(src)"
	@ $(CC) -o $@ $(src) $(CC_FLAG) $(flag)
	@ $(STRIP) $@
