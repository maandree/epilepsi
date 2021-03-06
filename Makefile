# epilepsi — Overkill media player for us organised enthusiasts
# 
# Copyright © 2012, 2013  Mattias Andrée (maandree@member.fsf.org)
# 
# Copying and distribution of this file, with or without modification,
# are permitted in any medium without royalty provided the copyright
# notice and this notice are preserved.  This file is offered as-is,
# without any warranty.
# 
# (GNU All Permissive License)


JAVA=java7
JAVAC=javac7

JAVA_DEBUG=-g
JAVA_WARN=-Xlint
JAVA_DIRS=-d bin -s src

JAVA_FLAGS=$(JAVA_DEBUG) $(JAVA_WARN) $(JAVA_DIRS)
JAVA_FILES=$$(find src | grep '.\.java$$')
JAVA_LIBS=$$(echo lib/*.jar | sed -e 's_ _:_g')

C_DEBUG=-g
C_WARN=-Wall -Wextra -pedantic
CFLAGS=$(C_DEBUG) $(C_WARN)
CPPFLAGS=
LDFLAGS=
C_FALGS=$(CFLAGS) $(CPPFLAGS) $(LDFLAGS)



all: java

java:
	mkdir -p bin
	if [ -d lib ] && [ ! $$(find lib | grep '.\.jar$$' | wc -l) = 0]; then  \
	  $(JAVAC) $(JAVA_FLAGS) -cp src:$(JAVA_LIBS) $(JAVA_FILES);            \
	else                                                                    \
	  $(JAVAC) $(JAVA_FLAGS) -cp src $(JAVA_FILES);                         \
	fi


test-checksum:
	$(JAVA) -cp bin test.ChecksumTest


.PHONY: clean
clean:
	rm -r bin || exit 0
	find | grep '.\.class$' | while read file; do rm "$file"; done

