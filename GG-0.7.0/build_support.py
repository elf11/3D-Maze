# -*- Python -*-

import os
import re
import types
from build_config import *

def OgrePluginName(name):
    return 'GiGiOgrePlugin_' + name

def OptionValue(key, env):
    return env.subst('$' + key)

def ParseOptionsCacheFile(filename):
    retval = {}
    try:
        f = open(filename, 'r')
        lines = f.readlines()
        f.close()
        for i in lines:
            key_and_value = i.split(" = ")
            retval[key_and_value[0]] = key_and_value[1]
    except IOError:
        pass
    return retval

def GenerateHelpText(options, env):
    import textwrap
    retval = ''
    lines = [
        '',
        'Targets:',
        '========',
        '',
        'The default target builds GiGi and GiGiSDL based on the options you specify.  Not that some options force GiGiSDL not to be built.',
        '',
        'install',
        'Installs the default targets in the installation directory, building it first if needed.',
        '',
        'uninstall',
        'Removes the files installed by the "install" targtet from the installation directory.',
        '',
        '',
        'Options:',
        '========',
        '',
        'Each of the following options should be specified with an equals sign (e.g., foo=bar).  Dashes are not required.  Once an option has been specified, it is saved in options.cache for later use; this means an option only needs to be set once.  To change or remove a single option, edit the file.  To clear all options, delete the file.  Note that only command-line option changes are guaranteed to trigger the configuration step when relevant changes are made; manual changes to the options cache are not.',
        ''
        ]
    for i in lines:
        if not i:
            retval += '\n'
        else:
            wrapped_text = textwrap.wrap(i,
                                         width = 78)
            for j in wrapped_text:
                retval += j + '\n'
    for i in options.options:
        retval += i.key + '\n'
        wrapped_text = textwrap.wrap(i.help,
                                     width = 78,
                                     initial_indent = '    ',
                                     subsequent_indent = '    ')
        for j in wrapped_text:
            retval += j + '\n'
        retval += '    value=' + str(OptionValue(i.key, env)) + '\n'
        retval += '    default=' + str(i.default) + '\n\n'
    return retval

def DirHeaders(dir):
    from fnmatch import fnmatchcase
    return [i for i in os.listdir(dir) if fnmatchcase(i, '*.h')]

pc_file_link_flags_used = []
pc_file_paths_used = []
pc_file_lib_paths_used = []
pc_file_libs_used = []

def CreateGiGiPCFile(target, source, env):
    values = {
        'prefix' : env['prefix'],
        'libdir' : env.subst(env['libdir']),
        'incdir' : env.subst(env['incdir']),
        'version' : gigi_version,
        'gigi_libs' : '',
        'boost_include' : ''
    }
    for flag in env['LINKFLAGS']:
        pc_file_link_flags_used.append(flag)
        values['gigi_libs'] += ' ' + (flag[0] != '$' and flag or env.subst(flag))
    for path in env['LIBPATH']:
        if path.find('SDL') == -1: # if it's not SDL-specific, include it
            pc_file_lib_paths_used.append(path)
            values['gigi_libs'] += ' -L' + (path[0] != '$' and path or env.subst(path))
    for path in env['CPPPATH']:
        if path.find('boost') != -1 and path[0] != '#' and path not in pc_file_paths_used:
            pc_file_paths_used.append(path)
            values['boost_include'] += ' -I' + (path[0] != '$' and path or env.subst(path))
    for lib in env['LIBS']:
        if lib.find('IL') == -1 and lib not in pc_file_libs_used:
            pc_file_libs_used.append(lib)
            values['gigi_libs'] += ' -l' + (lib[0] != '$' and lib or env.subst(lib))
    for tgt, src in zip(target, source):
        pc = open(str(tgt), 'w')
        pc_in = open(str(src), 'r')
        pc.write(pc_in.read() % values)
        pc.close()
        pc_in.close()
    return None

