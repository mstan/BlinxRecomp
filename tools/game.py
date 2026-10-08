#!/usr/bin/env python3
"""Build and launch this title through its pinned shared Xbox framework."""
import argparse
import hashlib
import json
import os
from pathlib import Path
import shutil
import subprocess
import sys
import time
import tomllib

ROOT = Path(__file__).resolve().parents[1]
PROFILE = tomllib.loads((ROOT / 'game.toml').read_text(encoding='utf-8'))
FRAMEWORK = ROOT / PROFILE['framework']['directory']
GENERATED = ROOT / 'build/generated'
REVISION = ROOT / 'config/revisions/ntsc-u'


def sha(path):
    with path.open('rb') as source:
        return hashlib.file_digest(source, 'sha256').hexdigest()


def executable(name):
    windows = {'cmake': 'C:/Program Files/CMake/bin/cmake.exe',
               'ctest': 'C:/Program Files/CMake/bin/ctest.exe',
               'git': 'C:/Program Files/Git/cmd/git.exe'}
    candidate = windows.get(name)
    if os.name == 'nt' and candidate and Path(candidate).is_file():
        return candidate
    found = shutil.which(name)
    if not found:
        raise ValueError(f'{name} executable missing')
    return found


def run(command, **kwargs):
    subprocess.run([str(value) for value in command], check=True, **kwargs)


def paths():
    config = ROOT / 'local/paths.json'
    if not config.is_file():
        raise ValueError('Create ignored local/paths.json using local-paths.example.json')
    return {key: (ROOT / value).resolve() for key, value in json.loads(config.read_text(encoding='utf-8-sig')).items()}


def check_framework():
    commit = subprocess.check_output([executable('git'), '-C', str(FRAMEWORK), 'rev-parse', 'HEAD'], text=True).strip()
    if commit != PROFILE['framework']['revision']:
        raise ValueError('Framework checkout differs from game.toml; review and update the pin together')
    run([executable('git'), '-C', FRAMEWORK, 'diff', '--quiet', 'HEAD', '--'])


def check_xbe(local):
    if sha(local['xbe']) != PROFILE['game']['xbe_sha256']:
        raise ValueError('Owned XBE does not match original NTSC Blinx')


def check_generated(directory):
    manifest = json.loads((directory / 'generation.aot-private.json').read_text(encoding='utf-8'))
    if manifest['sha256'] != PROFILE['game']['xbe_sha256'] or manifest['donor'] != PROFILE['generation']['donor_revision']:
        raise ValueError('Generated identity or CPU donor differs from the reviewed profile')
    actual = {path.name for path in (directory / 'gen').iterdir() if path.is_file()}
    if actual != set(manifest['files']):
        raise ValueError('Generated file inventory differs from its manifest')
    for name, expected in manifest['files'].items():
        if Path(name).name != name or sha(directory / 'gen' / name) != expected:
            raise ValueError(f'Changed generated artifact: {name}')
    expected_bindings = json.loads((REVISION / 'binding-records.json').read_text(encoding='utf-8'))
    for family, expected in expected_bindings.items():
        if sha(directory / f'{family}-binding.aot-private.json') != expected:
            raise ValueError(f'{family} revision binding differs from the game-owned record')
    if json.loads((directory / 'function-extents.aot-private.json').read_text(encoding='utf-8')) != json.loads((REVISION / 'function-extents.json').read_text(encoding='utf-8')):
        raise ValueError('Generated original-function recovery evidence differs from this title revision')


def prepare(local):
    GENERATED.mkdir(parents=True, exist_ok=True)
    shutil.copy2(REVISION / 'function-extents.json', GENERATED / 'function-extents.aot-private.json')
    command = [sys.executable, FRAMEWORK / 'tools/native_pilot.py', '--donor', local['cpu_donor'],
               '--xbe', local['xbe'], '--output', GENERATED, '--data-sections', PROFILE['generation']['data_sections'],
               '--seed-functions', REVISION / 'seeds.json']
    for family in PROFILE['generation']['bindings']:
        command += [f'--{family}-symbols', local['xdk_symbols']]
    env = dict(os.environ)
    env['PYTHONPATH'] = str(local['unicorn'].parent) + os.pathsep + env.get('PYTHONPATH', '')
    run(command, env=env)
    check_generated(GENERATED)


def configure(local, mode):
    check_generated(GENERATED)
    options = dict(PROFILE['build'], **PROFILE['modes'][mode])
    mapping = {'PILOT_D3D_SOURCE': 'd3d_source', 'PILOT_DSP_SOURCE': 'dsp_source',
               'PILOT_DSP_LIBRARY': 'dsp_library', 'PILOT_ADPCM_SOURCE': 'adpcm_source',
               'PILOT_CALENDAR_KERNEL': 'calendar_kernel', 'PILOT_CALENDAR_UNICORN': 'unicorn',
               'PILOT_CALENDAR_UNICORN_NOTICES': 'unicorn_notices', 'PILOT_IO_DEVICE_SNAPSHOT': 'io_snapshot'}
    if options.get('PILOT_DISPLAY') == 'LLE':
        mapping.update(PILOT_DISPLAY_LIBRARY='display_library',
                       PILOT_DISPLAY_DEPENDENCIES='display_dependencies')
    for key, value in mapping.items():
        if not local[value].exists():
            raise ValueError(f'Missing local {value}: {local[value]}')
        options[key] = str(local[value])
    run([executable('cmake'), '-S', ROOT, '-B', ROOT / 'build' / mode,
         '-G', 'Visual Studio 17 2022', '-A', 'x64', *[f'-D{key}={value}' for key, value in options.items()]])


