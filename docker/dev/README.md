# docker/dev

This document describes how to build and run a Docker image so that
it can be used for development purposes only in this project.

## Build

From the root of the repository:

```bash
docker build -t lily-dev docker/dev
```

## Run

```bash
docker run -it -v "$(pwd):/lily" lily-dev:latest
```
