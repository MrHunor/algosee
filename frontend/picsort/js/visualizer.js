/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

//read URL-Parameter 
const urlParams = new URLSearchParams(window.location.search);
const algoKey = urlParams.get('algo') || 'bubble';

const serverParam = urlParams.get('server') || 'remote';
const backendBase = serverParam === 'local' 
    ? 'http://localhost:8080' 
    : 'https://algosee.onrender.com';

// adj title
const algoNames = {
    bogo: "BogoSort (Image)",
    miracle: "MiracleSort (Image)",
    selection: "SelectionSort (Image)",
    merge: "MergeSort (Image)",
    bubble: "BubbleSort (Image)",
    counting: "CountingSort (Image)",
    quick: "QuickSort (Image)",
    cycle: "CycleSort (Image)",
    radix: "RadixSort (Image)",
    intro: "IntroSort (Image)"
};
document.getElementById('algo-title').innerText = algoNames[algoKey] || "Image Visualization";

const imageUpload = document.getElementById('image-upload');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');
const stopBtn = document.getElementById('stop-btn');

const sourceCanvas = document.getElementById('source-canvas');
const sourceCtx = sourceCanvas.getContext('2d');
const targetCanvas = document.getElementById('target-canvas');
const targetCtx = targetCanvas.getContext('2d');

let isCancelled = false;
let uploadedImage = null;

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

// read picture and scale to canvas (100x100)
imageUpload.addEventListener('change', (e) => {
    const file = e.target.files[0];
    if (!file) return;

    const reader = new FileReader();
    reader.onload = function(event) {
        const img = new Image();
        img.onload = function() {
            uploadedImage = img;
            renderImageToCanvas(img);
        }
        img.src = event.target.result;
    }
    reader.readAsDataURL(file);
});

function renderImageToCanvas(img) {
    const size = 100; // size for grid
    sourceCanvas.width = size;
    sourceCanvas.height = size;
    targetCanvas.width = size;
    targetCanvas.height = size;

    sourceCtx.drawImage(img, 0, 0, size, size);
    targetCtx.drawImage(img, 0, 0, size, size);
}

// reset
resetBtn.addEventListener('click', () => {
    isCancelled = true;
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
});

// start button (sends picture to server and processes response)
startBtn.addEventListener('click', async () => {
    if (!uploadedImage) {
        alert("Please upload an image first!");
        return;
    }

    const width = sourceCanvas.width;
    const height = sourceCanvas.height;
    const imgData = sourceCtx.getImageData(0, 0, width, height);
    
    // extract pixel array
    const pixels = Array.from(imgData.data);

    // endpoint for picsort
    const backendUrl = `${backendBase}/picsortalgo?algo=${algoKey}`;

    startBtn.disabled = true;
    resetBtn.disabled = true;
    imageUpload.disabled = true;
    stopBtn.disabled = false;
    isCancelled = false;

    try {
        const response = await fetch(backendUrl, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ width: width, height: height, pixels: pixels })
        });

        if (!response.ok) throw new Error('Error while communicating with Backend');

        const data = await response.json();

        // give out time
        if (data.TIME !== undefined) {
            const timeMs = (data.TIME / 1_000_000).toFixed(2);
            document.getElementById('time-val').innerText = timeMs;
        }

        // visualize sorting
        if (data.STEPS && Array.isArray(data.STEPS)) {
            await visualizeImageSteps(data.STEPS, width, height);
        } else if (data.SORTED_PIXELS) {
            // incase final response gets send
            renderPixels(data.SORTED_PIXELS, width, height);
        } else {
            alert("No valid steps received from backend.");
        }

    } catch (error) {
        console.error("Connection-Error:", error);
        alert("Error connecting to backend.");
    } finally {
        startBtn.disabled = false;
        resetBtn.disabled = false;
        imageUpload.disabled = false;
        stopBtn.disabled = true;
    }
});

// frame-by-frame animation
async function visualizeImageSteps(steps, width, height) {
    for (let k = 0; k < steps.length; k++) {
        if (isCancelled) break;
        renderPixels(steps[k], width, height);
        await new Promise(resolve => setTimeout(resolve, 15)); // Geschwindigkeit pro Frame
    }
}

// help function for drawing pixel array to canvas
function renderPixels(pixelArray, width, height) {
    const newImgData = targetCtx.createImageData(width, height);
    for (let i = 0; i < pixelArray.length; i++) {
        newImgData.data[i] = pixelArray[i];
    }
    targetCtx.putImageData(newImgData, 0, 0);
}