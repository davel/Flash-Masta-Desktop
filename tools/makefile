BUILDDIR=./build
SOURCEDIR=./src

CC=gcc
CFLAGS=-Wno-dangling-else -pthread -ldl

CPPC=g++
CPPFLAGS=-Wno-dangling-else -std=c++11 -pthread

TARGETS=build-database


first: build-database


$(BUILDDIR)/%.o: $(SOURCEDIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CPPC) $(CPPFLAGS) -c $< -o $@

$(BUILDDIR)/%.o: $(SOURCEDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@


build-database: $(addprefix $(BUILDDIR)/, build-database.o ngp-games-row.o ws-games-row.o ngp-cart-row.o sqlite/sqlite3.o)
	$(CPPC) $(CPPFLAGS) $^ -ldl -o $@

clean:
	@find $(BUILDDIR) -name "*.o" -type f -delete
	@rm -f $(TARGETS)