def CreateGiGiDriverPCFile(target, source, env):
    values = {
        'prefix' : env['prefix'],
        'incdir' : env.subst(env['incdir']),
        'version' : gigi_version,
        'driver_libs' : ''
    }
    for flag in env['LINKFLAGS']:
        if flag not in pc_file_link_flags_used:
            pc_file_link_flags_used.append(flag)
            values['driver_libs'] += ' ' + (flag[0] != '$' and flag or env.subst(flag))
    for lib in env['LIBS']:
        if lib.find('IL') == -1 and lib not in pc_file_libs_used:
            pc_file_libs_used.append(lib)
            values['driver_libs'] += ' -l' + (lib[0] != '$' and lib or env.subst(lib))
    for tgt, src in zip(target, source):
        pc = open(str(tgt), 'w')
        pc_in = open(str(src), 'r')
        pc.write(pc_in.read() % values)
        pc.close()
        pc_in.close()
    return None

def AppendPackagePaths(package, env):
    root = OptionValue('with_' + package, env)
    inc = OptionValue('with_%s_include' % package, env)
    if not inc and root:
        inc =  os.path.normpath(os.path.join(root, 'include'))
    lib = OptionValue('with_%s_libdir' % package, env)
    if not lib and root:
        lib = os.path.normpath(os.path.join(root, 'lib'))
    if inc:
        env.Append(CPPPATH = [os.path.abspath(inc)])
    if lib:
        env.Append(LIBPATH = [os.path.abspath(lib)])

def CheckPkgConfig(context, version):
    context.Message('Checking for pkg-config... ')
    ret = context.TryAction('pkg-config --atleast-pkgconfig-version %s' % version)[0]
    context.Result(ret)
    return ret

def CheckPkg(context, name, version):
    context.Message('Checking for %s >= %s... ' % (name, version))
    ret = context.TryAction('pkg-config %s --atleast-version %s' % (name, version))[0]
    context.Result(ret)
    return ret

def VersionLEQ(lhs, rhs):
    lhs = lhs.split('.')
    rhs = rhs.split('.')
    try:
        for i in range(len(lhs)):
            if int(rhs[i]) < int(lhs[i]):
                return False
            elif int(lhs[i]) < int(rhs[i]):
                return True
    except Exception:
        for j in range(i, len(lhs)):
            if int(lhs[j]):
                return False
        return True
    return True

def FindRegexMatchesInHeader(regex, filename, env = None):
    f = None
    if env and env.has_key('CPPPATH'):
        for i in env['CPPPATH']:
            try:
                f = open(os.path.normpath(os.path.join(i, filename)), 'r')
                break
            except Exception:
                pass
    if not f:
        for i in ['.', '/usr/include', '/usr/local/include']:
            try:
                f = open(os.path.normpath(os.path.join(i, filename)), 'r')
                break
            except Exception:
                pass
    if f:
        return regex.findall(f.read())
    else:
        return []

def CheckVersionHeader(context, package, header, regex, comparison_string, version_leq_check, message = None):
    retval = False
    if message:
        context.Message(message)
    else:
        if version_leq_check:
            context.Message('Checking %s version >= %s... ' % (package, comparison_string))
        else:
            context.Message('Checking %s version == %s... ' % (package, comparison_string))
    matches = FindRegexMatchesInHeader(regex, header, context.env)
    if len(matches) == 1:
        import types
        if type(matches[0]) == types.StringType:
            match_str = matches[0]
        else:
            match_str = '.'.join(matches[0])
        if version_leq_check:
            retval = VersionLEQ(comparison_string, match_str)
        else:
            retval = comparison_string == match_str
    context.Result(retval and 'yes' or 'no')
    return retval

def CheckBoostLib(context, lib_tuple, conf):
    ret = ""
    lib_name = lib_tuple[0]
    suffix = OptionValue('boost_lib_suffix', context.env)
    if suffix:
        lib_name += suffix
    print 'Looking for boost lib %s... ' % lib_name
    if conf.CheckLibWithHeader(lib_name, lib_tuple[1], 'C++', lib_tuple[2]):
        ret = lib_name
    else:
        lib_name = lib_name + '-mt'
        if conf.CheckLibWithHeader(lib_name, lib_tuple[1], 'C++', lib_tuple[2]):
            ret = lib_name
    return ret

