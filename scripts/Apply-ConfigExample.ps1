<#

.SYNOPSIS
Applies a configuration example.

.PARAMETER  Name
Name of the configuration example. Must be an example config in the configs directory.

#>
[CmdletBinding()]
Param(
    [Parameter(Mandatory=$true)]
    $Name
)

# Include common scripts
#Requires -Version 6.0
. $PSScriptRoot/Common.ps1

# Set-up
$ExampleDir = Join-Path $ExamplesDirectory $Name

if (!(Test-Path $ExampleDir)) {
    Write-FatalError "Directory $ExampleDir does not exist"
}

# Apply configuration diff files
foreach ($FilePath in $FilesPathsToDiff) {
    Write-Host "Processing $FilePath"

    # Determine paths
    $FileName = Split-Path -Leaf -Path $FilePath

    $ExampleFilePath = Join-Path $ExampleDir $FileName
    $ExampleDiffFilePath = $ExampleFilePath + ".diff"

    # Apply diff if exists
    if (Test-Path $ExampleDiffFilePath) {
        Write-Host "... applying $ExampleDiffFilePath"
        git apply --verbose --ignore-space-change --ignore-whitespace --3way $ExampleDiffFilePath

        if ($LASTEXITCODE -ne 0) {
            Write-FatalError "Failed to apply diff file for $FilePath"
        }

        git reset HEAD $FilePath

        if ($LASTEXITCODE -ne 0) {
            Write-FatalError "Failed to unstage file $FilePath"
        }
    } else {
        Write-Host "... keeping $FilePath as-is because no .diff file exists"
    }
}

Pop-Location
