CXX = clang++
CXXFLAGS = -Wall -c -std=c++11 -Isrc/libtextmode -I/usr/local/include
LDFLAGS = -L/usr/local/lib

OUT = ans

$(OUT): ans.o\
		ansi.o\
		ansiedit.o\
		artworx.o\
		ascii.o\
		binary_text.o\
		file.o\
		ice_draw.o\
		image.o\
		image_data.o\
		lodepng.o\
		options.o\
		palettes.o\
		pc_board.o\
		sauce.o\
		screen.o\
		text.o\
		textmode.o\
		tundra_draw.o\
		utf8.o\
		xbin.o
	$(CXX) $(LDFLAGS) $^ -o $@

%.o: src/libtextmode/%.cpp src/libtextmode/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: src/libtextmode/file_formats/%.cpp src/libtextmode/file_formats/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: src/libtextmode/image/%.cpp src/libtextmode/image/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: src/libtextmode/term/%.cpp src/libtextmode/term/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

%.o: src/lodepng/%.cpp src/lodepng/%.h
	$(CXX) $(CXXFLAGS) $< -o $@

ans.o: src/ans.cpp
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm -f *.o
	rm -f $(OUT)
