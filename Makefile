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
	$(common)
client_out := pvz_client

git_hash := $(shell git rev-list --all --max-count=1 --abbrev-commit)
git_repo := $(shell git config --get remote.origin.url)
git_branch := $(shell git symbolic-ref --short -q HEAD)

server_src := \
	$(call src_under,src/server) \
	$(call src_under,src/module) \
	$(common)
server_flag := -shared -ldl
server_out := libpvz_server.so
CC_FLAG := -Iinc -Wall -Wstrict-prototypes -std=c++0x -fPIC \
	-DGIT_HASH=\"$(git_hash)\" \
	-DGIT_REPO=\"$(git_repo)\" \
	-DGIT_BRANCH=\"$(git_branch)\" \
	-DTIEBA_POST_URL=\"https://tieba.baidu.com/p/5645499737\" \
	-DAUTHOR=\"AS魇梦蚀\"

ifeq ($(NDK_BUILD),true)
	# We are use NDK-R17 current
	NDK_STANDALONE ?= $(HOME)/ndk
	NDK_TOOCHAIN ?= $(NDK_STANDALONE)/bin/arm-linux-androideabi
	CC := $(NDK_TOOCHAIN)-clang++
	STRIP := $(NDK_TOOCHAIN)-strip
	CC_FLAG += -pie
else
	CC := g++
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
