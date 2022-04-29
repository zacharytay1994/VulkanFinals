rem GLM
rmdir /s /q "./dependencies/glm/"
git clone https://github.com/g-truc/glm.git "./dependencies/glm/"

rem stb (stb_image)
rmdir /s /q "./dependencies/stb/"
git clone https://github.com/nothings/stb.git "./dependencies/stb/"

rem tinyddsloader
rmdir /s /q "./dependencies/tinyddsloader"
git clone https://github.com/benikabocha/tinyddsloader.git "./dependencies/tinyddsloader/"

rem assimp
rmdir /s /q "./dependencies/assimp"
git clone https://github.com/assimp/assimp.git "./dependencies/assimp/"
cd "./dependencies/assimp/"
call BUILDBINARIES_EXAMPLE.bat
pause