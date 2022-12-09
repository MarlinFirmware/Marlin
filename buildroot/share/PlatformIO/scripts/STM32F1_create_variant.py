#
# STM32F1_create_variant.py
#
import pioutil
if pioutil.is_pio_build():
    import shutil,marlin
    from pathlib import Path

    Import("env")
    platform = env.PioPlatform()
    board = env.BoardConfig()

    FRAMEWORK_DIR = Path(platform.get_package_dir("framework-arduinoststm32-maple"))
    assert FRAMEWORK_DIR.is_dir()

    source_root = Path("buildroot/share/PlatformIO/variants")
    assert source_root.is_dir()

    variant = board.get("build.variant")
    variant_dir = FRAMEWORK_DIR / "STM32F1/variants" / variant

    source_dir = source_root / variant
    assert source_dir.is_dir()

    if variant_dir.is_dir():
        shutil.rmtree(variant_dir)

    if not variant_dir.is_dir():
        variant_dir.mkdir()

    marlin.copytree(source_dir, variant_dir)
