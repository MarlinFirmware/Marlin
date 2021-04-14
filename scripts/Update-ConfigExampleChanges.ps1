<#

.SYNOPSIS
Interactively merge upstream changes, updating configuration examples, resolving merge conflicts.

#>
[CmdletBinding()]
Param()

# Include common scripts
#Requires -Version 6.0
. $PSScriptRoot/Common.ps1

## Run each example
Write-Host "Collecting example configurations..."
$Configs = Get-ExampleNames
$ConfigFiles = @("Configuration.h", "Configuration_adv.h")

foreach ($ConfigName in $Configs) {
    if ($Pause) {
        Read-Host -Prompt "Pausing for $ConfigName - press any key to continue"
    }

    foreach ($ConfigFile in $ConfigFiles) {
        $BaseFile = "Marlin/$ConfigFile"
        $CurrentFile = "config/$ConfigName/$ConfigFile"
        $TmpOtherFile = [System.IO.Path]::GetTempFileName()

        Copy-Item $CurrentFile $TmpOtherFile
        & TortoiseGitMerge.exe $("/theirsname:$ConfigName-$ConfigFile") $("/basename:Default-$ConfigFile") -L $("/mergedname:$ConfigFile") $("/base:$BaseFile") $("/theirs:$TmpOtherFile") $("/merged:$CurrentFile")
    }
}