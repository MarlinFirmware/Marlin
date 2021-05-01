<#

.SYNOPSIS
Runs a Platform.IO build and optionally deploy it for immediate flashing to a local USB drive.

PlatformIO executable must be in PATH (will be when running in the VSCode console)
The Show-Menu PowerShell module must be installed (`Install-Package Show-Menu`)

Add this to your PSProfile:

function Invoke-PioBuild($ConfigName, $Drive = "Q:", [Switch]$NoReset, [Switch] $ForceClean, [Switch] $NoClean) {
    $GitDir = $(git rev-parse --show-toplevel)
    $ScriptFile = "$GitDir/scripts/Invoke-PioBuild.ps1"
    & $ScriptFile -ConfigName $ConfigName -Drive $Drive -NoReset:$NoReset -ForceClean:$ForceClean -NoClean:$NoClean
}

.PARAMETER  ConfigName
The configuration to run. The configuration is assumed to live in the 'configs' directory. 
If the parameter is not provided, it is interactively asked.

This parameter is remembered, so if you execute the script again in the same powershell session the
option is preselected in the menu.

.PARAMETER  Drive
Drive letter to deploy to

.PARAMETER  NoReset
Do not reset the Configuration files copied to the Marlin directory.

.PARAMETER  ForceClean
Force to clean the .pio build and immediate files

.PARAMETER  NoClean
Do not clean the .pio build and immediate files

#>

[CmdletBinding()]
Param(
    [string]$ConfigName, 
    
    [Parameter(Mandatory=$true)]
    [string]$Drive, 
    
    [Switch]$NoReset, 
    
    [Switch]$ForceClean, 
    
    [Switch]$NoClean
)

# Include common scripts
#Requires -Version 6.0
. $PSScriptRoot/Common.ps1

if ($Drive -inotlike "*:") {
    $Drive += ":"
}

Set-Location $(Join-Path $PSScriptRoot ..)

$LastBuildType = $Global:LastBuildType;

if (!$ConfigName) {
    $Configs = $(Get-ChildItem ./config -Name)

    if ($LastBuildType) {
        $Configs = @($LastBuildType, $(Get-MenuSeparator)) + $Configs
    }

    $ConfigName = Show-Menu -MenuItems $Configs
    if (!$ConfigName) {
        Write-Error "Select config"
        return;
    }
}

# Determine clean mode
$ShouldClean = $LastBuildType -ne $ConfigName
$Global:LastBuildType = $ConfigName
if ($ForceClean) { $ShouldClean = $true }
if ($NoClean) { $ShouldClean = $false }

$ConfigDir = Join-Path -Path "config" -ChildPath $ConfigName
if (!(Resolve-Path $ConfigDir)) {
    Write-Error "Invalid path"
    return;
}

# Copy config
$PioEnv = Get-Content $(Join-Path $ConfigDir "platformio-environment.txt") -Raw
Copy-Item -Path $(Join-Path $ConfigDir "*.h") -Destination "Marlin" -Force -Verbose

# Clean
Remove-Item .pio/build/*/*.bin -Rec -Force -Verbose
if ($ShouldClean) {
    pio run -t clean
    pio run -t clean -e $PioEnv
}

# Copy clean
Remove-Item $Drive\*.cur -Force -Verbose -ErrorAction SilentlyContinue   
Remove-Item $Drive\*.bin -Force -Verbose -ErrorAction SilentlyContinue   

# Build
pio run -e $PioEnv

if ($LASTEXITCODE -eq 0) {
    Copy-Item .pio\build\$PioEnv\*.bin $Drive\ -Verbose       
}

if (!$NoReset) {
    git checkout Marlin/Configuration.h
    git checkout Marlin/Configuration_adv.h
}

Write-Host "Completed build $ConfigName to $Drive" -ForegroundColor Green