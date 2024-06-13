#/usr/bin/env bash

set -e
set -o pipefail
set -u

if ! command -v clang-tidy &> /dev/null
then
    echo "clang-tidy could not be found"
    exit 1
fi

ROOT="$(git rev-parse --show-toplevel)"

find_repository_files() {
    git ls-files --cached --exclude-standard -- "${ROOT}"
}

FILES="$(find_repository_files | grep -v '^vendor' | grep -E '\.(c|cpp|h)$')"

CLANG_TIDY="clang-tidy --config-file=${ROOT}/.clang-tidy -p ${ROOT}/build"

echo "${FILES}" | xargs -i -P 0 bash -c "${CLANG_TIDY} {}"

# Copyright (c) 2024 Matheus Sousa
# SPDX-License-Identifier: BSD-2-Clause
