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
    poll_contract = PROFILE['generation'].get('poll_waits')
    poll_record = manifest.get('poll_waits')
    if poll_contract:
        expected = json.loads((REVISION / poll_contract).read_text(encoding='utf-8'))
        if not poll_record or poll_record.get('version') != 1 or poll_record.get('contract') != expected:
            raise ValueError('Generated native poll contract differs from this title revision')
        digest = hashlib.sha256(json.dumps(expected, sort_keys=True, separators=(',', ':')).encode()).hexdigest()
        units = json.loads((directory / 'gen/pilot_poll_wait_units.json').read_text(encoding='utf-8'))
        if poll_record.get('contract_sha256') != digest or poll_record.get('units') != units:
            raise ValueError('Generated native poll inventory/provenance is inconsistent')
    elif poll_record:
        raise ValueError('Unexpected native poll contract in generated input')
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
    if PROFILE['generation'].get('poll_waits'):
        command += ['--poll-waits', REVISION / PROFILE['generation']['poll_waits']]
    env = dict(os.environ)
    env['PYTHONPATH'] = str(local['unicorn'].parent) + os.pathsep + env.get('PYTHONPATH', '')
    run(command, env=env)
    check_generated(GENERATED)


def configure(local, mode):
    # Refresh optional callback effects through the framework's strict,
    # reproducible generator; CPU bodies and binding receipts remain unchanged.
    run([sys.executable, FRAMEWORK / 'tools/native_gpu_callback_effects.py',
         '--xbe', local['xbe'], '--output', GENERATED])
    check_generated(GENERATED)
    options = dict(PROFILE['build'], **PROFILE['modes'][mode])
    mapping = {'PILOT_D3D_SOURCE': 'd3d_source', 'PILOT_DSP_SOURCE': 'dsp_source',
               'PILOT_DSP_LIBRARY': 'dsp_library', 'PILOT_ADPCM_SOURCE': 'adpcm_source',
               'PILOT_CALENDAR_KERNEL': 'calendar_kernel', 'PILOT_CALENDAR_UNICORN': 'unicorn',
               'PILOT_CALENDAR_UNICORN_NOTICES': 'unicorn_notices', 'PILOT_IO_DEVICE_SNAPSHOT': 'io_snapshot'}
    if options.get('PILOT_DISPLAY') == 'LLE':
        mapping.update(PILOT_DISPLAY_LIBRARY='display_library',
                       PILOT_DISPLAY_DEPENDENCIES='display_dependencies')
    if options.get('PILOT_FILES') == 'LLE':
        mapping.update(PILOT_FILES_LIBRARY='files_library',
                       PILOT_FILES_DEPENDENCIES='files_dependencies',
                       PILOT_FILES_HDD='files_hdd')
    for key, value in mapping.items():
        if not local[value].exists():
            raise ValueError(f'Missing local {value}: {local[value]}')
        options[key] = str(local[value])
    if local.get('launcher_sdl_sdk'):
        options['PILOT_SDL2_DIR'] = str(local['launcher_sdl_sdk'] / 'cmake')
    run([executable('cmake'), '-S', ROOT, '-B', ROOT / 'build' / mode,
         '-G', 'Visual Studio 17 2022', '-A', 'x64', *[f'-D{key}={value}' for key, value in options.items()]])


