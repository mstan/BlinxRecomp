"""Exercise resolved mod combinations through the compiled static registry.

These are plan/registry contracts. A mock host accepting a future frame mode
does not qualify an actual game-timing implementation.
"""
import argparse
import importlib.util
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('blinx_visual_mods', ROOT / 'tools/mods.py')
mods = importlib.util.module_from_spec(spec);spec.loader.exec_module(mods)


def run(binary, framework):
    with tempfile.TemporaryDirectory() as d:
        path = Path(d) / 'resolved.plan'
        profiles = [dict(height=h) for h in (1080, 1440, 1800, 2160)] + [
            dict(height=1080, supersample=2, antialiasing='fxaa', anisotropy=8),
            dict(height=1080, supersample=3), dict(frame_mode='interpolated', frame_rate=60),
            dict(frame_mode='interpolated', frame_rate=120), dict(frame_mode='actual', frame_rate=60)]
        for options in profiles:
            mods.resolve_visual(path, framework=framework, aspect='21:9', **options)
            subprocess.run([str(binary), str(path)], check=True)
        mods.resolve_visual(path, framework=framework, aspect='adaptive', height=1080,
                            supersample=2, antialiasing='fxaa', anisotropy=8,
                            frame_mode='interpolated', frame_rate=60)
        for transport in ('--legacy-plan', '--common-priority'):
            subprocess.run([str(binary), str(path), transport], check=True)
        mods.resolve_visual(path, framework=framework, height=1080)
        subprocess.run([str(binary), str(path), '--quality-only'], check=True)
        mods.resolve_visual(path, framework=framework, frame_mode='interpolated')
        subprocess.run([str(binary), str(path), '--frame-only'], check=True)
        before = path.read_bytes()
        for options in (dict(height=999), dict(supersample=0), dict(anisotropy=3), dict(frame_mode='both'),
                        dict(aspect='32:9', height=2160, supersample=3), dict(height=True)):
            try:
                mods.resolve_visual(path, framework=framework, **options)
            except ValueError:
                pass
            else:
                raise AssertionError('Invalid choice accepted: ' + repr(options))
            assert path.read_bytes() == before, 'Failed resolution mutated the prior plan'
    print('PASS independent visual plans, frame-only scope/reset and invalid-choice atomicity')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--contract', required=True, type=Path)
    parser.add_argument('--framework', type=Path, default=ROOT / 'xboxlle')
    args = parser.parse_args()
    run(args.contract, args.framework)
