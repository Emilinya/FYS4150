# This is a meta-makefile

problem_dict = {
    5: ("cpp", {
        "plot": False,
        "largs": "-larmadillo",
        "depends": ["project3Utils", "problem5/Particle"]
    }),
    6: ("cpp", {
        "plot": False,
        "largs": "-larmadillo",
        "depends": ["project3Utils", "problem5/Particle", "problem6/PenningTrap"]
    }),
    7: ("cpp", {
        "plot": True,
        "largs": "-larmadillo",
        "depends": ["project3Utils", "problem5/Particle", "problem6/PenningTrap", "problem7/integrator"]
    }),
    8: ("cpp", {
        "plot": True,
        "cargs": "-O3",
        "largs": "-larmadillo",
        "depends": ["project3Utils", "problem5/Particle", "problem6/PenningTrap", "problem7/integrator"],
        "datafiles": ["problem8/data_single_movement.dat"]
    }),
    9: ("cpp", {
        "plot": True,
        "cargs": "-fopenmp -O3",
        "largs": "-larmadillo",
        "depends": ["project3Utils", "problem5/Particle", "problem6/PenningTrap", "problem7/integrator"],
        "datafiles": ["problem9/data_f0.1.dat"]
    }),
}


def cpp_string(i, deps, cargs, largs):
    return f"""
.PHONY: p{i}_cpp
p{i}_cpp: problem{i}/.p{i}_cpp.stamp

problem{i}/.p{i}_cpp.stamp: ./problem{i}/p{i}
\t./problem{i}/p{i};
\ttouch $@
problem{i}/p{i}: problem{i}/p{i}.cpp ../cpp_utils/utils.hpp {' '.join([dep + '.hpp' for dep in deps])}
\tg++ -Wall -Wextra -std=c++17 {cargs} -o $@ $< {largs}
"""


def py_string(i, datafiles):
    imgName = datafiles[0].split(
        "/")[-1].replace("data", f"imgs/p{i}").replace(".dat", ".svg")
    return f"""
.PHONY: p{i}_py
p{i}_py: problem{i}/.p{i}_py.stamp

problem{i}/.p{i}_py.stamp: {imgName}
\ttouch $@
{imgName}: problem{i}/p{i}.py {' '.join(datafiles)}
\tpython3 problem{i}/p{i}.py
"""


def get_cpp_string(i, deps, cargs, largs, plot, datafiles):
    if plot:
        return f"\n.PHONY: p{i}\np{i}: p{i}_cpp p{i}_py\n" + cpp_string(i, deps, cargs, largs) + "\n" + py_string(i, datafiles) + "\n"
    else:
        return f"\n.PHONY: p{i}\np{i}: p{i}_cpp\n" + cpp_string(i, deps, cargs, largs) + "\n"


def get_py_string(i, datafiles):
    return f"\n.PHONY: p{i}\np{i}: p{i}_py\n" + py_string(i, datafiles) + "\n"


with open("Makefile", "w") as makefile:
    makefile.write("# This file is autogenerated, do not edit\n")
    makefile.write(f"""
.PHONY: all
all: {' '.join([f'p{i}' for i in problem_dict.keys()])}
""")

    for i, (ptype, args) in problem_dict.items():
        if ptype == "cpp":
            makefile.write(get_cpp_string(
                i, args.get("depends", []), args.get(
                    "cargs", ""), args.get("largs", ""),
                args.get("plot", True), args.get(
                    "datafiles", [f"problem{i}/data.dat"])
            ))
        elif ptype == "py":
            makefile.write(get_py_string(
                i, args.get("datafiles", [f"problem{i}/data.dat"])
            ))
