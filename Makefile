MODULE := \
	pvz_client \
	libpvz_server

common := src/utils.c src/pvz.c src/com.c
pvz_client_src := src/client.c $(common)

git_hash := $(shell git rev-list --all --max-count=1 --abbrev-commit)

libpvz_server_src := src/server.c src/cheat.c $(common)
libpvz_server_flag := -shared -fPIC -ldl

inc := $(shell find inc)
CC_FLAG := -Iinc -Wall -std=c99 -DGIT_HASH=\"$(git_hash)\"
ifeq ($(NDK_BUILD),true)
	NDK ?= $(HOME)/android-ndk-r14b
	NDK_STANDALONE ?= $(HOME)/ndk/arm
	NDK_TOOCHAIN ?= $(NDK_STANDALONE)/bin/arm-linux-androideabi
	NDK_SYSROOT ?= $(NDK)/platforms/android-12/arch-arm
	CC := $(NDK_TOOCHAIN)-gcc
	STRIP := $(NDK_TOOCHAIN)-strip
	CC_FLAG += -fPIC -pie --sysroot=$(NDK_SYSROOT)
else
	CC := gcc
	STRIP := strip
endif
include build/build.mk
define make_release
	make NDK_BUILD=true --no-print-directory
endef
.PHONY:release
release:
	$(call make_release)
