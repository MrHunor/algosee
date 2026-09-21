#!/usr/bin/env bash
# Algosee sorting benchmark
# this is made fully from ai as this is just meant to be a little benchmark, because of this it also contains no license
#
# Requirements:
#   curl jq seq shuf paste awk sort head tail wc sleep
#
# Usage:
#   chmod +x benchmark.sh
#   ./benchmark.sh
#
# Analyze an existing benchmark:
#   ./benchmark.sh --overview benchmark_v0.3_2026-09-16_20-15-32.csv
#
# Optional:
#   BASE_URL=https://algosee.onrender.com RUNS=5 ./benchmark.sh
#
# Output example:
#   benchmark_v0.3_2026-09-16_20-15-32.csv
#
# IMPORTANT:
# TIME is the C++ server-side sorting time in nanoseconds.
# Network latency is NOT included in TIME.
#



export LC_ALL=C
export LANG=C

set -u
set -o pipefail

echo
echo "========================================"
echo "ALGOsEE BENCHMARK STARTED"
echo "========================================"
echo "PID: $$"
echo "Shell: $SHELL"
echo "Working directory: $(pwd)"
echo

BASE_URL="${BASE_URL:-https://algosee.onrender.com}"
RUNS="${RUNS:-5}"
DELAY="${DELAY:-0.05}"

# ------------------------------------------------------------
# Commands
# ------------------------------------------------------------

require_cmd() {
    command -v "$1" >/dev/null 2>&1 || {
        echo "ERROR: '$1' is required but was not found." >&2
        exit 1
    }
}

for cmd in curl jq seq shuf paste awk sort head tail wc sleep date; do
    require_cmd "$cmd"
done

if ! [[ "$RUNS" =~ ^[1-9][0-9]*$ ]]; then
    echo "ERROR: RUNS must be a positive integer." >&2
    exit 1
fi

# ------------------------------------------------------------
# Overview mode
# ------------------------------------------------------------

overview_csv() {
    local csv="$1"

    if [[ ! -f "$csv" ]]; then
        echo "ERROR: CSV file not found: $csv" >&2
        exit 1
    fi

    echo "========================================"
    echo "BENCHMARK OVERVIEW"
    echo "========================================"
    echo "CSV: $csv"
    echo

    echo "Raw timing summary:"
    echo

    awk -F, '
        NR == 1 { next }

        {
            algo=$1
            n=$2
            median=$4

            count[algo]++

            if (!(algo in min_n) || n < min_n[algo])
                min_n[algo]=n

            if (!(algo in max_n) || n > max_n[algo])
                max_n[algo]=n

            if (!(algo in min_t) || median < min_t[algo])
                min_t[algo]=median

            if (!(algo in max_t) || median > max_t[algo])
                max_t[algo]=median
        }

        END {
            algos[1]="selection"
            algos[2]="bubble"
            algos[3]="merge"
            algos[4]="counting"
            algos[5]="quick"
            algos[6]="bogo"

            for (i=1; i<=6; i++) {
                a=algos[i]

                if (a in count) {
                    printf "  %-10s sizes=%-3d  n=%-6d..%-6d  median=%8.3f..%-8.3f ms\n",
                        a,
                        count[a],
                        min_n[a],
                        max_n[a],
                        min_t[a] / 1000000.0,
                        max_t[a] / 1000000.0
                }
            }
        }
    ' "$csv"

    echo
    echo "========================================"
    echo "EMPIRICAL TIME-COMPLEXITY ESTIMATES"
    echo "========================================"

    for algo in selection bubble merge counting quick bogo; do
        awk -F, -v wanted="$algo" '
            NR > 1 && $1 == wanted && $4 > 0 {
                x = log($2)
                y = log($4)

                sx += x
                sy += y
                sxx += x * x
                sxy += x * y
                syy += y * y

                n++
            }

            END {
                if (n < 2) {
                    printf "%-10s insufficient data\n", wanted
                    exit
                }

                denom = n * sxx - sx * sx

                if (denom == 0) {
                    printf "%-10s insufficient data\n", wanted
                    exit
                }

                # Fit:
                #   log(T) = log(C) + p*log(n)
                #
                # Therefore:
                #   T ~= C*n^p

                p = (n * sxy - sx * sy) / denom

                rden = (n * sxx - sx * sx) * \
                       (n * syy - sy * sy)

                if (rden > 0)
                    r2 = ((n * sxy - sx * sy) ^ 2) / rden
                else
                    r2 = 0

                printf "%-10s p=%7.3f   R^2=%6.3f   T ~= n^%.3f\n",
                    wanted, p, r2, p
            }
        ' "$csv"
    done

    echo
    echo "========================================"
    echo "THEORETICAL COMPLEXITIES"
    echo "========================================"
    echo "  selection : O(n^2)"
    echo "  bubble    : O(n^2) average/worst; O(n) best with early exit"
    echo "  merge     : O(n log n)"
    echo "  counting  : O(n + k), where k is the integer value range"
    echo "  quick     : O(n log n) average; O(n^2) worst"
    echo "  bogo      : expected factorial-scale behavior; highly impractical"
    echo
    echo "NOTE: empirical p is a fitted measurement,"
    echo "      not a proof of asymptotic complexity."
}

