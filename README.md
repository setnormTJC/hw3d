See the tutorial at: 

https://www.youtube.com/playlist?list=PLqCJpWy5Fohd3S7ICFXwUomYW0Wv67pDD


This project (as of Nov. 16, 2025) will not run without incorporating a library called "Assimp". There is more than one way to do this. One decent approach to doing this can probably be found in: 

https://www.youtube.com/watch?v=oci7xJEg6sU

I added a COPY of the required dll file to: ($ProjectDir)external/lib/assimp

When you clone this repo, MOVE that dll file to ($ProjectDir)../x64/Debug/