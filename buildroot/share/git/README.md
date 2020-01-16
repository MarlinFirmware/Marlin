## Marlin Github Helper Scripts

### Introduction

A Pull Request is often just the start of a longer process of patching and refining the code until it's ready to merge. In that process it's common to accumulate a lot of commits, some of which are non-functional. Before merging any PR, excess commits need to be "squashed" and sometimes rearranged or reworked to produce a well-packaged set of changes and keep the commit history relatively clean.

In addition, while a PR is being worked on other commits may be merged, leading to conflicts that need resolution. For this reason, it's a best practice to periodically refresh the PR so the working copy closely reflects the final merge into upstream `MarlinFirmware`.

#### Merge vs Rebase

If you plan to create PRs and work on them after submission I recommend not using Github Desktop to sync and merge. Use the command line instead. Github Desktop provides a "merge" option, but I've found that "`git rebase`" is much cleaner and easier to manage. Merge applies new work _after_ your commits, which buries them deeper in the commit history and makes it hard to bring them together as a final packaged unit. Rebase helpfully moves your commits to the tip of the branch, ensuring that your commits are adapted to the current code. This makes it easier to keep revising the commits in-place.

### The Scripts

The following scripts can be used on any system with a GNU environment to speed up the process of working with Marlin branches and submitting changes to the project.

#### Remotes

File|Description
----|-----------
mfadd&nbsp;[user]|Add and Fetch Remote - Add and fetch another user's Marlin fork. Optionally, check out one of their branches.
mfinit|Init Working Copy - Create a remote named '`upstream`' (for use by the other scripts) pointing to the '`MarlinFirmware`' fork. This only needs to be used once. Newer versions of Github Desktop may create `upstream` on your behalf.

#### Branches

File|Description
----|-----------
mfnew&nbsp;[branch]|New Branch - Creates a new branch based on `upstream/[PR-target]`. All new work should start with this command.
mffp|Fast Push - Push the HEAD or a commit ID to `upstream` immediately. Requires privileged access to the MarlinFirmware repo.
firstpush|Push the current branch to 'origin' -your fork on Github- and set it to track '`origin`'. The branch needs to reside on Github before you can use it to make a PR.

#### Making / Amending PRs

File|Description
----|-----------
mfpr|Pull Request - Open the Compare / Pull Request page on Github for the current branch.
mfrb|Do a `git rebase` then `git rebase -i` of the current branch onto `upstream/[PR-target]`. Use this to edit your commits anytime.
mfqp|Quick Patch - Commit all current changes as "patch", then do `mfrb`, followed by `git push -f` if no conflicts need resolution.

#### Documentation

File|Description
----|-----------
mfdoc|Build the documentation with Jekyll and preview it locally.
mfpub|Build and publish the documentation to marlinfw.org.

#### Utilities

File|Description
----|-----------
ghtp -[h/s]|Set the protocol to use for all remotes. -h for HTTPS, -s for SSL.
ghpc [-f]|Push current branch to 'origin' or to the remote indicated by the error.
mfinfo|This utility script is used by the other scripts to get:<br/>- The upstream project ('`MarlinFirmware`')<br/>- the '`origin`' project (i.e., your Github username),<br/>- the repository name ('`Marlin`'),<br/>- the PR target branch ('`bugfix-1.1.x`'), and<br/>- the current branch (or the first command-line argument).<br/><br/>By itself, `mfinfo` simply prints these values to the console.
mfclean&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;|Prune your merged and remotely-deleted branches.

---

### Examples

For a demonstration of these scripts see the video [Marlin Live - May 9 2019](https://youtu.be/rwT4G0uVTIY). There is also an old write-up at [#3193](https://github.com/MarlinFirmware/Marlin/issues/3193).
