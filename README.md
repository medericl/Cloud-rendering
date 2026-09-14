# Cloud Rendering

A volumetric cloud rendering project written in C++ and implemented from scratch.

The project implements the complete cloud rendering pipeline without relying on an existing rendering engine, including procedural cloud generation, ray marching, volumetric lighting, noise functions, and mathematical primitives.

## Features

* Volumetric cloud rendering implemented from scratch
* Ray marching through a 3D cloud volume
* Procedural cloud generation using custom fBm and Worley noise
* Light absorption and transmittance
* Henyey-Greenstein phase function for cloud lighting
* Wind and cloud coverage simulation
* Custom vector, image and scene components
* Interactive interface using ImGui

## Build

```bash id="ktv3fs"
make
```

Run the application:

```bash id="k36q71"
./ray
```

A version without the ImGui interface can also be built with:

```bash id="ovmtvl"
make fast
./ray_fast
```

## Screenshots

