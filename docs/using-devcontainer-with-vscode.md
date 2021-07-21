# Installing Marlin (VSCode devcontainer)

## Introduction

A dev container is a pre-built docker environment that is preinstalled with all the dependencies needed to build Marlin with VSCode. The devcontainer not only creates the code compilation environment but it also configures your VSCode with all the extensions needed. The end result is a completely configured VSCode ready to compile and upload Marlin.

The dev container official documentation can be found [here](https://code.visualstudio.com/docs/remote/containers).

## System requirements

**Local:**

-   **Windows:** [Docker Desktop](https://www.docker.com/products/docker-desktop) 2.0+ on Windows 10 Pro/Enterprise. Windows 10 Home (2004+) requires Docker Desktop 2.3+ and the [WSL 2 back-end](https://aka.ms/vscode-remote/containers/docker-wsl2). (Docker Toolbox is not supported. Windows container images are not supported.)
-   **macOS**: [Docker Desktop](https://www.docker.com/products/docker-desktop) 2.0+.
-   **Linux**: [Docker CE/EE](https://docs.docker.com/install/#supported-platforms) 18.06+ and [Docker Compose](https://docs.docker.com/compose/install) 1.21+. (The Ubuntu snap package is not supported.)
-   **VSCode**: https://code.visualstudio.com/docs/setup/setup-overview

## Getting started...

This quick start covers how to set up a dev container for an existing project to use as your full-time development environment using existing source code on your filesystem. Follow these steps:

1.  Start VS Code, run the **Remote-Containers: Open Folder in Container...** command from the Command Palette (F1) or quick actions Status bar item, and select the Marlin project folder.
    
    ![Quick actions Status bar item](https://code.visualstudio.com/assets/docs/remote/common/remote-dev-status-bar.png)
    
2.  The VS Code window will reload and start building the dev container. A progress notification provides status updates. You only have to build a dev container the first time you open it; opening the folder after the first successful build will be much quicker.
    
    ![Dev Container Progress Notification](https://code.visualstudio.com/assets/docs/remote/containers/dev-container-progress.png)
    
3.  After the build completes, VS Code will automatically connect to the container.
    

You can now interact with your project in VS Code just as you could when opening the project locally. From now on, when you open the project folder, VS Code will automatically pick up and reuse your dev container configuration.

While using this approach to [bind mount](https://docs.docker.com/storage/bind-mounts/) the local filesystem into a container is convenient, it does have some performance overhead on Windows and macOS. There are [some techniques](https://code.visualstudio.com/docs/remote/containers-advanced#_improving-container-disk-performance) that you can apply to improve disk performance, or you can [open a repository in a container using a isolated container volume](https://code.visualstudio.com/docs/remote/containers#_quick-start-open-a-git-repository-or-github-pr-in-an-isolated-container-volume) instead.