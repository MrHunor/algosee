/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

// read URL-Parameter
const urlParams = new URLSearchParams(window.location.search);
const algoKey = urlParams.get('algo') || 'bubble';

const backendBase = getBackendBase();

// display names (config.js does not define these)
const imageAlgoNames = {
    bogo: "BogoSort",
    miracle: "MiracleSort",
    selection: "SelectionSort",
    merge: "MergeSort",
    bubble: "BubbleSort",
    counting: "CountingSort",
    quick: "QuickSort",
    cycle: "CycleSort",
    radix: "RadixSort",
    intro: "IntroSort"
};

document.getElementById('algo-title').innerText = imageAlgoNames[algoKey] || "Image Visualization";

const imageUpload = document.getElementById('image-upload');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');
const stopBtn = document.getElementById('stop-btn');

const sourceCanvas = document.getElementById('source-canvas');
const sourceCtx = sourceCanvas.getContext('2d');
const targetCanvas = document.getElementById('target-canvas');
const targetCtx = targetCanvas.getContext('2d');

// state
let uploadedImage = null;
let isCancelled = false;
let abortController = null;
let run = null; // data collected from the stream for the current run

// draw init placeholder
function drawPlaceholder() {
    sourceCtx.fillStyle = '#161616';
    sourceCtx.fillRect(0, 0, sourceCanvas.width, sourceCanvas.height);
    sourceCtx.fillStyle = '#666';
    sourceCtx.font = '12px system-ui';
    sourceCtx.textAlign = 'center';
    sourceCtx.fillText('No image selected', sourceCanvas.width / 2, sourceCanvas.height / 2);

    targetCtx.fillStyle = '#161616';
    targetCtx.fillRect(0, 0, targetCanvas.width, targetCanvas.height);
    targetCtx.fillStyle = '#666';
    targetCtx.font = '12px system-ui';
    targetCtx.textAlign = 'center';
    targetCtx.fillText('Result preview', targetCanvas.width / 2, targetCanvas.height / 2);
}

drawPlaceholder();

// draw the image scaled to both canvases
function renderImageToCanvas(img) {
    sourceCtx.clearRect(0, 0, sourceCanvas.width, sourceCanvas.height);
    targetCtx.clearRect(0, 0, targetCanvas.width, targetCanvas.height);
    sourceCtx.drawImage(img, 0, 0, sourceCanvas.width, sourceCanvas.height);
    targetCtx.drawImage(img, 0, 0, targetCanvas.width, targetCanvas.height);
}

// image upload
imageUpload.addEventListener('change', (e) => {
    const file = e.target.files[0];
    if (!file) return;

    const url = URL.createObjectURL(file);
    const img = new Image();
    img.onload = () => {
        uploadedImage = img;
        renderImageToCanvas(img);
        document.getElementById('time-val').innerText = '-';
        URL.revokeObjectURL(url);
    };
    img.onerror = () => {
        URL.revokeObjectURL(url);
        alert("Could not load this file as an image.");
    };
    img.src = url;
});

// reset
resetBtn.addEventListener('click', () => {
    isCancelled = true;
    if (abortController) abortController.abort();
    if (uploadedImage) {
        renderImageToCanvas(uploadedImage);
    } else {
        drawPlaceholder();
    }
    document.getElementById('time-val').innerText = '-';
});

// stop
stopBtn.addEventListener('click', () => {
    isCancelled = true;
    if (abortController) abortController.abort();
});

