@echo off

:: Set the constant paths
set "SHADER_DIR=C:\Visual_Studio\Projects\VulkanTest\VulkanTest\Shaders"
set "GLSLC_PATH=C:\vulkanSDK\Bin\glslc.exe"

:: Notify that shader compilation is starting
echo Starting shader compilation...

:: Compile .frag files to .frag.spv
echo Compiling .frag shaders...
for %%f in ("%SHADER_DIR%\*.frag") do (
    echo Compiling %%f...
    "%GLSLC_PATH%" %%f -o "%%~dpnf.frag.spv"
    if %ERRORLEVEL% neq 0 (
        echo Error compiling %%f
    ) else (
        echo Successfully compiled %%f to %%~dpnf.spv
    )
)

:: Compile .vert files to .vert.spv
echo Compiling .vert shaders...
for %%f in ("%SHADER_DIR%\*.vert") do (
    echo Compiling %%f...
    "%GLSLC_PATH%" %%f -o "%%~dpnf.vert.spv"
    if %ERRORLEVEL% neq 0 (
        echo Error compiling %%f
    ) else (
        echo Successfully compiled %%f to %%~dpnf.spv
    )
)

