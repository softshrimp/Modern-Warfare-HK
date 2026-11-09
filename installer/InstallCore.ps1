param(
    [Parameter(Mandatory=$true)][ValidateSet('Install','Uninstall')][string]$Mode,
    [string]$GameRoot='C:\Games\Call of Duty - Modern Warfare 4'
)
$ErrorActionPreference='Stop'
$packageRoot=$PSScriptRoot
$manifest=Get-Content -LiteralPath (Join-Path $packageRoot 'manifest.json') -Raw -Encoding UTF8 | ConvertFrom-Json
$gameResolved=(Resolve-Path -LiteralPath $GameRoot).ProviderPath.TrimEnd('\')
$targetRoot=Join-Path $gameResolved 'sp26'
$receiptPath=Join-Path $targetRoot '_HKMW4-install.json'
$payloadRoot=Join-Path $packageRoot 'payload'
$helperNames=@('hkmw4.dll','hkmw4-sources.bin','hkmw4-changes.bin','hkmw4-mission-review.bin')
$allNames=@($helperNames)+@('bink2w64.dll')

function Get-Hash([string]$Path) {
    $stream=[IO.File]::OpenRead($Path)
    $algorithm=[Security.Cryptography.SHA256]::Create()
    try { [BitConverter]::ToString($algorithm.ComputeHash($stream)).Replace('-','').ToLowerInvariant() }
    finally { $algorithm.Dispose();$stream.Dispose() }
}
function Assert-Hash([string]$Path,[string]$Expected) {
    if(-not(Test-Path -LiteralPath $Path -PathType Leaf)){throw "Missing file: $Path"}
    if((Get-Hash $Path) -cne $Expected){throw "File version/hash mismatch: $Path"}
}
function Assert-Target([string]$Path) {
    $absolute=[IO.Path]::GetFullPath($Path)
    if(-not $absolute.StartsWith($targetRoot+'\',[StringComparison]::OrdinalIgnoreCase)){throw 'Target is outside the selected sp26 folder.'}
}
function Assert-Unlocked([string]$Path) {
    $handle=[IO.File]::Open($Path,[IO.FileMode]::Open,[IO.FileAccess]::ReadWrite,[IO.FileShare]::None)
    $handle.Dispose()
}
function Set-AtomicFile([string]$Source,[string]$Destination,[string]$Expected,[bool]$Replace) {
    Assert-Target $Destination
    $stage=Join-Path $targetRoot ('._HKMW4-'+[Guid]::NewGuid().ToString('N')+'.tmp')
    $atomicBackup=Join-Path $targetRoot ('._HKMW4-atomic-'+[Guid]::NewGuid().ToString('N')+'.tmp')
    Assert-Target $stage
    Assert-Target $atomicBackup
    try {
        Copy-Item -LiteralPath $Source -Destination $stage
        Assert-Hash $stage $Expected
        if($Replace){[IO.File]::Replace($stage,$Destination,$atomicBackup,$true)}else{[IO.File]::Move($stage,$Destination)}
        Assert-Hash $Destination $Expected
    } finally {
        if(Test-Path -LiteralPath $stage){Remove-Item -LiteralPath $stage -Force}
        if((Test-Path -LiteralPath $atomicBackup) -and (Test-Path -LiteralPath $Destination) -and (Get-Hash $Destination) -ceq $Expected){Remove-Item -LiteralPath $atomicBackup -Force}
    }
}
if(-not(Test-Path -LiteralPath (Join-Path $targetRoot 'sp26-cod.exe'))){throw 'Select the game root containing sp26\sp26-cod.exe.'}
foreach($name in $allNames){Assert-Target (Join-Path $targetRoot $name)}

if($Mode -eq 'Install') {
    foreach($name in $allNames){Assert-Hash (Join-Path $payloadRoot $name) $manifest.payload.$name.sha256}
    Assert-Hash (Join-Path $targetRoot 'sp26-cod.exe') $manifest.supported_exe_sha256
    $settings=Join-Path $targetRoot 'steam_settings\configs.user.ini'
    if(-not(Test-Path -LiteralPath $settings)){throw 'The existing language settings file is missing.'}
    $settingsText=[IO.File]::ReadAllText($settings)
    if($settingsText -notmatch '(?m)^\s*language\s*=\s*tchinese\s*$'){throw 'Set the existing game language to tchinese before installing. This installer does not change language settings.'}
    if(Test-Path -LiteralPath $receiptPath) {
        $existing=Get-Content -LiteralPath $receiptPath -Raw -Encoding UTF8 | ConvertFrom-Json
        if($existing.version -ne $manifest.version){throw 'A different HKMW4 installation exists. Restore that installation first.'}
        foreach($name in $allNames){Assert-Hash (Join-Path $targetRoot $name) $manifest.payload.$name.sha256}
        Write-Output 'This exact cumulative patch is already installed. No files changed.'
        exit 0
    }
    Assert-Hash (Join-Path $targetRoot 'bink2w64.dll') $manifest.original_bink_sha256
    foreach($name in $helperNames){if(Test-Path -LiteralPath (Join-Path $targetRoot $name)){throw "Existing file was not overwritten: $name"}}
    Assert-Unlocked (Join-Path $targetRoot 'bink2w64.dll')
    $backupRoot=Join-Path $targetRoot ('_HKMW4-backup\'+[DateTime]::UtcNow.ToString('yyyyMMddTHHmmssfffZ'))
    Assert-Target $backupRoot
    New-Item -ItemType Directory -Path $backupRoot -Force | Out-Null
    $backupBink=Join-Path $backupRoot 'bink2w64.dll'
    Copy-Item -LiteralPath (Join-Path $targetRoot 'bink2w64.dll') -Destination $backupBink
    Assert-Hash $backupBink $manifest.original_bink_sha256
    $installed=New-Object 'System.Collections.Generic.List[string]'
    try {
        foreach($name in $helperNames) {
            Set-AtomicFile (Join-Path $payloadRoot $name) (Join-Path $targetRoot $name) $manifest.payload.$name.sha256 $false
            $installed.Add($name)
            Assert-Hash (Join-Path $targetRoot $name) $manifest.payload.$name.sha256
        }
        # Install the loader only after all of its dependencies have passed readback.
        Set-AtomicFile (Join-Path $payloadRoot 'bink2w64.dll') (Join-Path $targetRoot 'bink2w64.dll') $manifest.payload.'bink2w64.dll'.sha256 $true
        $installed.Add('bink2w64.dll')
        Assert-Hash (Join-Path $targetRoot 'bink2w64.dll') $manifest.payload.'bink2w64.dll'.sha256
        $receiptText=[pscustomobject]@{version=$manifest.version;installedUtc=[DateTime]::UtcNow.ToString('o');backupRoot=$backupRoot;originalBinkSHA256=$manifest.original_bink_sha256;settingsSHA256=(Get-Hash $settings);files=$allNames;payload=$manifest.payload} | ConvertTo-Json -Depth 8
        $receiptStage=Join-Path $backupRoot 'install-receipt.stage.json'
        [IO.File]::WriteAllText($receiptStage,$receiptText,[Text.UTF8Encoding]::new($false))
        [IO.File]::Move($receiptStage,$receiptPath)
    } catch {
        $failure=$_
        # Restore only files whose identity belongs to this transaction.
        $liveBink=Join-Path $targetRoot 'bink2w64.dll'
        if((Test-Path -LiteralPath $liveBink) -and (Get-Hash $liveBink) -eq $manifest.payload.'bink2w64.dll'.sha256){Set-AtomicFile $backupBink $liveBink $manifest.original_bink_sha256 $true}
        foreach($name in $helperNames) {
            $live=Join-Path $targetRoot $name
            if((Test-Path -LiteralPath $live) -and (Get-Hash $live) -eq $manifest.payload.$name.sha256){Remove-Item -LiteralPath $live -Force}
        }
        throw $failure
    }
    Write-Output 'Cumulative Hong Kong patch 0.5.1 installed. The game has not been started.'
    Write-Output 'Start the game later using your existing launcher.'
    Write-Output 'Reviewed campaign and shared text is enabled. Typography is omitted by user agreement. See the included coverage limits.'
    Write-Output ('Original Bink backup: '+$backupBink)
} else {
    if(-not(Test-Path -LiteralPath $receiptPath)){throw 'No HKMW4 installation receipt exists. No files changed.'}
    $receipt=Get-Content -LiteralPath $receiptPath -Raw -Encoding UTF8 | ConvertFrom-Json
    $expectedFiles=@('hkmw4.dll','hkmw4-sources.bin','hkmw4-changes.bin','hkmw4-mission-review.bin','bink2w64.dll')
    if(@(Compare-Object $expectedFiles @($receipt.files)).Count -ne 0){throw 'Unexpected installation receipt. No files changed.'}
    $backupRoot=[IO.Path]::GetFullPath([string]$receipt.backupRoot)
    Assert-Target $backupRoot
    $backupBink=Join-Path $backupRoot 'bink2w64.dll'
    Assert-Hash $backupBink $receipt.originalBinkSHA256
    foreach($name in $helperNames){$path=Join-Path $targetRoot $name;if(Test-Path -LiteralPath $path){Assert-Hash $path $receipt.payload.$name.sha256;Assert-Unlocked $path}}
    $liveBink=Join-Path $targetRoot 'bink2w64.dll'
    $currentBinkHash=Get-Hash $liveBink
    if($currentBinkHash -cne $receipt.originalBinkSHA256 -and $currentBinkHash -cne $receipt.payload.'bink2w64.dll'.sha256){throw 'Bink changed after installation. No files changed.'}
    Assert-Unlocked $liveBink
    # Restore the loader first, so the optional DLLs are no longer required.
    if($currentBinkHash -cne $receipt.originalBinkSHA256){Set-AtomicFile $backupBink $liveBink $receipt.originalBinkSHA256 $true}
    Assert-Hash (Join-Path $targetRoot 'bink2w64.dll') $receipt.originalBinkSHA256
    foreach($name in $helperNames){$path=Join-Path $targetRoot $name;if(Test-Path -LiteralPath $path){Remove-Item -LiteralPath $path -Force}}
    $receipt | Add-Member -NotePropertyName uninstalledUtc -NotePropertyValue ([DateTime]::UtcNow.ToString('o')) -Force
    $receipt | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath (Join-Path $backupRoot 'uninstalled.json') -Encoding UTF8
    Remove-Item -LiteralPath $receiptPath -Force
    Write-Output 'Original Bink restored. Capture logs and the backup are retained.'
}
