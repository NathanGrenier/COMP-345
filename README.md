# About

## How to Play
You can download and play the game here: https://github.com/NathanGrenier/COMP-345/releases

Steps:
1. Download the `COMP-345.zip` file.
2. Unzip it.
3. Run the `COMP-345.exe` file.

> [!IMPORTANT]
> The game only runs on Windows.

## Team Members

| Name            | Student ID | Email                      |
| --------------- | ---------- | -------------------------- |
| Nathan Grenier  | 40250986   | nathangrenier01@gmail.com  |
| Denmar Ermitano | 40286029   | d_ermita@live.concordia.ca |
| Nirav Patel     | 40248940   | pa_nir@live.concordia.ca   |

## Attributions
Any and all attributions to third-party assets, resources, or contributions used in the project can be found on the [Attributions Wiki](https://github.com/NathanGrenier/COMP-345/wiki/Credits-&-Attributions).

## Analysis and Design

A document outlining the project's analysis and design considerations can be found at: [`deliverables/Analysis_and_Design.md`](deliverables/Analysis_and_Design.md).

## Assignment Documentation

All assignment documentation can be found in the [`deliverables/`](deliverables/) directory. There will be a markdown file for each assignment.

## Source Code Documentation

The project's source code documentation can be found online here: https://comp345.ngrenier.com/

# Getting Started

The repo can be cloned at https://github.com/NathanGrenier/COMP-345.

> [!NOTE]
> The project was tested on VisualStudio v17.3.6+

> [!TIP]
>  If you are running the project via a `.zip` file, skip to step 2.

1. Install the project's dependencies by running `git submodule init` followed by `git submodule update --init --recursive`.
   > If you don't have git installed. You can use the installation .exe in `vendor/`.
2. Select the CMake project name as the startup to execute.
<div align="center">
	<img src="static/select_startup.png" alt="Select Startup" width="500">
</div>
1. Compile and run the project by click the green arrow.
<div align="center">
	<img src="static/run_project.png" alt="Select Startup" width="800">
</div>

# Contributing

## Building the Release Version

Run the following commands to build the application in release mode:
1. `mkdir -p ./build; cd build`
2. `cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXE_LINKER_FLAGS="-Wl" ..`
3. `cmake --build . --config Release`

You can also use the `.\install.bat` script to build and launch the release version.

## Doxygen Comments

In order to generate Doxygen documentation, you must first install the software. Follow this guide (just the installation): https://darkognu.eu/programming/tutorials/doxygen_tutorial_cpp

To generate the documentation, run the following command in the workspace root directory:

```bash
doxygen
```

To view the generated documentation, open the <a href="docs/html/index.html" target="_blank">`docs/html/index.html`</a> file in a web browser.

### Compile Doxygen Documentation to PDF

1. Run the following command (with latex installed): `docs/latex/make.bat`

You can now view the generated PDF at [`docs/latex/refman.pdf`](docs/latex/refman.pdf).
