"""Run bounded native projection/HUD contracts and reject invalid mod plans."""
from pathlib import Path
import importlib.util
import argparse
import os
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[1]
spec = importlib.util.spec_from_file_location('blinx_mods', ROOT / 'tools/mods.py')
mods = importlib.util.module_from_spec(spec)
spec.loader.exec_module(mods)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--contract', required=True, type=Path)
    args = parser.parse_args()
    executable = str(args.contract.resolve())
    env = {k: v for k, v in os.environ.items() if not k.startswith('BLINX_MOD_')}
    subprocess.run([executable], env=env, check=True)
    with tempfile.TemporaryDirectory(prefix='blinx-mod-contract-') as temporary:
        path = Path(temporary) / 'resolved.plan'
        for aspect in ('4:3', '16:9', '21:9', '32:9', '3440:1440', 'adaptive'):
            first = mods.resolve(aspect, path)
            second = mods.resolve(aspect, path)
            assert first == second
            subprocess.run([executable, str(path)], env=env, check=True)
        for invalid in ('0:9', '64:9', '1:1', 'nan:9', '65537:20000', '16:9 extra'):
            try:
                mods.resolve(invalid, path)
            except ValueError:
                pass
            else:
                raise AssertionError('accepted invalid aspect: ' + invalid)
        mods.resolve('21:9', path)
        original = path.read_text()
        for text in (original.replace(mods.SHA, '0' * 64),
                     original.replace('blinx.widescreen', 'unknown.plugin'),
                     original.replace('aspect 7 3', 'aspect 64 9'),
                     original + 'unplanned-operation\n'):
            path.write_text(text)
            result = subprocess.run([executable, str(path)], env=env, capture_output=True, text=True)
            assert result.returncode == 2 and '[TITLE MOD STOP]' in result.stderr, result
    print('PASS: disabled; six aspect/projection/HUD/menu profiles; six resolver guards; four native plan guards')


if __name__ == '__main__':
    main()
