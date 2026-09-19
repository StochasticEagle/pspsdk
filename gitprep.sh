#!/usr/bin/env bash
# Prepare pspsdk after clone or pull.

set -euo pipefail

ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd -P)"
JOBS="${GITPREP_JOBS:-8}"

if (( EUID == 0 )); then
    echo "ERROR: Do not run gitprep.sh as root." >&2
    exit 1
fi

cd "${ROOT}"

if ! git rev-parse --is-inside-work-tree >/dev/null 2>&1; then
    echo "ERROR: ${ROOT} is not a Git working tree." >&2
    exit 1
fi

TOPLEVEL="$(git rev-parse --show-toplevel)"
if [[ "${TOPLEVEL}" != "${ROOT}" ]]; then
    echo "ERROR: gitprep.sh must live at the repository root." >&2
    exit 1
fi

[[ -f .gitmodules ]] || { echo "No .gitmodules file; nothing to prepare."; exit 0; }

if ! git diff --quiet --ignore-submodules=none -- components 2>/dev/null; then
    echo "ERROR: unstaged component/gitlink changes are present." >&2
    exit 1
fi
if ! git diff --cached --quiet --ignore-submodules=none -- components 2>/dev/null; then
    echo "ERROR: staged component/gitlink changes are present." >&2
    exit 1
fi
if ! git submodule foreach --quiet --recursive 'git diff --quiet && git diff --cached --quiet'; then
    echo "ERROR: a submodule contains uncommitted file changes." >&2
    exit 1
fi

git submodule sync --recursive
git -c remote.origin.tagOpt=--no-tags submodule update --init --recursive --depth 1 --jobs "${JOBS}"
git submodule foreach --quiet --recursive 'git config remote.origin.tagOpt --no-tags'

bad=0
while IFS= read -r line; do
    [[ -z "${line}" ]] && continue
    case "${line:0:1}" in
        " ") ;;
        *) echo "ERROR: bad submodule state: ${line}" >&2; bad=1 ;;
    esac
done < <(git submodule status --recursive)

(( bad == 0 )) || exit 1
echo "Repository prepared successfully."
