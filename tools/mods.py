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


def resolve(aspect, output, catalog=None):
    catalog = catalog or ROOT / 'mods/preloaded/packages' / PACKAGE / '1.0.0/manifest.toml'
    manifest = tomllib.loads(catalog.read_text(encoding='utf-8'))
    if (manifest['format_version'] != 1 or manifest['id'] != PACKAGE or manifest['version'] != '1.0.0'
        or manifest['target'] != [{'game_id': '4D530013', 'xbe_sha256': SHA}]
        or manifest['feature'] != [{'id': 'widescreen', 'name': 'Widescreen', 'default_enabled': False}]
        or manifest['plugin'] != [{'feature': 'widescreen', 'id': 'blinx.widescreen'}]):
        raise ValueError('Unsupported bundled mod identity or plan')
    adaptive = aspect == 'adaptive'
    value = '21:9' if adaptive else aspect
    if not re.fullmatch(r'[1-9][0-9]{0,4}:[1-9][0-9]{0,4}', value):
        raise ValueError('Aspect must be adaptive or numerator:denominator')
    n, d = map(int, value.split(':'))
    if n > 65536 or d > 65536 or not Fraction(4, 3) <= Fraction(n, d) <= Fraction(32, 9):
        raise ValueError('Aspect must be from 4:3 through 32:9')
    ratio = Fraction(n, d)
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


if __name__ == '__main__':
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--aspect', required=True)
    parser.add_argument('--output', required=True, type=Path)
    args = parser.parse_args()
    print(json.dumps(resolve(args.aspect, args.output)))
