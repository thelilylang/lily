#!/usr/bin/env bash

# Usage: ./scripts/check_format.sh

set -e
set -o pipefail

./scripts/format.sh --dry-run --Werror -- --check
