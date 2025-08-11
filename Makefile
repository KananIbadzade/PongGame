# Makefile ─ build Pong on macOS (Apple-silicon Homebrew install)

CXX      = clang++
CXXFLAGS = -std=c++17 -I/opt/homebrew/include
LDFLAGS  = -L/opt/homebrew/lib -lraylib \
           -framework CoreVideo -framework IOKit -framework Cocoa \
           -framework GLUT -framework OpenGL

pong: main.cpp
	$(CXX) $< -o $@ $(CXXFLAGS) $(LDFLAGS)