// start button (sends picture to server, collects the NDJSON stream, then animates)
startBtn.addEventListener('click', async () => {
    if (!uploadedImage) {
        alert("Please upload an image first!");
        return;
    }

    const width = sourceCanvas.width;
    const height = sourceCanvas.height;

    // endpoint for picsort
    const backendUrl = `${backendBase}/image?algo=${algoKey}`;

    startBtn.disabled = true;
    resetBtn.disabled = true;
    imageUpload.disabled = true;
    stopBtn.disabled = false;
    isCancelled = false;
    abortController = new AbortController();
    run = newRun();
    document.getElementById('time-val').innerText = '-';

    try {
        // 1. export canvas as blob
        const blob = await new Promise((resolve) => {
            sourceCanvas.toBlob(resolve, 'image/png');
        });

        // 2. create FormData
        const formData = new FormData();
        formData.append('image', blob, 'upload.png');

        // 3. POST as multipart/form-data
        const response = await fetch(backendUrl, {
            method: 'POST',
            body: formData,
            signal: abortController.signal
        });

        // errors before the stream started (validation etc.)
        if (!response.ok) {
            const txt = await response.text();
            let msg = txt;
            try {
                const parsed = JSON.parse(txt);
                if (typeof parsed === 'string') msg = parsed;
                else if (parsed && parsed.ERROR) msg = parsed.ERROR;
            } catch (_) {}
            throw new Error(msg || `server-error: ${response.status}`);
        }

        // 4. read NDJSON line by line and collect the data
        const reader = response.body.getReader();
        const decoder = new TextDecoder();
        let buffer = '';

        while (true) {
            const { done, value } = await reader.read();
            if (done) break;
            buffer += decoder.decode(value, { stream: true });

            let nl;
            while ((nl = buffer.indexOf('\n')) >= 0) {
                const line = buffer.slice(0, nl).trim();
                buffer = buffer.slice(nl + 1);
                if (line) handleChunk(JSON.parse(line), width, height);
                if (isCancelled) return;
            }
        }

        // last line without trailing newline (safety)
        const rest = (buffer + decoder.decode()).trim();
        if (rest && !isCancelled) handleChunk(JSON.parse(rest), width, height);

        // 5. everything received -> play the animation
        if (!isCancelled) await playRun(width, height);

    } catch (error) {
        if (error.name === 'AbortError') {
            // stopped by the user, nothing to report
        } else {
            console.error("Connection-Error:", error);
            alert(error.message);
        }
    } finally {
        abortController = null;
        startBtn.disabled = false;
        resetBtn.disabled = false;
        imageUpload.disabled = false;
        stopBtn.disabled = true;
    }
});

// ---------- collecting the stream ----------

function newRun() {
    return {
        shuffled: null, // [[id,R,G,B,A], ...]
        sorted: null,   // [[id,R,G,B,A], ...]
        moves: [],      // selection/bubble/quick: [posA, posB] = swap
        swaped: [],     // cycle: [displacedId, position]
        merged: [],     // merge: [left, right, result] (arrays of pixels)
        tries: []       // bogo: complete states
    };
}

function appendTo(target, source) {
    if (!Array.isArray(source)) return;
    for (const item of source) target.push(item);
}

// handles one decoded chunk of the stream (only collects, playback happens at the end)
function handleChunk(data, width, height) {
    if (isCancelled) return;

    // once the stream runs, HTTP status is already 200, so errors arrive as chunks
    if (data.ERROR) throw new Error(data.ERROR);

    // initial shuffled state: show it right away
    const shuffled = data["SHUFFLED (INDEXES)"];
    if (Array.isArray(shuffled) && shuffled.length > 0) {
        run.shuffled = shuffled;
        setWorkingState(shuffled);
        renderPixels(workPixels, width, height);
    }

    // timing (nanoseconds -> ms)
    if (data.TIME !== undefined) {
        document.getElementById('time-val').innerText = (data.TIME / 1_000_000).toFixed(2);
    }

    appendTo(run.moves, data.MOVES);
    appendTo(run.swaped, data.SWAPED);
    appendTo(run.merged, data["MERGED (LEFT,RIGHT,RESULT)"]);
    appendTo(run.tries, data.TRIES);

    if (Array.isArray(data.SORTED) && data.SORTED.length > 0) {
        run.sorted = data.SORTED;
    }
}

// ---------- working state ----------

const ANIMATION_MS = 4000; // target duration of the whole animation

let workPixels = null; // Uint8ClampedArray, flat RGBA of the current state
let workIds = null;    // Int32Array, pixel id per position
let posOfId = null;    // Int32Array, current position per pixel id
let colorById = null;  // Uint8ClampedArray, RGBA per pixel id

function setWorkingState(arr) {
    const n = arr.length;
    workPixels = new Uint8ClampedArray(n * 4);
    workIds = new Int32Array(n);
    posOfId = new Int32Array(n);
    colorById = new Uint8ClampedArray(n * 4);
    for (let i = 0; i < n; i++) {
        const [id, r, g, b, a] = arr[i];
        workIds[i] = id;
        workPixels[i * 4] = r;
        workPixels[i * 4 + 1] = g;
        workPixels[i * 4 + 2] = b;
        workPixels[i * 4 + 3] = a;
        if (id >= 0 && id < n) {
            posOfId[id] = i;
            colorById[id * 4] = r;
            colorById[id * 4 + 1] = g;
            colorById[id * 4 + 2] = b;
            colorById[id * 4 + 3] = a;
        }
    }
}

// swap the pixels at two positions
function swapPositions(a, b) {
    const n = workIds.length;
    if (a === b || a < 0 || b < 0 || a >= n || b >= n) return;
    const idA = workIds[a], idB = workIds[b];
    workIds[a] = idB; workIds[b] = idA;
    posOfId[idB] = a; posOfId[idA] = b;
    for (let k = 0; k < 4; k++) {
        const t = workPixels[a * 4 + k];
        workPixels[a * 4 + k] = workPixels[b * 4 + k];
        workPixels[b * 4 + k] = t;
    }
}

