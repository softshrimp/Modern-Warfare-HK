param(
    [Parameter(Mandatory=$true)][ValidateSet('Install','Uninstall')][string]$Mode,
    [Parameter(Mandatory=$true)][string]$GameRoot
)
$ErrorActionPreference='Stop'
$packageRoot=$PSScriptRoot
$manifest=Get-Content -LiteralPath (Join-Path $packageRoot 'manifest.json') -Raw -Encoding UTF8 | ConvertFrom-Json
function Read-Hash([string]$Path) {
    $stream=[IO.File]::OpenRead($Path)
    $algorithm=[Security.Cryptography.SHA256]::Create()
    try {[BitConverter]::ToString($algorithm.ComputeHash($stream)).Replace('-','').ToLowerInvariant()}
    finally {$algorithm.Dispose();$stream.Dispose()}
}
if($Mode -eq 'Uninstall') {
    & (Join-Path $packageRoot 'InstallCore.ps1') -Mode Uninstall -GameRoot $GameRoot
    return
}
# Reconstruct the loader from the user's matching local file. No game DLL is distributed.
$resolvedGame=(Resolve-Path -LiteralPath $GameRoot).ProviderPath
$loaderPath=Join-Path $resolvedGame 'sp26\bink2w64.dll'
if(-not(Test-Path -LiteralPath $loaderPath -PathType Leaf)){throw 'The selected folder does not contain sp26\bink2w64.dll.'}
$currentHash=Read-Hash $loaderPath
$expectedPatched=$manifest.payload.'bink2w64.dll'.sha256
if($currentHash -cne $manifest.original_bink_sha256 -and $currentHash -cne $expectedPatched){throw 'Unsupported Bink version. No files changed.'}
$stageRoot=Join-Path ([IO.Path]::GetTempPath()) ('HKMW4-stage-'+[Guid]::NewGuid().ToString('N'))
[IO.Directory]::CreateDirectory($stageRoot) | Out-Null
try {
    $stagePayload=Join-Path $stageRoot 'payload'
    [IO.Directory]::CreateDirectory($stagePayload) | Out-Null
    Copy-Item -LiteralPath (Join-Path $packageRoot 'InstallCore.ps1') -Destination (Join-Path $stageRoot 'manage.ps1')
    Copy-Item -LiteralPath (Join-Path $packageRoot 'manifest.json') -Destination (Join-Path $stageRoot 'manifest.json')
    foreach($name in @('hkmw4.dll','hkmw4-sources.bin','hkmw4-changes.bin','hkmw4-mission-review.bin')) {
        $source=Join-Path $packageRoot ('payload\'+$name)
        if((Read-Hash $source) -cne $manifest.payload.$name.sha256){throw ('Damaged patch payload: '+$name)}
        Copy-Item -LiteralPath $source -Destination (Join-Path $stagePayload $name)
    }
    $bytes=[IO.File]::ReadAllBytes($loaderPath)
    if($currentHash -ceq $manifest.original_bink_sha256) {
        $delta=Get-Content -LiteralPath (Join-Path $packageRoot 'bink-delta.json') -Raw -Encoding UTF8 | ConvertFrom-Json
        if($bytes.Length -ne $delta.original_length){throw 'Bink length mismatch.'}
        foreach($change in $delta.changes) {
            $offset=[int]$change.offset
            if($offset -lt 0 -or $offset -ge $bytes.Length -or $bytes[$offset] -ne [byte]$change.before){throw 'Bink patch precondition failed.'}
            $bytes[$offset]=[byte]$change.after
        }
    }
    $generatedLoader=Join-Path $stagePayload 'bink2w64.dll'
    [IO.File]::WriteAllBytes($generatedLoader,$bytes)
    if((Read-Hash $generatedLoader) -cne $expectedPatched){throw 'Generated loader hash mismatch. No files changed.'}
    & (Join-Path $stageRoot 'manage.ps1') -Mode Install -GameRoot $GameRoot
} finally {
    # Only this invocation's fixed-name temporary folder may be removed recursively.
    $absolute=[IO.Path]::GetFullPath($stageRoot)
    $tempBase=[IO.Path]::GetFullPath([IO.Path]::GetTempPath()).TrimEnd('\')+'\'
    if($absolute.StartsWith($tempBase,[StringComparison]::OrdinalIgnoreCase) -and
       [IO.Path]::GetFileName($absolute) -match '^HKMW4-stage-[0-9a-f]{32}$') {
        if(Test-Path -LiteralPath $absolute){Remove-Item -LiteralPath $absolute -Recurse -Force}
    }
}
