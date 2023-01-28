mkdir -p external
git submodule add -f https://github.com/glfw/glfw.git external/glfw
cmake -S . -B build