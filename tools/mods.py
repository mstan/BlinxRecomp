"""Resolve the bundled, data-only widescreen feature to its static plugin.

This bounded v1 catalog adapter is also the configuration boundary for the
future shared launcher provider. It does not load DLLs or execute package code.
"""
from pathlib import Path
import argparse
import hashlib
import json
import re
import tomllib
from fractions import Fraction

ROOT = Path(__file__).resolve().parents[1]
SHA = 'e6b7cd8a5527dd001e05bb3e881d9b24ffc58824ff67def394215ddbc8009249'
PACKAGE = 'blinx.enhancement.widescreen'


def aspect_value(aspect):
    adaptive = aspect == 'adaptive'
    value = '21:9' if adaptive else aspect
    if not isinstance(value, str) or not re.fullmatch(r'[1-9][0-9]{0,4}:[1-9][0-9]{0,4}', value):
        raise ValueError('Aspect must be adaptive or numerator:denominator')
    n, d = map(int, value.split(':'))
    if n > 65536 or d > 65536 or not Fraction(4, 3) <= Fraction(n, d) <= Fraction(32, 9):
        raise ValueError('Aspect must be from 4:3 through 32:9')
    return Fraction(n, d), adaptive


def resolve(aspect, output, catalog=None):
    catalog = catalog or ROOT / 'mods/preloaded/packages' / PACKAGE / '1.0.0/manifest.toml'
    manifest = tomllib.loads(catalog.read_text(encoding='utf-8'))
    if (manifest['format_version'] != 1 or manifest['id'] != PACKAGE or manifest['version'] != '1.0.0'
        or manifest['target'] != [{'game_id': '4D530013', 'xbe_sha256': SHA}]
        or manifest['feature'] != [{'id': 'widescreen', 'name': 'Widescreen', 'default_enabled': False}]
        or manifest['plugin'] != [{'feature': 'widescreen', 'id': 'blinx.widescreen'}]):
        raise ValueError('Unsupported bundled mod identity or plan')
    ratio, adaptive = aspect_value(aspect)
    plan = (f'xlle-mod-plan 1\npackage {PACKAGE} 1.0.0\nplugin blinx.widescreen\n'
            f'identity {SHA}\naspect {ratio.numerator} {ratio.denominator}\nadaptive {int(adaptive)}\n')
    output = Path(output)
    output.parent.mkdir(parents=True, exist_ok=True)
    temporary = output.with_suffix(output.suffix + '.tmp')
    temporary.write_text(plan, encoding='ascii', newline='\n')
    temporary.replace(output)
    result = {'format_version': 1, 'feature': [PACKAGE, 'widescreen'], 'enabled': True,
              'aspect': aspect, 'plan_sha256': hashlib.sha256(plan.encode('ascii')).hexdigest(),
              'catalog_sha256': hashlib.sha256(catalog.read_bytes()).hexdigest(), 'xbe_sha256': SHA}
    output.with_suffix(output.suffix + '.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


def resolve_visual(output, *, aspect=None, height=0, supersample=1,
                   antialiasing='original', anisotropy=1, frame_mode='original',
                   frame_rate=60, framework=None):
    """Resolve independent bundled features to a complete v2 native plan.

    The frame mode is one choice, never two simultaneously enabled features.
    Unsupported runtime/profile choices fail at the native capability gate.
    Catalog data selects only trusted prelinked implementations.
    """
    framework = Path(framework) if framework is not None else ROOT / 'xboxlle'
    if (any(type(value) is not int for value in (height, supersample, anisotropy, frame_rate))
        or height not in (0, 1080, 1440, 1800, 2160) or supersample not in (1, 2, 3)
        or antialiasing not in ('original', 'fxaa') or anisotropy not in (1, 2, 4, 8, 16)
        or frame_mode not in ('original', 'interpolated', 'actual') or frame_rate not in (60, 120)):
        raise ValueError('Invalid visual mod option')
    ratio, adaptive = aspect_value(aspect) if aspect is not None else (Fraction(4, 3), False)
    canvas_height = height or 480
    canvas_width = (canvas_height * ratio.numerator + ratio.denominator - 1) // ratio.denominator if height else 640
    if max(canvas_width, canvas_height) * supersample > 16384:
        raise ValueError('Selected resolution/aspect/supersampling exceeds the native texture limit')
    operations, catalogs = [], {}
    if aspect is not None:
        path = ROOT / 'mods/preloaded/packages' / PACKAGE / '1.0.0/manifest.toml'
        manifest = tomllib.loads(path.read_text(encoding='utf-8'))
        if (manifest['format_version'] != 1 or manifest['id'] != PACKAGE or manifest['version'] != '1.0.0'
            or manifest['target'] != [{'game_id': '4D530013', 'xbe_sha256': SHA}]
            or manifest['feature'] != [{'id': 'widescreen', 'name': 'Widescreen', 'default_enabled': False}]
            or manifest['plugin'] != [{'feature': 'widescreen', 'id': 'blinx.widescreen'}]):
            raise ValueError('Unsupported widescreen catalog')
        operations.append(f'operation aspect 1 blinx.widescreen {ratio.numerator} {ratio.denominator} {int(adaptive)}')
        catalogs[PACKAGE] = hashlib.sha256(path.read_bytes()).hexdigest()

    def catalog(package, features, plugins):
        path = framework / 'mods/preloaded/packages' / package / '1.0.0/manifest.toml'
        manifest = tomllib.loads(path.read_text(encoding='utf-8'))
        if (manifest['format_version'] != 1 or manifest['id'] != package or manifest['version'] != '1.0.0'
            or manifest['target'] != [{'system': 'xbox', 'native_title_mod_abi': 2}]
            or [f['id'] for f in manifest['feature']] != features
            or any(f['default_enabled'] for f in manifest['feature']) or manifest['plugin'] != plugins):
            raise ValueError('Unsupported shared visual catalog: ' + package)
        catalogs[package] = hashlib.sha256(path.read_bytes()).hexdigest()

    if height or supersample != 1 or antialiasing != 'original' or anisotropy != 1:
        catalog('xboxlle.enhancement.visuals', ['resolution', 'antialiasing', 'supersampling', 'filtering'],
                [{'feature': f, 'id': 'xboxlle.visuals.' + f} for f in ('resolution', 'antialiasing', 'supersampling', 'filtering')])
    if height: operations.append(f'operation resolution 1 {height}')
    if antialiasing != 'original': operations.append('operation antialiasing 1 1')
    if supersample != 1: operations.append(f'operation supersampling 1 {supersample}')
    if anisotropy != 1: operations.append(f'operation filtering 1 {anisotropy}')
    if frame_mode != 'original':
        catalog('xboxlle.enhancement.frame-rate', ['frame-rate'], [{'feature': 'frame-rate', 'id': 'xboxlle.frame-rate'}])
        operations.append(f'operation frame-rate 1 {1 if frame_mode == "interpolated" else 2} {frame_rate}')
    plan = f'xlle-mod-plan 2\nidentity {SHA}\n' + ''.join(op + '\n' for op in operations) + 'end\n'
    output = Path(output);output.parent.mkdir(parents=True, exist_ok=True)
    temporary = output.with_suffix(output.suffix + '.tmp')
    temporary.write_text(plan, encoding='ascii', newline='\n');temporary.replace(output)
    result = dict(format_version=2, xbe_sha256=SHA, operations=operations,
                  plan_sha256=hashlib.sha256(plan.encode('ascii')).hexdigest(), catalog_sha256=catalogs,
                  requested=dict(aspect=aspect,height=height,supersample=supersample,antialiasing=antialiasing,
                                 anisotropy=anisotropy,frame_mode=frame_mode,frame_rate=frame_rate),
                  internal_canvas=[canvas_width*supersample,canvas_height*supersample])
    output.with_suffix(output.suffix + '.json').write_text(json.dumps(result, indent=2) + '\n')
    return result


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--aspect', required=True)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    print(json.dumps(resolve(args.aspect, args.output)))
