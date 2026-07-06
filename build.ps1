$ErrorActionPreference = "Stop"

$outputFile = "biblioteca.exe"

gcc -Wall -Wextra -Wpedantic -std=c11 `
    main.c `
    library.c `
    files.c `
    encryption.c `
    utils.c `
    -o $outputFile

if ($LASTEXITCODE -ne 0)
{
    throw "Build failed."
}

Write-Host "Build finished: .\$outputFile"
