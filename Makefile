MODULE := \
	pvz_client \
	libpvz_server.so
common := src/utils.c src/pvz.c
pvz_client_src := src/client.c $(common)
libpvz_server.so_src := src/server.c src/cheat.c $(common)
libpvz_server.so_flag := -shared -fPIC
inc := $(shell find inc)
CC_FLAG := -Iinc -Wall -std=c99
ifeq ($(NDK_BUILD),true)
  NDK ?= $(HOME)/android-ndk-r14b
  ifeq ($(ARM64),true)
		NDK_STANDALONE ?= $(HOME)/ndk/arm64
		NDK_TOOCHAIN ?= $(NDK_STANDALONE)/bin/aarch64-linux-android
		NDK_SYSROOT ?= $(NDK)/platforms/android-24/arch-arm64
  else
		NDK_STANDALONE ?= $(HOME)/ndk/arm
		NDK_TOOCHAIN ?= $(NDK_STANDALONE)/bin/arm-linux-androideabi
		NDK_SYSROOT ?= $(NDK)/platforms/android-24/arch-arm
	endif
	CC := $(NDK_TOOCHAIN)-gcc
	STRIP := $(NDK_TOOCHAIN)-strip
	CC_FLAG += -fPIC -pie --sysroot=$(NDK_SYSROOT)
else
	CC := gcc
	STRIP := strip
endif
all:$(MODULE)
$(foreach m,$(MODULE),$(eval TARGET := $(m))$(eval DEP := $($(m)_src) $(inc))$(eval include build/reg_rule.mk))
define make_release
	make NDK_BUILD=true --no-print-directory $(1)
	@ mkdir -p release/$(dir $(2))
	@ cp cheater release/$(2)
	@ make clean
endef
.PHONY:release
release:
	$(call make_release,,arm/pvz_client)
	$(call make_release,ARM64=true,aarch64/pvz_client)
.PHONY:clean
clean:
	-@ rm -rf $(MODULE)
