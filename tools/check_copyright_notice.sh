#/usr/bin/env bash

set -e
set -o pipefail
set -u

ROOT="$(git rev-parse --show-toplevel)"
NOTICE_PATTERN="SPDX-License-Identifier: BSD-2-Clause"

find_repository_files() {
    git ls-files --cached --exclude-standard -- "${ROOT}"
}

FILES="$(find_repository_files | grep -v '^vendor' | grep -E '\.(c|cpp|h)$')"

for FILE in ${FILES}; do
    if ! grep -q "$NOTICE_PATTERN" "$FILE"; then
        echo "Warning: Copyright notice not found in $FILE"
    fi
done