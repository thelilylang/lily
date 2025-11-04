#!/usr/bin/env bash

# Usage: ./scripts/ci/valid_format.sh

set -e

docker build -t lily-dev docker/dev
docker run -v "$(pwd):/lily" lily-dev:latest ./scripts/format.sh
