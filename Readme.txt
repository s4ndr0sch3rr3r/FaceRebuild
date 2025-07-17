# FaceRebuild

FaceRebuild is a C++ application for reconstructing 3D models of the human head and brain from MRI scan data. It combines tools from the VTK, ITK, and Qt libraries to process volumetric medical data, generate 3D surface meshes, and provide an interactive visualization interface. This application was developed for educational and research purposes.

<p align="center">
  <img src="images/brainImg.png" alt="Brain Reconstruction" width="45%">
  <img src="images/faceImg.png" alt="Face Reconstruction" width="45%">
</p>

## Features

- Load and visualize MRI scan volumes (e.g., DICOM, NIfTI formats)
- Perform basic image processing and volume manipulation using ITK
- Generate 3D meshes of anatomical structures with both serial and parallel implementations
- Convert ITK image data into VTK-compatible formats for rendering
- Explore MRI slices with a custom scroll interactor
- Render and interact with 3D meshes using VTK within a Qt-based GUI

## Technologies Used

- **VTK (Visualization Toolkit)**: Used for 3D rendering, mesh handling, and visualization pipeline.
- **ITK (Insight Segmentation and Registration Toolkit)**: Used for medical image processing and conversion.
- **Qt Framework**: Provides the graphical user interface and interaction logic.
- **C++ (Modern)**: Implements modular architecture for file handling, rendering, and mesh processing.
- **Multithreading (Standard Library)**: Enables parallel mesh generation for performance improvement.

## Project Structure

The codebase is modularized into several core components:
- `main.cpp`: Application entry point and GUI initialization
- `mainwindow.cpp`: Qt-based GUI implementation
- `filemanager.cpp`: Handles image loading and file conversion
- `imageprocessor.cpp`: Basic image processing routines
- `meshgenerator.cpp`, `serialmeshgenerator.cpp`, `parallelmeshgenerator.cpp`: Mesh generation logic
- `renderermanager.cpp`: Manages rendering pipeline and scene setup
- `itktovtkconverter.cpp`: Converts data between ITK and VTK formats
- `scrollinteractorstyle.cpp`: Enables slice scrolling in the 3D viewer
