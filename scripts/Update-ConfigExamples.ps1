<#

.SYNOPSIS
Refreshes the configuration examples based on the current base configuration changes

#>
[CmdletBinding()]
Param(
    [Switch]
    $Pause,

    [Switch]
    $InteractiveResolve
)

# Include common scripts
#Requires -Version 6.0
. $PSScriptRoot/Common.ps1

## Run each example
Write-Host "Collecting example configurations..."
$Configs = Get-ExampleNames

foreach ($ConfigName in $Configs) {
    if ($Pause) {
        Read-Host -Prompt "Pausing for $ConfigName - press any key to continue"
    }

    .\scripts\Generate-ConfigExample.ps1 -Name $ConfigName -InteractiveResolve:$InteractiveResolve
    
    if ($LASTEXITCODE -ne 0) {
        Write-FatalError "Unable to generate configuration example for $ConfigName"
    }
}