def launch(local, args):
    binary = ROOT / 'build' / args.mode / 'framework/Release/BlinxRecomp.exe'
    if not binary.is_file():
        raise ValueError('Build this mode before launching it')
    check_generated(GENERATED)
    if not local['state_dir'].is_dir() or not local['media_root'].is_dir():
        raise ValueError('Existing local state_dir and mounted media_root are required')
    label = time.strftime('%Y%m%d-%H%M%S') + '-' + args.mode
    output = ROOT / 'local/runs' / label
    output.mkdir(parents=True, exist_ok=False)
    env = {key: value for key, value in os.environ.items() if not key.startswith('XLLE_PILOT_')}
    env.update(PROFILE['runtime'])
    env['XLLE_PILOT_VISIBLE'] = '0' if args.headless else '1'
    if args.replay:
        env['XLLE_PILOT_INPUT'] = 'replay'
        env['XLLE_PILOT_INPUT_REPLAY'] = str(args.replay.resolve())
    if args.capture:
        env['XLLE_PILOT_PRESENT_CAPTURE'] = str(output)
        env['XLLE_PILOT_PRESENT_CAPTURE_INTERVAL_MS'] = '10000'
    command = [str(binary), str(local['xbe']), str(local['eeprom']), str(local['media_root']), str(local['state_dir'])]
    start = time.monotonic()
    deadline = False
    with (output / 'title.log').open('w', encoding='utf-8') as log:
        child = subprocess.Popen(command, env=env, stdout=log, stderr=subprocess.STDOUT)
        print(f'Blinx PID {child.pid}; evidence {output}', flush=True)
        try:
            child.wait(timeout=args.seconds)
        except subprocess.TimeoutExpired:
            deadline = True
            child.kill()
            child.wait()
        except KeyboardInterrupt:
            child.terminate()
            child.wait()
    text = (output / 'title.log').read_text(encoding='utf-8', errors='replace')
    guards = [line for line in text.splitlines() if any(marker in line for marker in ['[PILOT STOP]', '[PILOT AUDIO STOP]', '[FAULT]'])]
    result = dict(mode=args.mode, elapsed=time.monotonic()-start, child_returncode=child.returncode,
                  stopped_at_deadline=deadline, guards=guards, executable_sha256=sha(binary),
                  framework_revision=PROFILE['framework']['revision'], command=command,
                  configuration={key: env[key] for key in env if key.startswith('XLLE_PILOT_')})
    (output / 'run.json').write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({key: result[key] for key in ['elapsed', 'child_returncode', 'stopped_at_deadline', 'guards']}, indent=2))
    if guards or (not deadline and child.returncode):
        raise SystemExit(child.returncode or 2)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=['doctor', 'adopt', 'prepare', 'configure', 'build', 'test', 'run'])
    parser.add_argument('--mode', choices=list(PROFILE['modes']), default='attract')
    parser.add_argument('--from-dir', type=Path)
    parser.add_argument('--seconds', type=float)
    parser.add_argument('--headless', action='store_true')
    parser.add_argument('--capture', action='store_true')
    parser.add_argument('--replay', type=Path)
    args = parser.parse_args()
    if args.seconds is not None and args.seconds <= 0:
        parser.error('--seconds must be positive')
    local = paths()
    check_framework()
    check_xbe(local)
    if args.action == 'adopt':
        if not args.from_dir:
            parser.error('adopt requires --from-dir')
        check_generated(args.from_dir)
        if GENERATED.exists():
            raise ValueError('Private generated directory already exists; adopt will not overwrite it')
        GENERATED.mkdir(parents=True)
        shutil.copytree(args.from_dir / 'gen', GENERATED / 'gen')
        for name in ['generation.aot-private.json', 'identity.aot-private.json', 'function-extents.aot-private.json']:
            shutil.copy2(args.from_dir / name, GENERATED / name)
        for family in PROFILE['generation']['bindings'] + ['display']:
            shutil.copy2(args.from_dir / f'{family}-binding.aot-private.json', GENERATED)
        check_generated(GENERATED)
    elif args.action == 'prepare':
        prepare(local)
    elif args.action in ['configure', 'build']:
        configure(local, args.mode)
        if args.action == 'build':
            run([executable('cmake'), '--build', ROOT / 'build' / args.mode, '--config', 'Release', '--parallel', '4'])
    elif args.action == 'test':
        check_generated(GENERATED)
        run([executable('ctest'), '--test-dir', ROOT / 'build' / args.mode, '-C', 'Release', '--output-on-failure'])
    elif args.action == 'run':
        launch(local, args)
    else:
        check_generated(GENERATED)
        print('Reviewed NTSC input, framework pin, generated file hashes and revision bindings verified.')


if __name__ == '__main__':
    try:
        main()
    except (ValueError, OSError, KeyError, subprocess.CalledProcessError) as exc:
        raise SystemExit(str(exc))
