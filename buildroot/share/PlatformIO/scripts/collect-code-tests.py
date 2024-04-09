#
# collect-code-tests.py
# Convenience script to collect all code tests
#

import pioutil
if pioutil.is_pio_build():

    import os
    Import("env")
    Import("projenv")

    os.environ['PATH'] = f"./buildroot/bin/:./buildroot/tests/:{os.environ['PATH']}"

    def collect_test_suites():
        """Get all the test suites"""
        from pathlib import Path
        return list(Path("./test").glob("test_*/test_all.ini"))

    def register_test_suites():
        """Register all the test suites"""
        test_suites = collect_test_suites()
        for path in test_suites:
            name = path.parent.name
            configuration = get_configuration(path)
            env.AddCustomTarget(
                name=f"marlin_{name}",
                dependencies=None,
                actions=[
                    f"echo ====== Configuring for marlin_{name} ======",
                    "restore_configs",
                    "opt_set MOTHERBOARD BOARD_SIMULATED",
                ] + configuration + [
                    f"echo ====== Finished configuring for marlin_{name} ======",
                    f"platformio test -e linux_native_test -f {name}",
                    f"restore_configs",
                ],
                title="Marlin: {}".format(name.lower().title().replace("_", " ")),
                description=(
                    f"Run a Marlin test suite, with the appropriate configuration, "
                    f"that sits in {path}"
                ),
            )
        env.AddCustomTarget(
            name="test-marlin",
            dependencies=None,
            actions=[
                f"platformio run -t marlin_{path.parent.name} -e linux_native_test"
                for path in test_suites
            ],
            title="Marlin: Test all code test suites",
            description=(
                f"Run all Marlin code test suites ({len(test_suites)} found), each "
                f"with the appropriate configuration"
            ),
        )

    def get_configuration(path):
        """Get the configuration from a test suite"""
        code_lines = path.read_text().splitlines()

        # Find start index
        start_indexes = [
            index
            for index, line in enumerate(code_lines)
            if 'START_CONFIGURATION' in line
        ]
        if not start_indexes:
            raise Exception(
                f"Test suite {path.name} did not contain 'START_CONFIGURATION'")
        elif len(start_indexes) > 1:
            raise Exception(
                f"Test suite {path.name} contained too many instances of "
                f"'START_CONFIGURATION'")
        start_index, = start_indexes

        # Find end index
        end_indexes = [
            index
            for index, line in enumerate(code_lines)
            if 'END_CONFIGURATION' in line
        ]
        if not end_indexes:
            raise Exception(
                f"Test suite {path.name} did not contain 'END_CONFIGURATION'")
        elif len(end_indexes) > 1:
            raise Exception(
                f"Test suite {path.name} contained too many instances of "
                f"'END_CONFIGURATION'")
        end_index, = end_indexes

        # Remove whitespace, empty lines, and commented out lines
        configuration_lines = code_lines[start_index + 1:end_index]
        configuration_lines = [line.strip() for line in configuration_lines]
        configuration_lines = list(filter(None, configuration_lines))
        configuration_lines = [
            line
            for line in configuration_lines
            if not line.startswith("//")
        ]

    def get_configuration(path):
        """Get the configuration from a test suite"""
        configuration_lines = path.read_text().splitlines()

        # Remove whitespace, empty lines, and commented out lines
        configuration_lines = [line.strip() for line in configuration_lines]
        configuration_lines = list(filter(None, configuration_lines))
        configuration_lines = [
            line
            for line in configuration_lines
            if not line.startswith("//")
        ]

        return configuration_lines


    register_test_suites()
