


[//]: # "Minigin is a very small project using [SDL3](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. "

[//]: # "[![Build Status](https://github.com/Ferentz/mEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/Ferentz/cmake/actions)
[![Build Status](https://github.com/Ferentz/mEngine/blob/main/.github/workflows/emscripten.yml/badge.svg)](https://github.com/Ferentz/emscripten/actions)
[![GitHub Release](https://img.shields.io/github/v/release/Ferentz/mEngine?logo=github&sort=semver)](https://github.com/Ferentz/mEngine/releases/latest)"
# overvieuw
mEngine is a small project that contains a 2D game engine as well as a remake of digger as an example usage case.
it makes use of sdl3 as well as xinput(for controller input)

in the engine :
- sprite drawing
- sound playing, pausing, looping
- collision detetction
- grid functionality (drawing, moving, collision interactions)
- gamepad and keyboard input
- scenes + scene graph
- complete gameloop + fixed time steps in case of lag
- event system (targeted + broadcasted)
- game objects and basic object components that can be build from

the Engine makes use of a variety of programing design patterns as specified in the [programing patterns book](https://gameprogrammingpatterns.com/contents.html) and adheres to the c++ guidelines for readable and maintainable code.

git : https://github.com/Ferentz/mEngine

