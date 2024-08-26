# Quest-OpenXR-Vulkan

A minimal (well, as minimal as you can get with OpenXR + Vulkan) quest app for rendering the scene from the Vulkan tutorial.

A lot of other Quest OpenXR apps have a weird Android folder structure which really isn't needed, this app demonstrates how little is actually needed to run a native app.

## Compiling shaders

Believe it or not, Android Studio includes functionality to do this for you. You just need to place the shaders in src/main/shaders, and they get packaged up into the
app assets directory.