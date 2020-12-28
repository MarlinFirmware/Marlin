#Requires -Version 6.0
function Write-FatalError() {
    Pop-Location
    Write-Error @args
    Exit -1
}

# Check if we can find git
if (!(Get-Command git -ErrorAction SilentlyContinue)) {
    Write-FatalError "Unable to find git command"
}

# Set root directory
$Script:RootDir = Join-Path $PSScriptRoot ".."
Push-Location $Script:RootDir

# Constants
$Script:ExamplesDirectory = Resolve-Path -Path "config"
$Script:FilesPathsToDiff = @("Marlin/Configuration.h", "Marlin/Configuration_adv.h")

function Get-ExampleNames() {
    Return Get-ChildItem -Path $Script:ExamplesDirectory -Name -Directory
}

