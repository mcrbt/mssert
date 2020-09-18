NAME = mssert
VERSION = 0.1.0
CC = /usr/bin/gcc
CXX = /usr/bin/g++
STRIP = /usr/bin/strip --strip-all
CSTD = -ansi
CXXSTD = -std=c++98
CWARN = -Wall -Werror -Wextra -Wunused -Wstrict-prototypes \
	-Wpedantic -Wredundant-decls -Wsuggest-attribute=const \
	-Wsuggest-attribute=noreturn -Wsuggest-attribute=pure \
	-Wswitch -Wswitch-default -Wswitch-enum -pedantic \
	-pedantic-errors
CXXWARN = -Wall -Werror -Wextra -Wdeprecated -Wunused \
	-Wparentheses -Wpedantic -Wredundant-decls \
	-Wsuggest-attribute=const -Wsuggest-attribute=noreturn \
	-Wsuggest-attribute=pure -Wsuggest-final-methods \
	-Wsuggest-final-types -Wsuggest-override \
	-Wtautological-compare -Wtemplates -Wswitch \
	-Wswitch-default -Wswitch-enum -Wuseless-cast \
	-pedantic -pedantic-errors
OPTIMIZE = -fdelete-null-pointer-checks -fexpensive-optimizations \
	-ffinite-math-only -finline-functions-called-once \
	-finline-small-functions -fmerge-all-constants \
	-fmerge-constants -fmove-loop-invariants \
	-fomit-frame-pointer -foptimize-register-move \
	-foptimize-sibling-calls -fregmove -freorder-blocks \
	-freorder-functions -fwhole-program -fshort-enums \
	-fshort-wchar -Os
DEFINES = -DDEBUG -DVERSION='"$(VERSION)"' \
	-DMSSERT -DMSSERT_BUFFER_SIZE=32
INCLUDES = -Iinclude
CFLAGS = -x c $(CSTD) $(CWARN) $(OPTIMIZE) \
	$(INCLUDES) $(DEFINES)
CXXFLAGS = -x c++ $(CXXSTD) $(CXXWARN) $(OPTIMIZE) \
	-fstrict-enums $(INCLUDES) $(DEFINES)
TAR = /usr/bin/tar cJf
RM = /usr/bin/rm -rf

.PHONY: all clean pack version

all: c$(NAME) cpp$(NAME)

c$(NAME): example/$(NAME).c include/$(NAME).h
		$(CC) -o $@ -x c $(CFLAGS) $<
		$(STRIP) $@

cpp$(NAME): example/$(NAME).cpp include/$(NAME).h
	$(CXX) -o $@ $(CXXSTD) -x c++ $(CXXFLAGS) $<
	$(STRIP) $@

clean:
	$(RM) {c,cpp}$(NAME) include/*~ example/*~ *~ core.*

pack:
	$(TAR) $(NAME)_$(VERSION).txz include/$(NAME).h \
	example/$(NAME).{c,cpp} Makefile LICENSE \
	README.md .gitignore

version:
	@echo "$(NAME) version $(VERSION)"
