set mypath=%cd%

%mypath%\..\libs\Vulkan\1.3.236.0\Bin\glslc.exe default.vert -o default.vert.spv
%mypath%\..\libs\Vulkan\1.3.236.0\Bin\glslc.exe default.frag -o default.frag.spv

%mypath%\..\libs\Vulkan\1.3.236.0\Bin\glslc.exe pointLight.vert -o pointLight.vert.spv
%mypath%\..\libs\Vulkan\1.3.236.0\Bin\glslc.exe pointLight.frag -o pointLight.frag.spv

pause