set mypath=%cd%

%mypath%\libs\Vulkan\1.3.236.0\Bin\glslc.exe default.vert -o default.vert.spv
%mypath%\libs\Vulkan\1.3.236.0\Bin\glslc.exe default.frag -o default.frag.spv

Start %mypath%\..\x64\Debug\Engine_03.exe

pause