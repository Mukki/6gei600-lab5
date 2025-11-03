# Raspberry Pi 3B Traffic Light (Bare-metal AArch64)

Minimal bare-metal C project for Raspberry Pi 3 Model B using the system timer interrupt to drive a traffic light (red/yellow/green LEDs).

## Wiring
- Ground: GND (pin 9)
- Red LED: GPIO 17 (pin 11)
- Yellow LED: GPIO 27 (pin 3)
- Green LED: GPIO 22 (pin 15)

## Installation (Docker)

Build and run the project in a Docker container with all dependencies pre-installed.

### Build the Docker image

From the project root directory:

```bash
docker build -t lab5-baremetal .
```

### Build the project using Docker

Run the container and build `kernel8.img`:

```bash
docker run --rm -v "$(pwd):/workspace" -w /workspace lab5-baremetal make CROSS=aarch64-linux-gnu-
```

This command:
- `--rm`: Automatically removes the container after it exits
- `-v "$(pwd):/workspace"`: Mounts the current directory into `/workspace` in the container
- `-w /workspace`: Sets the working directory to `/workspace`
- `CROSS=aarch64-linux-gnu-`: Overrides the Makefile's default `aarch64-elf-` prefix to match the toolchain in the Docker image

The `kernel8.img` file will be created in your current directory and will be accessible outside the container.

### Interactive container session

To open an interactive bash session in the container:

```bash
docker run -it --rm -v "$(pwd):/workspace" -w /workspace lab5-baremetal
```

Then you can run `make CROSS=aarch64-linux-gnu-` manually inside the container.

## Build
Prereq: aarch64-elf-gcc toolchain in PATH.

```
make
```
Produces `kernel8.img`.

## Run
- On SD card boot partition:
  - Add to `config.txt`:
    ```
    arm_64bit=1
    kernel=kernel8.img
    ```
  - Copy `kernel8.img` to the boot partition alongside the Raspberry Pi firmware files.
- Boot the Pi.

## Notes
- SoC base addresses assume Raspberry Pi 3B: peripheral base 0x3F000000.

