"""Check native guards and the real launcher's default/persistence boundary."""
import argparse
import json
from pathlib import Path
import subprocess
import sys
import tempfile
import tomllib

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / 'xboxlle/tools'))
from native_mod_launcher import Catalog
from native_title_switches import environment, manifests


def run(binary):
    for mode in ('default', 'on', 'off'):
        subprocess.run([str(binary), mode], check=True)
    for mode, message in (
        ('invalid', 'invalid Blinx blur-removal selection'),
        ('identity', 'blur-removal title identity mismatch'),
        ('guard', 'Blinx scene-copy coordinates differ'),
        ('locked', 'Blinx scene-copy coordinates are not writable'),
        ('no-writer', 'blur-removal title identity mismatch'),
    ):
        result = subprocess.run([str(binary), mode], capture_output=True, text=True)
        assert result.returncode == 2 and message in result.stderr, (mode, result)
    profile = tomllib.loads((ROOT / 'game.toml').read_text(encoding='utf-8'))
    key = 'blinx.enhancement.blur-removal/blur-removal'
    variable = 'XLLE_PILOT_TITLE_MOD_BLUR_REMOVAL'
    with tempfile.TemporaryDirectory() as temporary:
        state = Path(temporary) / 'selection.json'

        def catalog():
            return Catalog(manifests(ROOT / 'game.toml', profile), state,
                           visibility=[ROOT / 'mods/ui-visibility.json'],
                           defaults=profile['launcher']['defaults'])

        for saved in ({}, {'version': 1, 'features': {}}):
            state.write_text(json.dumps(saved))
            current = catalog()
            feature = next(f for f in current.features if f['package'] + '/' + f['id'] == key)
            assert feature['name'] == 'Remove image blur' and feature['group'] == 'Visual enhancements'
            assert environment(current, profile)[variable] == '1'
        feature['enabled'] = False
        state.write_text(json.dumps(current.selection()))
        restored = catalog()
        assert environment(restored, profile)[variable] == '0', 'Saved off choice was lost'
        assert all(not f['enabled'] for f in restored.features if f['package'] == 'blinx.enhancement.cheats')
    print('PASS blur-removal native guards, visible default and saved-off selection')


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--contract', required=True, type=Path)
    run(parser.parse_args().contract)
