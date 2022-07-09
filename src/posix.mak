# Proxy Makefile for backwards compatibility after move to /compiler/src


all:
	$(MAKE) -C ../compiler/src -f posix.mak $@

auto-tester-build:
	$(MAKE) -C ../compiler/src -f posix.mak $@

auto-tester-test:
	$(MAKE) -C ../compiler/src -f posix.mak $@

buildkite-test:
	$(MAKE) -C ../compiler/src -f posix.mak $@

toolchain-info:
	$(MAKE) -C ../compiler/src -f posix.mak $@

clean:
	$(MAKE) -C ../compiler/src -f posix.mak $@

test:
	$(MAKE) -C ../compiler/src -f posix.mak $@

html:
	$(MAKE) -C ../compiler/src -f posix.mak $@

tags:
	$(MAKE) -C ../compiler/src -f posix.mak $@

install:
	$(MAKE) -C ../compiler/src -f posix.mak $@

check-clean-git:
	$(MAKE) -C ../compiler/src -f posix.mak $@

style:
	$(MAKE) -C ../compiler/src -f posix.mak $@
