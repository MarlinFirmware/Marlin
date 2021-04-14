<#

.SYNOPSIS
Generates or updates a configuration example file

.PARAMETER  Name
Name of the configuration example. Must be a valid path name.

.PARAMETER  InteractiveResolve
Interactively resolve merge conflicts in the configuration files.

.PARAMETER  GitRevision
The first revision to compare against. For instance: after a merge, 
pass in the first revision of the current repository before the merge.

#>
[CmdletBinding()]
Param(
    [Parameter(Mandatory=$true)]
    $Name,

    [Switch]
    $InteractiveResolve, 

    [String]
    $GitRevision
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

    # Generate diff of Configuration changes
    Remove-Item -Path $ExampleDiffFilePath -Verbose -ErrorAction SilentlyContinue
    
    if ($GitRevision) {
        $RevisionArg = "$($GitRevision)..HEAD"
        git diff --unified=5 --output=$ExampleDiffFilePath $RevisionArg -- $FilePath
    } else {
        git diff --unified=5 --output=$ExampleDiffFilePath $FilePath
    }

    if ($LASTEXITCODE -ne 0) {
        Write-FatalError "Unable to diff file $FilePath"
    }

    $TheDiff = Get-Content -Path $ExampleDiffFilePath -Raw
    $HasDiff = [String]::IsNullOrWhiteSpace($TheDiff) -eq $false

    # If the example exists then we must apply the diff, otherwise we can copy the config
    if (!(Test-Path $ExampleFilePath)) {
        Write-Host "... copy to $ExampleFilePath"
        Copy-Item $FilePath $ExampleFilePath
    } else {
        if ($HasDiff) {
            Write-Host "... diff was written to $ExampleDiffFilePath"
            
            # Tinker with the file paths in the diff
            $ReplacementPath = $ExampleFilePath.Replace($(Get-Location), "").Replace('\','/').TrimStart("/")
            Write-Host "   ... replacing $FilePath with $ReplacementPath"
            $TheDiff = $TheDiff.Replace($FilePath, $ReplacementPath)
            $TheDiff | Out-File -Path $ExampleDiffFilePath -Encoding UTF8NoBom

            Write-Host "Updating example..."
            Write-Host "... applying $ExampleDiffFilePath"
            git apply --verbose --ignore-space-change --ignore-whitespace --3way $ExampleDiffFilePath

            if ($InteractiveResolve -and $LASTEXITCODE -ne 0) {
                Write-Warning "Failed to apply diff file for $FilePath"
                Write-Host "... opening merge tool"
                
                & "git" @("mergetool")
            }

            if ($LASTEXITCODE -ne 0) {
                Write-FatalError "Failed to apply diff file for $FilePath"
            }
        } else {
            Write-Warning "No changes in $FilePath"
        }

        Remove-Item -Path $ExampleDiffFilePath -Force -ErrorAction SilentlyContinue -Verbose
    }

    Write-Host ""
}

Write-Host "Done! Don't forget to update scripts/README.md."

Pop-Location

Exit 0