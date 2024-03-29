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
generated_spirv_dir = os.path.join(generated_dir, "spirv")


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


def clear_generated_spirv_files():
    if os.path.exists(generated_spirv_dir):
        shutil.rmtree(generated_spirv_dir)
    os.mkdir(generated_spirv_dir)


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


def get_material_name(shader_name):
    if "_VS" in shader_name:
        return shader_name.split("_VS")[0]
    elif "_PS" in shader_name:
        return shader_name.split("_PS")[0]
    else:
        exit(1)


def convert_source_to_spirv_format(shaders, defines):
    flags = "-Fo"
    for name, item in shaders.items():
        model = get_shader_model(item.shader_type)
        generated_path = os.path.join(generated_spirv_dir, get_generated_spirv_filename(name))
        cmd = [args.DxcPath, "-spirv", "-T", model, "-E", "main"]
        for d in defines:
            cmd.append("-D")
            cmd.append(d)
        cmd.append(item.path)
        cmd.append(flags)
        cmd.append(generated_path)
        p = subprocess.Popen(cmd)
        p.wait()
        if p.returncode != 0:
            exit(p.returncode)

    print("convert_source_to_spirv_format OK!")


# def convert_spirv_to_reflection_meta(shaders):
#     for name, item in shaders.items():
#         source_file_path = os.path.join(generated_dir, get_generated_spirv_filename(name))
#         target_file_path = os.path.join(generated_dir, name + ".json")
#         cmd = [args.SpirvCrossPath, "--reflect", "--output", target_file_path, source_file_path]
#         p = subprocess.Popen(cmd)
#         p.wait()
#         if p.returncode != 0:
#             exit(p.returncode)


def analyze_input_layout(item):
    fp = open(item.path, "r")
    for line in fp.readlines():
        if "main" in line:
            if "VertexInput_P3N3T3" in line:
                return {0: "POSITION", 1: "NORMAL", 2: "TEXCOORD"}
            elif "VertexInput_P3T3" in line:
                return {0: "POSITION", 1: "TEXCOORD"}
            elif "VertexInput_P3N3" in line:
                return {0: "POSITION", 1: "NORMAL"}
            elif "VertexInput_P3" in line:
                return {0: "POSITION"}
            else:
                exit(1)


def collect_all_input_layouts(shaders):
    input_layouts = {}
    for name, item in shaders.items():
        if "_VS" in name:
            material_name = get_material_name(name)
            layout = analyze_input_layout(item)
            input_layouts[material_name] = layout
    return input_layouts


def convert_source_to_hlsl_dx11(shaders, input_layouts):
    clear_generated_spirv_files()
    defines = ["_HLSL_"]
    convert_source_to_spirv_format(shaders, defines)

    for name, item in shaders.items():
        source_file_path = os.path.join(generated_spirv_dir, get_generated_spirv_filename(name))
        target_file_path = os.path.join(generated_dx11_dir, name + ".hlsl")
        cmd = [args.SpirvCrossPath, "--hlsl", "--shader-model", "50", "--remove-unused-variables"]
        input_layout = input_layouts[get_material_name(name)]
        for location, semantic in input_layout.items():
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


def convert_source_to_glsl_opengl(shaders):
    clear_generated_spirv_files()
    defines = ["_GLSL_"]
    convert_source_to_spirv_format(shaders, defines)

    for name, item in shaders.items():
        source_file_path = os.path.join(generated_spirv_dir, get_generated_spirv_filename(name))
        target_file_path = os.path.join(generated_opengl_dir, name + ".glsl")
        # version must greater than 420 to support binding semantic
        p = subprocess.Popen([args.SpirvCrossPath,
                              "--remove-unused-variables",
                              "--version", "420",
                              "--no-420pack-extension",
                              "--combined-samplers-inherit-bindings",
                              "--output", target_file_path,
                              source_file_path])
        p.wait()
        if p.returncode != 0:
            exit(p.returncode)
    print("convert_spirv_to_glsl_opengl ok!")


def main():
    create_generated_dirs()
    shaders = collect_all_shaders()
    input_layouts = collect_all_input_layouts(shaders)
    convert_source_to_hlsl_dx11(shaders, input_layouts)
    convert_source_to_glsl_opengl(shaders)


if __name__ == "__main__":
    main()
