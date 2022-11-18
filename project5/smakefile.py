# This is a meta-makefile

problem_dict = {
	"problem2": ("cpp", {
		"plot": False,
		"largs": "-larmadillo",
		"depends": ["include/crank_nicolson_ititializer"]
	}),
	"problem4": ("cpp", {
		"plot": True,
		"largs": "-larmadillo",
		"depends": ["include/schrodinger_intiializer"],
		"datafile": "problem7/u.bin",
	}),
	"problem7": ("cpp", {
		"plot": True,
		"cargs": "-O3",
		"largs": "-larmadillo",
		"depends": ["include/crank_nicolson_ititializer", "include/schrodinger_intiializer", "include/schrodinger_solver"],
		"datafile": "problem7/wall_us.bin",
	}),
}


def cpp_string(name, deps, cargs, largs, data_path):
	return f"""
.PHONY: {name}_data
{name}_data: {data_path}

{data_path}: ./src/{name}/output/data.out
	./src/{name}/output/data.out
src/{name}/output/data.out: src/{name}/data.cpp ../cpp_utils/utils.hpp {' '.join([dep + '.hpp' for dep in deps])}
	@mkdir -p src/{name}/output
	$(CXX) -Wall -Wextra -std=c++17 {cargs} -o $@ $< {largs}
"""


def py_string(name, data_path):
	return f"""
.PHONY: {name}_plot
{name}_plot: src/{name}/output/.plot.stamp

src/{name}/output/.plot.stamp: src/{name}/plot.py {data_path}
	@mkdir -p imgs/{name}
	python3 src/{name}/plot.py
	@touch $@
"""


def get_cpp_string(name, deps, cargs, largs, plot, datafile):
	data_src, data_name = datafile.split("/")
	data_path = "src/" + data_src + "/output/" + data_name

	if plot:
		return f"\n.PHONY: {name}\n{name}: {name}_data {name}_plot\n" + cpp_string(name, deps, cargs, largs, data_path) + "\n" + py_string(name, data_path) + "\n"
	else:
		return f"\n.PHONY: {name}\n{name}: {name}_data\n" + cpp_string(name, deps, cargs, largs, data_path) + "\n"


def get_py_string(name, datafile):
	data_src, data_name = datafile.split("/")
	data_path = "src/" + data_src + "/output/" + data_name
	return f"\n.PHONY: {name}\n{name}: {name}_plot\n" + py_string(name, data_path) + "\n"


with open("Makefile", "w") as makefile:
	makefile.write("# This file is autogenerated, do not edit\n")
	makefile.write(f"""
.PHONY: all
all: {' '.join(problem_dict.keys())}
""")

	for name, (ptype, args) in problem_dict.items():
		if ptype == "cpp":
			makefile.write(get_cpp_string(
				name, args.get("depends", []), args.get(
					"cargs", ""), args.get("largs", ""),
				args.get("plot", True), args.get(
					"datafile", f"{name}/data.dat")
			))
		elif ptype == "py":
			makefile.write(get_py_string(
				name, args.get("datafile", f"{name}/data.dat")
			))

	makefile.write("""
DIR_NAME ?= ""

.PHONY: init_dir
init_dir:
	@if [ $(DIR_NAME) != "" ]; then\\
		echo "Initializing $(DIR_NAME)";\\
		mkdir -p imgs/$(DIR_NAME);\\
		mkdir -p src/$(DIR_NAME)/output;\\
		touch src/$(DIR_NAME)/data.cpp;\\
		touch src/$(DIR_NAME)/plot.py;\\
	fi

.PHONY: clean_dir
clean_dir:
	@if [ $(DIR_NAME) != "" ]; then\\
		echo "Cleaning up $(DIR_NAME)";\\
		rm -rf src/$(DIR_NAME)/output;\\
		rm -rf imgs/$(DIR_NAME);\\
	fi
""")