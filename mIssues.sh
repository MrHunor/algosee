#!/usr/bin/env bash
set -euo pipefail

REPO="MrHunor/algosee"
PR="https://github.com/MrHunor/algosee/pull/149"

# Labels that already exist in the repo
mapfile -t EXISTING < <(gh label list --repo "$REPO" --limit 200 --json name --jq '.[].name')

# mk "title" "wanted,labels" < body   -> only labels that exist are attached
mk() {
  local title="$1" wanted="$2" body label
  body="$(cat)"
  local args=()
  IFS=',' read -ra W <<< "$wanted"
  for label in "${W[@]}"; do
    for e in "${EXISTING[@]}"; do
      [[ "$e" == "$label" ]] && args+=(--label "$label") && break
    done
  done
  body+=$'\n\nSource: CodeRabbit review on '"$PR"$'\n\n(Made by AI)'
  if [[ "${DRY_RUN:-0}" == "1" ]]; then
    echo "--- $title | labels: ${args[*]:-none}"
  else
    gh issue create --repo "$REPO" --title "$title" --body "$body" "${args[@]}"
  fi
}

mk "Wait for server readiness before running test client in CI" "bug,ci" <<'EOF'
**File:** `.github/workflows/testExecutable.yaml` (lines 52-62)

The workflow starts `./build/algosee &` but does not wait for port 8080 / `/status` before running `algosee_testclient`. If the first request arrives before the listener is ready, the client marks the test failed and returns -1, so CI can fail even though the server comes up moments later.

**Suggested fix:** In "Start executable for testing", poll `http://127.0.0.1:8080/status` with a bounded retry (e.g. 30 x 1s, short curl timeout) and fail the step if the server never becomes ready.
EOF

mk "Remove -march=native from Release builds" "bug,build" <<'EOF'
**File:** `CMakeLists.txt` (line 55)

`-march=native` in the Release options ties the binary to the build machine's CPU. Published binaries can crash at startup or at runtime on CPUs with fewer instruction-set features.

**Suggested fix:** Remove `$<$<CONFIG:Release>:-march=native>` (or use a documented minimum CPU target). Keep the other configuration-specific options unchanged.
EOF

mk "Normalize CMAKE_BUILD_TYPE for algosee_testclient checks" "bug,build" <<'EOF'
**File:** `CMakeLists.txt` (line 40)

The `CMAKE_BUILD_TYPE STREQUAL "debug"/"release"` comparisons are case-sensitive, so a standard `-DCMAKE_BUILD_TYPE=Debug` skips the `algosee_testclient` conditions. (The CI workflow is unaffected because `rebuildLinuxDebug.sh` passes lowercase `debug`.)

**Suggested fix:** Compute `string(TOLOWER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_NORMALIZED)` once and use it in every build-type check governing `algosee_testclient`.
EOF

mk "Picture sort frontend does not match the /image backend contract" "bug,frontend" <<'EOF'
**File:** `frontend/picsort/js/visualizer.js` (lines 118-151)

The client and backend use different contracts, so every sort request fails:
- Client POSTs JSON to `/picsortalgo`; the server registers `POST /image`.
- `loadImage` expects a multipart file field named `image`.
- Client reads `STEPS` / `SORTED_PIXELS`; the server returns `SORTED`, `SHUFFLED (INDEXES)`, `MOVES`, etc.
- Client sends 100x100 = 10000 pixels, but `MAX_PIXEL_COUNT_IMAGE` is 2500.

**Suggested fix:** Use a canvas of at most 50x50, send its blob as `image` in `FormData` to `/image?algo=...`, and render the returned `SORTED` entries (`[index,R,G,B,A]`).
EOF

mk "gpush.sh: shebang is not on the first line" "bug" <<'EOF'
**File:** `gpush.sh` (line 17)

The shebang sits below the header comments, so direct execution does not use Bash.

**Suggested fix:** Move the shebang to line 1, before the header comments.
EOF

mk "src/defs/general.h: missing <string> include" "bug,backend" <<'EOF'
**File:** `src/defs/general.h` (line 37)

The declarations using `clientListType` do not include the standard string header, so the header fails to compile when included first.

**Suggested fix:** Add the standard `<string>` include. Leave the `randomStringDataset` declaration unchanged.
EOF

mk "Exception what() leaks function name, file, line and column to clients" "security,backend" <<'EOF'
**File:** `src/exception/exception.h` (lines 34-37)

The formatter that builds `retval` puts source-location details into `what()`, and handlers forward that text into HTTP responses. This exposes internal diagnostics to attackers.

**Suggested fix:** Make `what()` contain only a client-safe message. Keep function/file/line/column details in server-side logs only.
EOF

mk "A*: start node uses a different coordinate order than the distance lookup" "bug,backend" <<'EOF'
**File:** `src/pathalgo/ASTAR.cpp` (lines 68-71)

The initial frontier entry uses a different coordinate order than the search's distance lookup, which makes A* fail for most start points.

