name: 🪲 Report a bug
description: Create a bug report to help improve Marlin Firmware
title: "[BUG] (bug summary)"
issue_body: true
body:
  - type: markdown
    attributes:
      value: >
        Do you want to ask a question? Are you looking for support? Please use one of the [support links](https://github.com/MarlinFirmware/Marlin/issues/new/choose).

  - type: markdown
    attributes:
      value: |
        **Thank you for reporting a bug in Marlin Firmware!**

        ## Before Reporting a Bug

        - Read and understand Marlin's [Code of Conduct](https://github.com/MarlinFirmware/Marlin/blob/master/.github/code_of_conduct.md). You are expected to comply with it, including treating everyone with respect.

        - Test with the [`bugfix-2.0.x` branch](https://github.com/MarlinFirmware/Marlin/archive/bugfix-2.0.x.zip) to see whether the issue still exists.

        ## Instructions

        Please follow the instructions below. Failure to do so may result in your issue being closed. See [Contributing to Marlin](https://github.com/MarlinFirmware/Marlin/blob/2.0.x/.github/contributing.md) for additional guidelines.

        1. Provide a good title starting with [BUG].
        2. Fill out all sections of this bug report form.
        3. Always attach configuration files so we can build and test your setup.

  - type: dropdown
    attributes:
      label: Did you test the latest `bugfix-2.0.x` code?
      description: >-
        Always try the latest code to make sure the issue you are reporting is not already fixed. To download
        the latest code just [click this link](https://github.com/MarlinFirmware/Marlin/archive/bugfix-2.0.x.zip).
      options:
        - Yes, and the problem still exists.
        - No, but I will test it now!
    validations:
      required: true

  - type: markdown
    attributes:
      value: |
        # Bug Details

  - type: textarea
    attributes:
      label: Bug Description
      description: >-
        Describe the bug in this section. Tell us what you were trying to do and what
        happened that you did not expect. Provide a clear and concise description of the
        problem and include as many details as possible.
      placeholder: |
        Marlin doesn't work.
    validations:
      required: true

  - type: input
    attributes:
      label: Bug Timeline
      description: Is this a new bug or an old issue? When did it first start?

  - type: textarea
    attributes:
      label: Expected behavior
      description: >-
        What did you expect to happen?
      placeholder: I expected it to move left.

  - type: textarea
    attributes:
      label: Actual behavior
      description: What actually happened instead?
      placeholder: It moved right instead of left.

  - type: textarea
    attributes:
      label: Steps to Reproduce
      description: >-
        Please describe the steps needed to reproduce the issue.
      placeholder: |
        1. [First Step] ...
        2. [Second Step] ...
        3. [and so on] ...

  - type: markdown
    attributes:
      value: |
        # Your Setup

  - type: input
    attributes:
      label: Version of Marlin Firmware
      description: "See the About Menu on the LCD or the output of `M115`. NOTE: For older releases we only patch critical bugs."
    validations:
      required: true

  - type: input
    attributes:
      label: Printer model
      description: Creality Ender 3, Prusa mini, or Kossel Delta?

  - type: input
    attributes:
      label: Electronics
      description: Stock electronics, upgrade board, or something else?

  - type: input
    attributes:
      label: Add-ons
      description: Please list any hardware add-ons that could be involved.

  - type: dropdown
    attributes:
      label: Your Slicer
      description: Do you use Slic3r, Prusa Slicer, Simplify3D, IdeaMaker...?
      options:
        - Slic3r
        - Simplify3D
        - Prusa Slicer
        - IdeaMaker
        - Cura
        - Other (explain below)

  - type: dropdown
    attributes:
      label: Host Software
      description: Do you use OctoPrint, Repetier Host, Pronterface...?
      options:
        - SD Card (headless)
        - Repetier Host
        - OctoPrint
        - Pronterface
        - Cura
        - Same as my slicer
        - Other (explain below)

  - type: markdown
    attributes:
      value: >-
        ## Other things to include

        Please also be sure to include these items to help with troubleshooting:

          * **A ZIP file** containing your `Configuration.h` and `Configuration_adv.h`.
            (Please don't paste lengthy configuration text here.)
          * **Log output** from the host. (`M111 S247` for maximum logging.)
          * **Images or videos** demonstrating the problem, if it helps to make it clear.
          * **A G-Code file** that exposes the problem, if not affecting _all_ G-code.

        If you've made any other modifications to the firmware, please describe them in detail in the space provided.

        When pasting formatted text into the box below don't forget to put ` ``` ` (on its own line) before and after to make it readable.