def launch(local, args):
    if args.replay and not args.replay.is_file():
        raise ValueError(f'Input replay does not exist: {args.replay}')
    binary = ROOT / 'build' / args.mode / 'framework/Release/BlinxRecomp.exe'
    if not binary.is_file():
        raise ValueError('Build this mode before launching it')
    check_generated(GENERATED)
    original_files = PROFILE['modes'][args.mode].get('PILOT_FILES', PROFILE['build']['PILOT_FILES']) == 'LLE'
    if original_files:
        if not local.get('files_hdd', Path()).is_file():
            raise ValueError('files-lle requires its separately provisioned writable HDD')
    elif not local['state_dir'].is_dir() or not local['media_root'].is_dir():
        raise ValueError('Existing local state_dir and mounted media_root are required')
    label = time.strftime('%Y%m%d-%H%M%S') + '-' + args.mode
    output = ROOT / 'local/runs' / label
    output.mkdir(parents=True, exist_ok=False)
    env = {key: value for key, value in os.environ.items() if not key.startswith(('XLLE_PILOT_', 'BLINX_MOD_'))}
    env.update(PROFILE['runtime'])
    launcher_binary = ROOT / 'build/launcher/Release/xlle_launcher.exe'
    use_preferences = args.launcher or (not args.no_launcher and not args.headless and not args.replay and launcher_binary.is_file())
    if use_preferences:
        if args.headless or args.replay:
            raise ValueError('Interactive launcher requires an owner-facing run')
        import importlib.util
        spec = importlib.util.spec_from_file_location('xlle_native_mod_launcher', FRAMEWORK / 'tools/native_mod_launcher.py')
        ui = importlib.util.module_from_spec(spec);spec.loader.exec_module(ui)
        manifests = [ROOT / 'mods/preloaded/packages/blinx.enhancement.widescreen/1.0.0/manifest.toml']
        manifests += [FRAMEWORK / 'mods/preloaded/packages' / pid / '1.0.0/manifest.toml'
                      for pid in ('xboxlle.enhancement.visuals', 'xboxlle.enhancement.frame-rate', 'xboxlle.enhancement.camera')]
        switch_spec = importlib.util.spec_from_file_location('xlle_native_title_switches', FRAMEWORK / 'tools/native_title_switches.py')
        switches = importlib.util.module_from_spec(switch_spec);switch_spec.loader.exec_module(switches)
        manifests += switches.manifests(ROOT / 'game.toml', PROFILE)
        catalog = ui.Catalog(manifests, ROOT / 'local/mod-selection.json',
                             visibility=[FRAMEWORK / 'mods/ui-visibility.json', ROOT / 'mods/ui-visibility.json'],
                             frame_modes=PROFILE['launcher']['frame_modes'])
        result, image = (0, str(local['xbe']))
        if args.launcher or not catalog.skip_launcher:
            result, image = catalog.run(launcher_binary, output, title=PROFILE['game']['name'], region=PROFILE['game']['region'],
                                       sha=PROFILE['game']['xbe_sha256'], image=local['xbe'])
        if result != 0:
            print('Launcher closed; game was not started.', flush=True)
            return
        local = dict(local, xbe=Path(image))
        check_xbe(local)
        selected = catalog.values()
        quality = 'xboxlle.enhancement.visuals'
        args.widescreen = selected.get(('blinx.enhancement.widescreen', 'widescreen', 'aspect'))
        args.internal_resolution = int(selected.get((quality, 'resolution', 'height'), '0'))
        args.supersampling = int(selected.get((quality, 'supersampling', 'axis'), '1'))
        args.antialiasing = selected.get((quality, 'antialiasing', 'method'), 'original')
        args.anisotropy = int(selected.get((quality, 'filtering', 'anisotropy'), '1'))
        args.frame_mode = selected.get(('xboxlle.enhancement.frame-rate', 'frame-rate', 'mode'), 'original')
        args.frame_rate = int(selected.get(('xboxlle.enhancement.frame-rate', 'frame-rate', 'rate'), '60'))
        env['RECOMP_VOLUME'] = str(catalog.volume)
        input_spec = importlib.util.spec_from_file_location('xlle_native_input_mods', FRAMEWORK / 'tools/native_input_mods.py')
        input_mods = importlib.util.module_from_spec(input_spec);input_spec.loader.exec_module(input_mods)
        env.update(input_mods.environment(catalog))
        env.update(switches.environment(catalog, PROFILE))
        player = catalog.player
        if player['source'] in ('none', 'keyboard'):
            env['XLLE_PILOT_INPUT'] = player['source']
        elif player['source'] == 'gamepad':
            env['XLLE_PILOT_INPUT'] = 'sdl'
            env['XLLE_PILOT_INPUT_GUID'] = player['guid']
    if args.frame_mode != 'original' and args.frame_mode not in PROFILE['launcher']['frame_modes']:
        raise ValueError('This frame-rate mode is not yet qualified for Blinx; use original timing')
    if args.headless or args.replay:
        env['RECOMP_VOLUME'] = '0'
        env['SDL_AUDIODRIVER'] = 'dummy'
    else:
        if env.get('SDL_AUDIODRIVER') == 'dummy':
            env.pop('SDL_AUDIODRIVER')
        if not use_preferences:
            env['RECOMP_VOLUME'] = '100'
    mod_plan = None
    visual_requested = (args.internal_resolution or args.supersampling != 1 or args.antialiasing != 'original'
                        or args.anisotropy != 1 or args.frame_mode != 'original')
    if visual_requested:
        from mods import resolve_visual
        mod_plan = resolve_visual(output / 'mods.plan', aspect=args.widescreen, height=args.internal_resolution,
                                  supersample=args.supersampling, antialiasing=args.antialiasing,
                                  anisotropy=args.anisotropy, frame_mode=args.frame_mode, frame_rate=args.frame_rate,
                                  framework=FRAMEWORK)
        env['BLINX_MOD_PLAN'] = str(output / 'mods.plan')
    elif args.widescreen:
        from mods import resolve
        mod_plan = resolve(args.widescreen, output / 'mods.plan')
        env['BLINX_MOD_PLAN'] = str(output / 'mods.plan')
    env['XLLE_PILOT_VISIBLE'] = '0' if args.headless else '1'
    env['XLLE_PILOT_AUDIO_OUTPUT_MUTE'] = '1' if args.headless or args.replay else '0'
    if args.replay:
        env['XLLE_PILOT_INPUT'] = 'replay'
        env['XLLE_PILOT_INPUT_REPLAY'] = str(args.replay.resolve())
    if args.capture:
        env['XLLE_PILOT_PRESENT_CAPTURE'] = str(output)
        env['XLLE_PILOT_PRESENT_CAPTURE_INTERVAL_MS'] = '10000'
    command = [str(binary), str(local['xbe']), str(local['eeprom']), str(local['media_root']), str(local['state_dir'])]
    if args.diagnose_loading:
        # Diagnostic work is explicit and bounded. Keep real rendering/audio,
        # guest timing and owner volume; automated launches still mute above.
        env.update(XLLE_PILOT_GPU_CALL_PROFILE='1', XLLE_PILOT_FILE_PROFILE='1',
                   XLLE_PILOT_SERVICE_PROFILE='1',
                   XLLE_PILOT_AUDIO_CAPTURE=str(output / 'loading-audio.f32'),
                   XLLE_PILOT_AUDIO_CAPTURE_SECONDS='120',
                   XLLE_PILOT_AUDIO_TIMELINE=str(output / 'loading-audio.csv'))
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
    guards = [line for line in text.splitlines()
              if ('[PILOT' in line and 'STOP]' in line) or '[FAULT]' in line]
    result = dict(mode=args.mode, elapsed=time.monotonic()-start, child_returncode=child.returncode,
                  stopped_at_deadline=deadline, guards=guards, executable_sha256=sha(binary),
                  framework_revision=PROFILE['framework']['revision'], command=command,
                  configuration={key: env[key] for key in env if key.startswith(('XLLE_PILOT_', 'BLINX_MOD_'))},
                  mod_plan=mod_plan)
    if original_files:
        result['original_files_hdd'] = str(local['files_hdd'])
    (output / 'run.json').write_text(json.dumps(result, indent=2)+'\n', encoding='utf-8')
    print(json.dumps({key: result[key] for key in ['elapsed', 'child_returncode', 'stopped_at_deadline', 'guards']}, indent=2))
    if guards or (not deadline and child.returncode):
        raise SystemExit(child.returncode or 2)


