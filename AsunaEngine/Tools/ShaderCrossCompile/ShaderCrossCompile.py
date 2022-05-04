import shutil
import os
import argparse
import subprocess

DXC_PATH_HELP = "Path to dxc.exe, \nsee [https://github.com/microsoft/DirectXShaderCompiler/wiki/SPIR-V-CodeGen]"
SPIRV_CROSS_HELP = "Path to spirv-cross.exe, \nsee [https://github.com/KhronosGroup/SPIRV-Cross]"
SHADER_ROOT_HELP = "Path to Shader Directory"


parser = argparse.ArgumentParser(description="Shader cross compile tool")
parser.add_argument("--DxcPath", required=True, help=DXC_PATH_HELP)
parser.add_argument("--SpirvCrossPath", required=True, help=SPIRV_CROSS_HELP)
parser.add_argument("--ShaderRoot", required=True, help=SHADER_ROOT_HELP)
args = parser.parse_args()

generated_dir = os.path.join(args.ShaderRoot, "Generated")
generated_dx11_dir = os.path.join(generated_dir, "dx11")
generated_opengl_dir = os.path.join(generated_dir, "opengl")


def create_generated_dirs():
    if os.path.exists(generated_dir):
        shutil.rmtree(generated_dir)
    os.mkdir(generated_dir)

    if os.path.exists(generated_dx11_dir):
        shutil.rmtree(generated_dx11_dir)
    os.mkdir(generated_dx11_dir)

    if os.path.exists(generated_opengl_dir):
        shutil.rmtree(generated_opengl_dir)
    os.mkdir(generated_opengl_dir)
    print("create_generated_dirs ok!")


Shader_Type_None = 0
Shader_Type_VS = 1
Shader_Type_PS = 2


class CompileItem(object):
    def __init__(self, path, st):
        self.path = path
        self.shader_type = st


def get_shader_type(name):
    if name.endswith("_VS.hlsl"):
        return Shader_Type_VS
    elif name.endswith("_PS.hlsl"):
        return Shader_Type_PS
    else:
        return Shader_Type_None


def collect_all_shaders():
    shaders_to_type = {}
    for root, ds, fs in os.walk(args.ShaderRoot):
        for f in fs:
            st = get_shader_type(f)
            if st == Shader_Type_None:
                continue
            name = f.split(".")[0]
            if f in shaders_to_type:
                print("duplicated shader name error!")
                exit(1)
            path = os.path.join(root, f)
            shaders_to_type[name] = CompileItem(path, st)

    print("collect_all_shaders ok!")
    return shaders_to_type


def get_shader_model(shader_type):
    if shader_type == Shader_Type_VS:
        return "vs_6_0"
    elif shader_type == Shader_Type_PS:
        return "ps_6_0"
    else:
        exit(1)


def get_generated_spirv_filename(name):
    return name + ".spirv"


def convert_source_to_spirv_format(shaders):
    flags = "-Fo"
    for name, item in shaders.items():
        model = get_shader_model(item.shader_type)
        generated_file_name = get_generated_spirv_filename(name)
        generated_path = os.path.join(generated_dir, generated_file_name)
        p = subprocess.Popen([args.DxcPath, "-spirv", "-T", model, "-E", "main", item.path, flags, generated_path])
        p.wait()
        if p.returncode != 0:
            exit(p.returncode)

    print("convert_source_to_spirv_format OK!")


hlsl_vertex_semantic =\
    {
        0: "POSITION",
        1: "NORMAL",
        2: "TEXCOORD0"
    }


def convert_spirv_to_hlsl_dx11(shaders):
    for name, item in shaders.items():
        source_file_path = os.path.join(generated_dir, get_generated_spirv_filename(name))
        target_file_path = os.path.join(generated_dx11_dir, name + ".hlsl")
        cmd = [args.SpirvCrossPath, "--hlsl", "--shader-model", "50"]

        for location, semantic in hlsl_vertex_semantic.items():
            cmd.append("--set-hlsl-vertex-input-semantic")
            cmd.append(str(location))
            cmd.append(semantic)

        cmd.append("--output")
        cmd.append(target_file_path)
        cmd.append(source_file_path)
        p = subprocess.Popen(cmd)
        p.wait()
        if p.returncode != 0:
            exit(p.returncode)
    print("convert_spirv_to_hlsl_dx11 ok!")


def convert_spirv_to_glsl_opengl(shaders):
    for name, item in shaders.items():
        source_file_path = os.path.join(generated_dir, get_generated_spirv_filename(name))
        target_file_path = os.path.join(generated_opengl_dir, name + ".glsl")
        # version must greater than 420 to support binding semantic
        p = subprocess.Popen([args.SpirvCrossPath, "--version", "420", "--no-420pack-extension", "--output",
                              target_file_path, source_file_path])
        p.wait()
        if p.returncode != 0:
            exit(p.returncode)
    print("convert_spirv_to_glsl_opengl ok!")


def main():
    create_generated_dirs()
    shaders = collect_all_shaders()
    convert_source_to_spirv_format(shaders)
    convert_spirv_to_hlsl_dx11(shaders)
    convert_spirv_to_glsl_opengl(shaders)


if __name__ == "__main__":
    main()