// write the pixel with the given id to a position
function writePixel(pos, id) {
    if (pos < 0 || pos >= workIds.length || id < 0 || id >= posOfId.length) return;
    workIds[pos] = id;
    posOfId[id] = pos;
    workPixels.set(colorById.subarray(id * 4, id * 4 + 4), pos * 4);
}

// ---------- playback ----------

function delay(ms) {
    return new Promise(resolve => setTimeout(resolve, ms));
}

// plays `total` steps over ~ANIMATION_MS; several steps per frame, one draw per frame
function playSteps(total, apply, stepMs, width, height) {
    return new Promise((resolve) => {
        if (total === 0) { resolve(); return; }
        const frameMs = Math.max(16.7, stepMs);
        const frames = Math.max(1, Math.round(ANIMATION_MS / frameMs));
        const perFrame = Math.max(1, Math.ceil(total / frames));
        let pos = 0;
        let last = 0;

        function frame(ts) {
            if (isCancelled) { resolve(); return; }
            if (stepMs && ts - last < stepMs) { requestAnimationFrame(frame); return; }
            last = ts;
            const end = Math.min(pos + perFrame, total);
            for (; pos < end; pos++) apply(pos);
            renderPixels(workPixels, width, height);
            if (pos < total) requestAnimationFrame(frame);
            else resolve();
        }
        requestAnimationFrame(frame);
    });
}

async function playRun(width, height) {
    // nothing to animate -> just show the result
    if (!run.shuffled) {
        if (run.sorted) renderPixels(toFlat(run.sorted), width, height);
        return;
    }

    await delay(300); // let the shuffled image be seen

    const n = workIds.length;
    const finalIds = run.sorted ? run.sorted.map(p => p[0]) : null;
    let total = 0;
    let apply = null;
    let stepMs = 0;

    switch (algoKey) {
        // [posA, posB] = swap positions
        case 'selection':
        case 'bubble':
        case 'quick':
            total = run.moves.length;
            apply = (k) => swapPositions(run.moves[k][0], run.moves[k][1]);
            break;

        // [displacedId, position]: one cycle = swaps of the cycle start with each position.
        // The cycle start is the first position that is not yet in its final place.
        case 'cycle':
            if (finalIds) {
                total = run.swaped.length;
                let scan = 0;
                let start = -1;
                apply = (k) => {
                    if (start < 0) {
                        while (scan < n && workIds[scan] === finalIds[scan]) scan++;
                        start = scan;
                    }
                    const p = run.swaped[k][1];
                    swapPositions(start, p);
                    if (p === start) start = -1; // cycle finished
                };
            }
            break;

        // [left, right, result]: the merged run is written back to where the left part starts
        case 'merge':
            total = run.merged.length;
            apply = (k) => {
                const left = run.merged[k][0];
                const result = run.merged[k][2];
                if (!left || !left.length || !result) return;
                const lo = posOfId[left[0][0]];
                for (let j = 0; j < result.length; j++) writePixel(lo + j, result[j][0]);
            };
            break;

        // counting sort writes its output array front to back
        case 'counting':
            if (finalIds) {
                total = n;
                apply = (k) => writePixel(k, finalIds[k]);
            }
            break;

        // every try is a complete state
        case 'bogo':
            total = run.tries.length;
            stepMs = 60;
            apply = (k) => {
                const state = run.tries[k];
                for (let j = 0; j < state.length; j++) writePixel(j, state[j][0]);
            };
            break;
    }

    if (apply) await playSteps(total, apply, stepMs, width, height);
    if (isCancelled) return;

    // final state from the backend is always the truth
    if (run.sorted) {
        if (workIds && run.sorted.some((p, i) => p[0] !== workIds[i])) {
            console.warn(`Animation for "${algoKey}" ended differently than SORTED (move format mismatch?).`);
        }
        renderPixels(toFlat(run.sorted), width, height);
    }
}

// ---------- helpers ----------

// [[index, R, G, B, A], ...] -> flat [R, G, B, A, R, G, B, A, ...]
function toFlat(arr) {
    const flat = [];
    for (const [, r, g, b, a] of arr) flat.push(r, g, b, a);
    return flat;
}

// help function for drawing pixel array to canvas
function renderPixels(pixelArray, width, height) {
    const newImgData = targetCtx.createImageData(width, height);
    newImgData.data.set(pixelArray);
    targetCtx.putImageData(newImgData, 0, 0);
}