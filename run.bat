@echo off

set MODE=display
if not "%1"=="" (
    set MODE=%1
)

set CONFIG=debug
if not "%2"=="" (
    set CONFIG=%2
)

if /I "%MODE%"=="build" (
    if exist "venv\Scripts\activate.bat" (
        call venv\Scripts\activate.bat
    ) else (
        echo [ERROR] Virtual environment activation script not found!
        exit /b 1
    )

    if not exist "build" (
        echo [INFO] Build directory does not exist. Creating it...
        mkdir build
        if %ERRORLEVEL% neq 0 (
            echo [ERROR] Failed to create build directory!
            exit /b 1
        )
    )

    cmake -S . -B build
    if %ERRORLEVEL% neq 0 (
        echo [ERROR] CMake configuration failed!
        exit /b 1
    )

    cmake --build build --config=%CONFIG%
    if %ERRORLEVEL% neq 0 (
        echo [ERROR] Build failed!
        exit /b 1
    )

    if exist "build\%CONFIG%\raytracer.exe" (
        build\%CONFIG%\raytracer.exe > image.ppm
        if %ERRORLEVEL% neq 0 (
            echo [ERROR] Failed to run raytracer.exe!
            exit /b 1
        )
    ) else (
        echo [ERROR] Executable build\%CONFIG%\raytracer.exe not found!
        exit /b 1
    )

    if exist "image.ppm" (
        echo [INFO] Image file image.ppm successfully created.
    ) else (
        echo [ERROR] Image file image.ppm was not created!
        exit /b 1
    )
)

if exist "image.ppm" (
    if exist "image_viewer.py" (
        call venv\Scripts\activate.bat
        python image_viewer.py image.ppm
        if %ERRORLEVEL% neq 0 (
            echo [ERROR] Failed to run image_viewer.py!
            exit /b 1
        )
    ) else (
        echo [ERROR] Python script image_viewer.py not found!
        exit /b 1
    )
) else (
    echo [ERROR] Image file image.ppm not found! Please run with 'build' mode to generate it.
    exit /b 1
)

echo [INFO] All tasks completed successfully.
exit /b 0
