<#

.SYNOPSIS
Generates a configuration example file

.PARAMETER  Name
Name of the configuration example. Must be a valid path name.

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

Write-Host "Going to output examples to $ExampleDir"

if (!(Test-Path $ExampleDir)) {
    try {
        New-Item -Path $ExampleDir -ItemType Directory | Out-Null
    } catch {
        Write-Error "Unable to create directory $ExampleDir"
        Write-FatalError $_
    }
}

# Now run some diffs
class ConfigFile {
    [string] $Path;
    [string] $FileName;
    [string] $Diff;
    [string] $Contents;
}

foreach ($FilePath in $FilesPathsToDiff) {
    Write-Host "Processing $FilePath"
    if (!(Test-PAth $FilePath)) {
        Write-FatalError "Unable to find file $FilePath"
    }

    $FileName = Split-Path -Leaf -Path $FilePath

    $ExampleFilePath = Join-Path $ExampleDir $FileName
    $ExampleDiffFilePath = $ExampleFilePath + ".diff"

    git diff --unified=5 --output=$ExampleDiffFilePath $FilePath

    if ($LASTEXITCODE -ne 0) {
        Write-FatalError "Unable to diff file $FilePath"
    }

    $HasDiff = [String]::IsNullOrWhiteSpace($(Get-Content -Path $ExampleDiffFilePath -Raw)) -eq $false

    # Copy entire file
    Write-Host "... copy to $ExampleFilePath"
    Copy-Item -Path $FilePath -Destination $ExampleFilePath

    # Write diff
    if (!$HasDiff) {
        Write-Warning "No changes in $FilePath"
        Remove-Item -Path $ExampleDiffFilePath -Force -ErrorAction SilentlyContinue -Verbose
    } else {
        Write-Host "... diff was written to $ExampleDiffFilePath"
    }

    Write-Host "... resetting $FilePath to HEAD"
    git checkout HEAD -- $FilePath

    Write-Host ""
}

Write-Host "Done! Don't forget to update scripts/README.md."

Pop-Location

Exit 0