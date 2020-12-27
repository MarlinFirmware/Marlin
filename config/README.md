# CR-6 community configuration examples

You can find various example configurations in the sub-directories.

The following examples are available: 

- [Creality CR-6 SE with BTT SKR CR6 with BTT TFT](./btt-skr-cr6-with-btt-tft)
- [Creality CR-6 SE with BTT SKR CR6 with Creality stock TFT](./btt-skr-cr6-with-btt-tft)
- [Creality CR-6 SE with v4.5.2 motherboard](./cr6-se-v4.5.2-mb)
- [Creality CR-6 SE with v4.5.3 motherboard](./cr6-se-v4.5.3-mb)
- [Creality CR-6 MAX](./cr6-max)

There are some helper scripts. You need Powershell Core (`pwsh`) to run them.

## Generating configuration examples
To generate or update a configuration example, do the modifications and then run:

    scripts/Generate-ConfigExample.ps1 -Name MyExampleName

## Applying configuration examples
To apply a configuration example, do the modifications, and then run:

    scripts/Apply-ConfigExample.ps1 -Name [folder name here]


You can also copy the `Configuration*` files from the example directory.

## Maintainance

To refresh configuration examples (repository maintainers):

    scripts/Refresh-ConfigExamples.ps1

This script essentially runs a conbination of Generate and Apply.

## Run builds

To run builds for all examples (repository maintainers):

    scripts/Run-ExampleConfigBuilds.ps1 -TouchscreenRepositoryPath [path to cr6 touch screen repository]

This script is meant to be executed in the VSCode console and in preperation for firmware release.