**Suggested fix:** Use the same coordinate order in the start-node entry as in the distance lookup. Keep the existing priority and heuristic values.
EOF

mk "No-path error should return HTTP 500, not the default 400" "bug,backend" <<'EOF'
**File:** `src/routing/pathalgo.cpp` (lines 75-77)

When `path` is empty, the exception path calls `returnFailedAnswer` with its default 400 status, which misclassifies a server-side condition as a client error.

**Suggested fix:** Pass an explicit 500 status to `returnFailedAnswer` there.
EOF

mk "Pre-routing logs request bodies and iterates the whole client list" "security,backend" <<'EOF'
**File:** `src/routing/pre_routing.cpp` (lines 44-58)

The pre-routing flow logs `req.body` and iterates/logs every `clientList` entry after `addClient`, doing list-wide log I/O on each request (privacy and performance concern).

**Suggested fix:** Omit `req.body` from logging and remove the per-entry client list logging. Keep the mutex-protected client registration.
EOF

mk "sortalgo: do not send formatted e.what() details to clients" "security,backend" <<'EOF'
**File:** `src/routing/sortalgo.cpp` (line 80)

`returnFailedAnswer` receives `e.what()`, which includes internal formatting details.

**Suggested fix:** Log the detailed message on the server only. Store the original user-facing message separately on `excep` and use it for that exception's response; return a generic error for other `std::exception` types.
EOF

mk "sortalgo: excep exit code is ignored, wrong HTTP status codes returned" "bug,backend" <<'EOF'
**File:** `src/routing/sortalgo.cpp` (lines 78-81)

`excep` does not expose the constructor's exit code, so `runSortalgo` cannot pass the intended status (e.g. 413 or 501) to `returnFailedAnswer`.

**Suggested fix:** Store and expose the exit code on `excep`, catch `excep` before `std::exception` in `runSortalgo`, and pass its code to `returnFailedAnswer`. At validation sites that replaced earlier responses, pass the intended 413/501 explicitly. Keep generic `std::exception` handling.
EOF

mk "Rate limiter: addClient evicts at most one expired entry per call" "bug,security,backend" <<'EOF'
**File:** `src/utils/general/utils.cpp` (lines 81-93)

`addClient` removes at most one entry older than an hour per call, so history can grow without a fixed bound and is scanned under the global admission mutex (availability risk).

**Suggested fix:** Before appending `newEntry`, repeatedly remove expired entries from the front of `cList` (it is timestamp-ordered), keeping everything inside the one-hour window.
EOF

mk "Image upload: decode from memory instead of writing a temp file" "security,bug,backend" <<'EOF'
**File:** `src/utils/image/image.cpp` (lines 59-62, also line 70)

Uploads are written to the working directory, and the oversized-image exception throws before `remove` runs, leaving files on disk. Repeated rejected uploads can fill the disk.

**Suggested fix:** Use `stbi_load_from_memory` with `image.content` directly so no temporary file is created or needs cleanup.
EOF

mk "loadImage: unchecked stbi_info result and int overflow in pixel count" "security,bug,backend" <<'EOF'
**File:** `src/utils/image/image.cpp` (lines 69-70)  
**CWE:** CWE-190 (Integer Overflow), potential denial of service

`stbi_info`'s result is ignored and `width * height` is computed as `int`. A failed metadata read can pass the limit check, and large dimensions can overflow before decoding.

**Suggested fix:** Reject failed reads and non-positive dimensions, compute the pixel count with a wide type (e.g. `long long`) before comparing with `MAX_PIXEL_COUNT_IMAGE`, and keep the existing oversized-image error.
EOF

mk "castIntMapToBoolIfNeeded: validate MAP shape before iterating" "bug,backend" <<'EOF'
**File:** `src/utils/pathalgo/pathalgo.cpp` (lines 128-136)

`MAP` and each row are iterated without shape checks. A `MAP` such as `[1,0]` yields a one-dimensional result treated as rows and fails later in unclear ways.

**Suggested fix:** Check `is_array()` on `MAP` and on each row before iterating, and throw `excep` on failure. Keep the existing boolean-detection logic.
EOF

mk "Image validation: per-algorithm pixel limit exception is never thrown" "bug,security,backend" <<'EOF'
**File:** `src/validate/image/validate.cpp` (lines 50-53)

The per-algorithm limit check constructs an `excep` but does not `throw` it, so the limit is never enforced. For example `bogo` can run on up to 2500 pixels and block a server thread indefinitely.

**Suggested fix:** `throw` the constructed exception. Keep the existing message and limit lookup (and fix the "provied" typo).
EOF

mk "Path validation: bound cell costs and require cost >= 1 for ASTAR" "bug,backend" <<'EOF'
**File:** `src/validate/path/validate.cpp` (lines 78-80)

`checkRange` allows any value >= -1. Zero-cost cells make the Manhattan heuristic overestimate in ASTAR (non-optimal paths), and very large costs can overflow `newDistance` in