def CheckBoost(context, required_version, lib_tuples, conf, check_libs):
    AppendPackagePaths('boost', context.env)
    if not conf.CheckCXXHeader('boost/shared_ptr.hpp'):
        context.Message('Boost configuration... ')
        context.Result(False)
        return False
    version_regex = re.compile(r'BOOST_VERSION\s*(\d+)')
    if not CheckVersionHeader(context, 'Boost', 'boost/version.hpp', version_regex, boost_version, True, 'Checking Boost version >= %s... ' % boost_version_string):
        context.Message('Boost configuration... ')
        context.Result(False)
        return False
    if check_libs:
        for i in lib_tuples:
            lib = CheckBoostLib(context, i, conf)
            if not lib:
                context.Message('Boost configuration... ')
                context.Result(False)
                return False
    context.Message('Boost configuration... ')
    context.Result('ok')
    return True

def CheckSDL(context, options, conf, sdl_config, check_lib):
    ret = True
    AppendPackagePaths('sdl', context.env)
    context.Message('Checking for sdl-config... ')
    if sdl_config:
        context.Result('yes')
    else:
        context.Result('no')
    build_dynamic = OptionValue('dynamic', context.env)
    sdl_root = OptionValue('with_sdl', context.env)
    found_it_with_sdl_config = False
    if sdl_config:
        sdl_config_prefix_flag = sdl_root and ('--prefix=' + sdl_root) or ''
        context.env.ParseConfig('sdl-config ' + sdl_config_prefix_flag + ' --cflags ' + (build_dynamic and '--libs' or '--static-libs'))
        found_it_with_sdl_config = True
    if not found_it_with_sdl_config:
        if not conf.CheckCHeader('SDL/SDL.h') and not conf.CheckCHeader('SDL.h'):
            context.Message('SDL configuration... ')
            context.Result(False)
            return False
        if check_lib and not conf.CheckLib('SDL', 'SDL_Init'):
            context.Message('SDL configuration... ')
            context.Result(False)
            return False
    version_regex = re.compile(r'SDL_MAJOR_VERSION\s*(\d+).*SDL_MINOR_VERSION\s*(\d+).*SDL_PATCHLEVEL\s*(\d+)', re.DOTALL)
    if not conf.CheckVersionHeader('SDL', 'SDL/SDL_version.h', version_regex, sdl_version, True):
        context.Message('SDL configuration... ')
        context.Result(False)
        return False
    if check_lib:
        context.Message('Linking SDL/OpenGL test app... ')
        link_test_app = """
#include <SDL/SDL.h>
#include <SDL/SDL_opengl.h>
int main(int argc, char **argv)
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_SetVideoMode(640, 480, SDL_GetVideoInfo()->vfmt->BitsPerPixel, SDL_OPENGL);
    glBegin(GL_TRIANGLES);
    glEnd();
    return 0;
}
"""
        if context.TryLink(link_test_app, '.c'):
            context.Result(True)
        else:
            context.Result(False)
            ret = False
    context.Message('SDL configuration... ')
    context.Result(ret)
    return ret

def CheckLibLTDL(context):
    retval = True
    context.Message('Generating libltdl/config.h using libltdl/configure... ')
    initial_dir = os.getcwd()
    os.chdir('libltdl')
    configure_run = os.system('./configure > /dev/null')
    code = os.WEXITSTATUS(configure_run)
    if code:
        retval = False
    os.chdir(initial_dir)
    context.Result(retval and 'ok' or 'failed')
    return retval

def CheckConfigSuccess(context, complete):
    context.Message('Configuration successful... ')
    context.Result(complete and 'yes' or 'no')
    return complete
