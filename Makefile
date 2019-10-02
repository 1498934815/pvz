MODULE := \
	client \
	server
define src_under
$(shell find $(1) -type f)
endef
common := \
	$(call src_under,src/common)
inc := $(call src_under,inc)
client_src := \
	$(call src_under,src/client) \
	$(common) \
	src/server/PvzCheats.cc # Auxiliary options defined
client_out := pvz_client

git_hash := $(shell git rev-list --all --max-count=1 --abbrev-commit)
git_repo := $(shell git config --get remote.origin.url)
git_branch := $(shell git symbolic-ref --short -q HEAD)
local_version := $(shell date +'%y%m%d')

server_src := \
	$(call src_under,src/server) \
	$(common)
server_flag := -shared -ldl -llog -DSERVER
server_out := libpvz_server.so
CC_FLAG := -Iinc -Wall -Wstrict-prototypes -std=c++0x -fPIC \
	-static-libstdc++ \
	-DGIT_HASH=\"$(git_hash)\" \
	-DGIT_REPO=\"$(git_repo)\" \
	-DGIT_BRANCH=\"$(git_branch)\" \
	-DAUTHOR=\"AS魇梦蚀\"

ifeq ($(NDK_BUILD),true)
	# We are use NDK-R19c now
	NDK ?= $(HOME)/android-ndk-r20
	NDK_TOOLCHAIN ?= $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin
	CC := $(NDK_TOOLCHAIN)/armv7a-linux-androideabi16-clang++
	STRIP := $(NDK_TOOLCHAIN)/arm-linux-androideabi-strip
	CC_FLAG += -pie -DLOCAL_VERSION=$(local_version)
else
	CC := g++
	STRIP := strip
	CC_FLAG += -DLOCAL_VERSION=0
endif
ifneq ($(DEBUG),)
	CC_FLAG += -g -DDEBUG
	STRIP := \#
endif
include build/build.mk
define make_release
	make NDK_BUILD=true --no-print-directory
endef
.PHONY:release local_install __local_install_build __local_install_build
release:
	$(call make_release)
__local_install_build:release
	@ ./tools/build_release.py $(local_version) src/prebuilts/com.popcap.pvz_na:lib/armeabi:$(server_out) src/prebuilts/PVZ_CHEATER:assets:$(client_out)
	@ zip -j out/PVZ_CHEATER_$(local_version)-$(git_hash).zip out/*.apk
__local_install_install:release
	@ for i in out/*.apk;do \
		adb install -r $$i; \
	done
local_install:__local_install_build __local_install_install
