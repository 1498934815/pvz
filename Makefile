MODULE := \
	pvz_client \
	libpvz_server

common := src/utils.c src/pvz.c src/com.c
pvz_client_src := src/client.c $(common)

git_hash := $(shell git rev-list --all --max-count=1 --abbrev-commit)
git_repo := $(shell git config --get remote.origin.url)
git_branch := $(shell git symbolic-ref --short -q HEAD)

libpvz_server_src := src/server.c src/cheat.c $(common)
libpvz_server_flag := -shared -fPIC -ldl

inc := $(shell find inc)
CC_FLAG := -Iinc -Wall -Wstrict-prototypes -std=c99 \
	-DGIT_HASH=\"$(git_hash)\" \
	-DGIT_REPO=\"$(git_repo)\" \
	-DGIT_BRANCH=\"$(git_branch)\" \
	-DTIEBA_POST_URL=\"https://tieba.baidu.com/p/5645499737\" \
	-DAUTHOR=\"AS魇梦蚀\"

ifeq ($(NDK_BUILD),true)
	# We are use NDK-R17 current
	NDK_STANDALONE ?= $(HOME)/ndk
	NDK_TOOCHAIN ?= $(NDK_STANDALONE)/bin/arm-linux-androideabi
	CC := $(NDK_TOOCHAIN)-clang
	STRIP := $(NDK_TOOCHAIN)-strip
	CC_FLAG += -fPIC -pie
else
	CC := gcc
	STRIP := strip
endif
ifneq ($(DEBUG),)
	CC_FLAG += -g -DDEBUG
	STRIP := \#
endif
include build/build.mk
define make_release
	make NDK_BUILD=true --no-print-directory
endef
.PHONY:release
release:
	$(call make_release)
