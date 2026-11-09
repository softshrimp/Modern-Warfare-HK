param()
$ErrorActionPreference='Stop'
Add-Type -AssemblyName System.IO.Compression
Add-Type -AssemblyName System.IO.Compression.FileSystem
$repo=$PSScriptRoot
$version=(Get-Content -LiteralPath (Join-Path $repo 'data\manifest.json') -Raw -Encoding UTF8 | ConvertFrom-Json).version
$dist=Join-Path $repo 'dist'
$work=Join-Path $repo ('.build\'+[Guid]::NewGuid().ToString('N'))
$package=Join-Path $work 'package'
New-Item -ItemType Directory -Path (Join-Path $package 'payload'),$dist -Force | Out-Null
foreach($name in @('hkmw4.dll','hkmw4-sources.bin','hkmw4-changes.bin','hkmw4-mission-review.bin')) {
    Copy-Item -LiteralPath (Join-Path $repo ('data\payload\'+$name)) -Destination (Join-Path $package ('payload\'+$name))
}
foreach($name in @('manifest.json','bink-delta.json')) {
    Copy-Item -LiteralPath (Join-Path $repo ('data\'+$name)) -Destination (Join-Path $package $name)
}
foreach($name in @('InstallPatch.ps1','InstallCore.ps1')) {
    Copy-Item -LiteralPath (Join-Path $repo ('installer\'+$name)) -Destination (Join-Path $package $name)
}
Copy-Item -LiteralPath (Join-Path $repo 'README.md') -Destination (Join-Path $package 'README.md')
$entrypoints=@{
    'Install.cmd'='@echo off' + "`r`n" + 'powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0InstallPatch.ps1" -Mode Install -GameRoot "C:\Games\Call of Duty - Modern Warfare 4"' + "`r`n" + 'pause' + "`r`n"
    'Uninstall.cmd'='@echo off' + "`r`n" + 'powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%~dp0InstallPatch.ps1" -Mode Uninstall -GameRoot "C:\Games\Call of Duty - Modern Warfare 4"' + "`r`n" + 'pause' + "`r`n"
}
foreach($name in $entrypoints.Keys){[IO.File]::WriteAllText((Join-Path $package $name),$entrypoints[$name],[Text.Encoding]::ASCII)}
$sums=New-Object 'System.Collections.Generic.List[string]'
$files=Get-ChildItem -LiteralPath $package -Recurse -File | Sort-Object FullName
foreach($file in $files) {
    $relative=$file.FullName.Substring($package.Length+1).Replace('\','/')
    $sums.Add((Get-FileHash -LiteralPath $file.FullName -Algorithm SHA256).Hash.ToLowerInvariant()+'  '+$relative)
}
[IO.File]::WriteAllLines((Join-Path $package 'SHA256SUMS.txt'),$sums,[Text.UTF8Encoding]::new($false))
$zipPath=Join-Path $dist ('HKMW4-'+$version+'-portable.zip')
$zipStream=[IO.File]::Open($zipPath,[IO.FileMode]::Create)
$zip=New-Object IO.Compression.ZipArchive($zipStream,[IO.Compression.ZipArchiveMode]::Create)
try {
    foreach($file in (Get-ChildItem -LiteralPath $package -Recurse -File | Sort-Object FullName)) {
        $relative=$file.FullName.Substring($package.Length+1).Replace('\','/')
        $entry=$zip.CreateEntry($relative,[IO.Compression.CompressionLevel]::Optimal)
        $entry.LastWriteTime=[DateTimeOffset]::new(2000,1,1,0,0,0,[TimeSpan]::Zero)
        $input=[IO.File]::OpenRead($file.FullName);$output=$entry.Open()
        try{$input.CopyTo($output)}finally{$input.Dispose();$output.Dispose()}
    }
} finally {$zip.Dispose();$zipStream.Dispose()}
$zipHash=(Get-FileHash -LiteralPath $zipPath -Algorithm SHA256).Hash.ToLowerInvariant()
$source=[IO.File]::ReadAllText((Join-Path $repo 'installer\Setup.cs')).Replace('@PACKAGE_SHA256@',$zipHash)
$generatedSource=Join-Path $work 'Setup.cs'
[IO.File]::WriteAllText($generatedSource,$source,[Text.UTF8Encoding]::new($false))
$compiler=Join-Path $env:WINDIR 'Microsoft.NET\Framework64\v4.0.30319\csc.exe'
if(-not(Test-Path -LiteralPath $compiler)){throw 'The .NET Framework x64 C# compiler was not found.'}
$exe=Join-Path $dist ('HKMW4-Setup-'+$version+'.exe')
& $compiler /nologo /target:winexe /platform:x64 /optimize+ /codepage:65001 "/out:$exe" "/win32manifest:$(Join-Path $repo 'installer\app.manifest')" "/resource:$zipPath,HKMW4.Package" /reference:System.Windows.Forms.dll /reference:System.Drawing.dll /reference:System.IO.Compression.dll /reference:System.IO.Compression.FileSystem.dll $generatedSource
if($LASTEXITCODE -ne 0){throw 'Installer compilation failed.'}
$process=Start-Process -FilePath $exe -ArgumentList '--verify-payload' -WindowStyle Hidden -Wait -PassThru
if($process.ExitCode -ne 0){throw 'Embedded payload verification failed.'}
$releaseSums=@($exe,$zipPath) | ForEach-Object{(Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()+'  '+[IO.Path]::GetFileName($_)}
[IO.File]::WriteAllLines((Join-Path $dist 'SHA256SUMS.txt'),$releaseSums,[Text.Encoding]::ASCII)
Write-Output ('Built and verified: '+$exe)