def build_launcher(local):
    source = local['launcher_ui']
    revision = subprocess.check_output([executable('git'), '-C', str(source), 'rev-parse', 'HEAD'], text=True).strip()
    if revision != PROFILE['launcher']['ui_revision']:
        raise ValueError('Launcher UI differs from the reviewed game profile')
    prepared = ROOT / 'local/dependencies/recomp-ui-native'
    run([sys.executable, FRAMEWORK / 'tools/native_launcher_ui.py', '--source', source,
         '--output', prepared, '--git', executable('git')])
    run([executable('cmake'), '-S', FRAMEWORK / 'pilots/native-launcher', '-B', ROOT / 'build/launcher',
         '-DXLLE_UI_SOURCE=' + str(prepared), '-DSDL2_DIR=' + str(local['launcher_sdl_sdk'] / 'cmake'),
         '-DXLLE_LAUNCHER_BOXART=' + str(ROOT / 'assets/launcher/boxart.png'),
         '-DXLLE_LAUNCHER_ARTWORK_NOTICE=' + str(ROOT / 'assets/launcher/NOTICE.md')])
    run([executable('cmake'), '--build', ROOT / 'build/launcher', '--config', 'Release', '--parallel', '4'])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=['doctor', 'adopt', 'prepare', 'configure', 'build', 'build-launcher', 'test', 'run'])
    parser.add_argument('--mode', choices=list(PROFILE['modes']), default='attract')
    parser.add_argument('--from-dir', type=Path)
    parser.add_argument('--seconds', type=float)
    parser.add_argument('--headless', action='store_true')
    parser.add_argument('--capture', action='store_true')
    parser.add_argument('--diagnose-loading', action='store_true',
                        help='Record sparse GPU/file timings, service totals and 120 seconds of audio/refill evidence; diagnostic overhead is not a performance benchmark')
    parser.add_argument('--replay', type=Path)
    ui_group = parser.add_mutually_exclusive_group()
    ui_group.add_argument('--launcher', action='store_true', help='Open mod settings, even when Skip launcher is selected')
    ui_group.add_argument('--no-launcher', action='store_true', help='Use explicit CLI mod choices instead of saved launcher preferences')
    parser.add_argument('--widescreen', metavar='ASPECT', help='Enable the widescreen mod: adaptive or ratio such as 21:9 (through 32:9)')
    parser.add_argument('--internal-resolution', type=int, choices=[0, 1080, 1440, 1800, 2160], default=0,
                        help='Internal height; 0 preserves original rendering. Independent of the window size.')
    parser.add_argument('--antialiasing', choices=['original', 'fxaa'], default='original')
    parser.add_argument('--supersampling', type=int, choices=[1, 2, 3], default=1, help='Per-axis multiplier relative to internal resolution')
    parser.add_argument('--anisotropy', type=int, choices=[1, 2, 4, 8, 16], default=1)
    parser.add_argument('--frame-mode', choices=['original'] + PROFILE['launcher']['frame_modes'], default='original',
                        help='One mutually exclusive frame-rate mod; unsupported title/profile choices are rejected')
    parser.add_argument('--frame-rate', type=int, choices=[60, 120], default=60)
    args = parser.parse_args()
    if args.diagnose_loading and args.action != 'run':
        parser.error('--diagnose-loading is only valid with run')
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
        # Static title hooks record one binding per hooked function.
        for record in args.from_dir.glob('*-binding.aot-private.json'):
            shutil.copy2(record, GENERATED)
        check_generated(GENERATED)
    elif args.action == 'prepare':
        prepare(local)
    elif args.action == 'build-launcher':
        build_launcher(local)
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
