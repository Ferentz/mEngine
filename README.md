# Minigin

Minigin is a very small project using [SDL3](https://www.libsdl.org/) and [glm](https://github.com/g-truc/glm) for 2D c++ game projects. It is in no way a game engine, only a barebone start project where everything sdl related has been set up. It contains glm for vector math, to aleviate the need to write custom vector and matrix classes.

[![Build Status](https://github.com/Ferentz/mEngine/actions/workflows/cmake.yml/badge.svg)](https://github.com/Ferentz/cmake/actions)
[![Build Status](https://github.com/Ferentz/mEngine/blob/main/.github/workflows/emscripten.yml/badge.svg)](https://github.com/Ferentz/emscripten/actions)
[![GitHub Release](https://img.shields.io/github/v/release/Ferentz/mEngine?logo=github&sort=semver)](https://github.com/Ferentz/mEngine/releases/latest)

## engine specifics

the engine is curently a lightweight setup for grid games.
it hass ome basic grid functinalities that can be build upon. such as a base tile class, build in funcitionality to move along a grid and of course, a grid that works with both, creating a tildy system.

the engine has sound capabilituies with a set number of tracks
it's also psible to easily see what inuts are available for you to attach commands to or, for you to simply check for any button presses from anywhere. (gamepad minimaly supported)

the most unusual design choice would be to keep track of what scenes objects belong to, this is with the idea of scene settup, should you wish to append to a scene that isnt active.
other than that there is also a split between render component and non-render component. ensuring that the render functions doesnt become bloated with early exits from components that have nothing to do with rendering.

git : https://github.com/Ferentz/mEngine

