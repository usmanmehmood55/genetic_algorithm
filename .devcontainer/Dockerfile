# Use the official image as a parent image
FROM ubuntu:20.04

# Set environment variables to non-interactive (this prevents some prompts)
ENV DEBIAN_FRONTEND=non-interactive

# Run package updates and install packages
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        gdb \
        git \
        ninja-build \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set up a user
RUN useradd -m vscode
USER vscode

# Set the working directory
WORKDIR /workspace

# Overwrite this with 'CMD []' in a dependent Dockerfile
CMD [ "bash" ]