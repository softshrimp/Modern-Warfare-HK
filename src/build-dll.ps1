param([Parameter(Mandatory=$true)][string]$ZigExe)
$ErrorActionPreference='Stop'
$zigResolved=(Resolve-Path -LiteralPath $ZigExe).ProviderPath
$zigRoot=Split-Path -Parent $zigResolved
if((& $zigResolved version).Trim() -ne '0.14.1'){throw 'This source was validated with Zig 0.14.1.'}
$outputRoot=Join-Path $PSScriptRoot 'build-output'
New-Item -ItemType Directory -Path $outputRoot -Force | Out-Null
$env:ZIG_GLOBAL_CACHE_DIR=Join-Path $outputRoot 'cache-global'
$env:ZIG_LOCAL_CACHE_DIR=Join-Path $outputRoot 'cache-local'
& $zigResolved cc -target x86_64-windows-gnu -shared -O2 -ffreestanding -fno-builtin -fno-stack-protector -nostdlib -isystem (Join-Path $zigRoot 'lib\libc\include\any-windows-any') '-Wl,--entry,DllMain' -s (Join-Path $PSScriptRoot 'hkmw4.c') -o (Join-Path $outputRoot 'hkmw4.dll') -lkernel32
if($LASTEXITCODE -ne 0){throw 'DLL compilation failed.'}
Write-Output ('Rebuilt DLL: '+(Join-Path $outputRoot 'hkmw4.dll'))
Write-Output 'This build does not install files, change the game, or start it.'
