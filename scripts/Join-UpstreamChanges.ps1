<#

.SYNOPSIS
Interactively merge upstream changes, updating configuration examples, resolving merge conflicts.

#>
[CmdletBinding()]
Param()

# Include common scripts
#Requires -Version 6.0
. $PSScriptRoot/Common.ps1

function Write-Info {
    Write-Host $args -ForegroundColor Cyan
}

# Look up current revision for config file merge
Write-Info "Getting current revision"
[string] $CurrentRevision = git rev-parse HEAD

if ($LASTEXITCODE -ne 0) {
    Write-FatalError "Error getting current revision"
}

$CurrentRevision = $CurrentRevision.Trim()
Write-Info "... Current revision is: $CurrentRevision"

# Fetch and merge
Write-Info "Fetching upstream..."
git fetch upstream

if ($LASTEXITCODE -ne 0) {
    Write-FatalError "Error getting current revision"
}

[string] $CurrentBranch = $(git branch --show-current)
$CurrentBranch = $CurrentBranch.Trim()
$TargetBranch = if ($CurrentBranch -eq "extui") { "2.0.x" } else { "bugfix-2.0.x "}

Write-Info "Merging upstream/$TargetBranch..."
git merge "upstream/$TargetBranch" --no-ff

if ($LASTEXITCODE -ne 0) {
    Write-Warning "There appears to be merge conflicts. Opening merge tool."

    & "git" @("mergetool")

    if ($LASTEXITCODE -ne 0) {
        Write-FatalError "Failed to run merge tool"
    }

    Write-Info "Running commit..."
    git commit

    if ($LASTEXITCODE -ne 0) {
        Write-FatalError "Failed to commit"
    }
}

Write-Info "Update config examples based on previous revision $CurrentRevision..."
pwsh -NoProfile -File "$PSScriptRoot/Update-ConfigExamples.ps1" -InteractiveResolve -GitRevision $CurrentRevision

if ($LASTEXITCODE -ne 0) {
    Write-FatalError "Failed to update config examples"
}

Write-Info "Running commit..."
git commit -a

Write-Info "Success"
Exit 0