if [[ "${1:-}" == "--overview" ]]; then
    if [[ $# -ne 2 ]]; then
        echo "Usage: $0 --overview benchmark.csv" >&2
        exit 1
    fi

    overview_csv "$2"
    exit 0
fi

# ------------------------------------------------------------
# Benchmark configuration
# ------------------------------------------------------------

SELECTION_SIZES=(10 25 50 100 200 400 800 1200)
BUBBLE_SIZES=(10 25 50 100 200 400 800 1200)
MERGE_SIZES=(10 25 50 100 200 400 800 1200 2500 5000)
COUNTING_SIZES=(10 25 50 100 200 400 800 1200 2500 5000)
QUICK_SIZES=(10 25 50 100 200 400 800 1200 2500 5000)

# Kept extremely small because tries stores every attempted permutation.
BOGO_SIZES=(3 4 5 6 7 8)

ALGOS=(selection bubble merge counting quick bogo)

# ------------------------------------------------------------
# Check server and get version
# ------------------------------------------------------------

echo "Checking server: $BASE_URL"

if ! status="$(curl -fsS --max-time 15 "$BASE_URL/status")"; then
    echo "ERROR: Could not reach $BASE_URL/status" >&2
    exit 1
fi

if ! jq -e '.status == "online"' >/dev/null 2>&1 <<< "$status"; then
    echo 'ERROR: Server did not return {"status":"online"}.' >&2
    echo "$status" >&2
    exit 1
fi

echo "Server online."

# We need VERSION from an actual /sortalgo response.
# Use the smallest safe request.
version_response=""

if version_response="$(
    curl -fsS --max-time 30 \
        -X POST "$BASE_URL/sortalgo?algo=selection" \
        -H "Content-Type: application/json" \
        -d '{"values":[2,1]}'
)"; then

    VERSION="$(jq -r '.VERSION // "unknown"' <<< "$version_response")"
else
    VERSION="unknown"
fi

# Sanitize version for a filename.
VERSION_FILE="$(sed 's/[^A-Za-z0-9._-]/_/g' <<< "$VERSION")"

# Timestamp for this benchmark.
TIMESTAMP="$(date '+%Y-%m-%d_%H-%M-%S')"

OUT="${OUT:-benchmark_v${VERSION_FILE}_${TIMESTAMP}.csv}"

echo "Program version: $VERSION"
echo "Output CSV: $OUT"
echo

# ------------------------------------------------------------
# CSV header
# ------------------------------------------------------------

rm -f "$OUT"

echo "algorithm,n,runs,median_ns,min_ns,max_ns,median_ms" > "$OUT"

# ------------------------------------------------------------
# Helpers
# ------------------------------------------------------------

make_values() {
    local n="$1"

    seq 1 "$n" | shuf | paste -sd, -
}

median() {
    sort -n "$1" | awk '
        {
            a[NR] = $1
        }

        END {
            if (NR == 0)
                exit 1

            if (NR % 2 == 1)
                print a[(NR + 1) / 2]
            else
                print (a[NR / 2] + a[NR / 2 + 1]) / 2
        }
    '
}

run_request() {
    local algo="$1"
    local values="$2"

    curl -fsS --max-time 120 \
        -X POST "$BASE_URL/sortalgo?algo=$algo" \
        -H "Content-Type: application/json" \
        -d "{\"values\":[${values}]}"
}

# ------------------------------------------------------------
# Benchmark one input size
# ------------------------------------------------------------

benchmark_one() {
    local algo="$1"
    local n="$2"

    local values
    local response
    local time
    local count
    local med
    local min
    local max

    values="$(make_values "$n")"

    : > /tmp/algosee_bench_times.txt

    for ((run=1; run<=RUNS; run++)); do

        echo -n "    run $run/$RUNS... "

        if ! response="$(run_request "$algo" "$values")"; then
            echo "FAILED"
            continue
        fi

        # TIME is nanoseconds from std::chrono in the C++ server.
        time="$(jq -r '.TIME // empty' <<< "$response")"

        if [[ ! "$time" =~ ^[0-9]+$ ]]; then
            echo "INVALID RESPONSE"
            continue
        fi

        echo "$time" >> /tmp/algosee_bench_times.txt

        echo "${time} ns"

        sleep "$DELAY"
    done

    count="$(wc -l < /tmp/algosee_bench_times.txt | tr -d ' ')"

    if (( count == 0 )); then
        echo "    No valid timings for $algo, n=$n" >&2
        return 1
    fi

    med="$(median /tmp/algosee_bench_times.txt)"
    min="$(sort -n /tmp/algosee_bench_times.txt | head -n1)"
    max="$(sort -n /tmp/algosee_bench_times.txt | tail -n1)"

    # Save raw result.
    awk \
        -v algo="$algo" \
        -v n="$n" \
        -v runs="$count" \
        -v med="$med" \
        -v min="$min" \
        -v max="$max" \
        'BEGIN {
            printf "%s,%d,%d,%.0f,%d,%d,%.6f\n",
                algo,
                n,
                runs,
                med,
                min,
                max,
                med / 1000000.0
        }' >> "$OUT"

    # Human-readable output.
    printf \
        '    => n=%-6d median=%10.3f ms  min=%10.3f ms  max=%10.3f ms\n' \
        "$n" \
        "$(awk -v x="$med" 'BEGIN { printf "%.3f", x / 1000000 }')" \
        "$(awk -v x="$min" 'BEGIN { printf "%.3f", x / 1000000 }')" \
        "$(awk -v x="$max" 'BEGIN { printf "%.3f", x / 1000000 }')"
}

# ------------------------------------------------------------
# Run benchmark
# ------------------------------------------------------------

for algo in "${ALGOS[@]}"; do

    echo "========================================"
    echo "Benchmarking: $algo"
    echo "========================================"

    case "$algo" in
        selection)
            sizes=("${SELECTION_SIZES[@]}")
            ;;
        bubble)
            sizes=("${BUBBLE_SIZES[@]}")
            ;;
        merge)
            sizes=("${MERGE_SIZES[@]}")
            ;;
        counting)
            sizes=("${COUNTING_SIZES[@]}")
            ;;
        quick)
            sizes=("${QUICK_SIZES[@]}")
            ;;
        bogo)
            sizes=("${BOGO_SIZES[@]}")
            ;;
    esac

    for n in "${sizes[@]}"; do
        benchmark_one "$algo" "$n" || true
    done

    echo
done

# ------------------------------------------------------------
# Final analysis
# ------------------------------------------------------------

overview_csv "$OUT"
