/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
*/

const urlParams = new URLSearchParams(window.location.search);
const algoKey1 = urlParams.get('algo1'); // no fallback
const algoKey2 = urlParams.get('algo2'); // no fallback

const algoNames = {
    bogo: "BogoSort",
    miracle: "MiracleSort",
    selection: "SelectionSort",
    merge: "MergeSort",
    bubble: "BubbleSort",
    counting: "CountingSort"
};

// if no algo, show placeholder
const name1 = algoNames[algoKey1] || "no algo selected";
const name2 = algoNames[algoKey2] || "no algo selected";

document.getElementById('algo1-title').innerText = name1;
document.getElementById('algo2-title').innerText = name2;
document.getElementById('algo1-name-label').innerText = name1;
document.getElementById('algo2-name-label').innerText = name2;

const container1 = document.getElementById('array-container-1');
const container2 = document.getElementById('array-container-2');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');
const sizeSlider = document.getElementById('array-size-slider');
const sizeValSpan = document.getElementById('size-val');
const speedSlider = document.getElementById('speed-slider');
const speedValSpan = document.getElementById('speed-val');
const stopBtn = document.getElementById('stop-btn');

let animationSpeed = 200;
let isCancelled = false;
let baseArray = [];
let arraySize = 15;

if (sizeValSpan) sizeValSpan.innerText = arraySize;

// lock s-b if no algo
if (!algoKey1 || !algoKey2) {
    startBtn.disabled = true;
    startBtn.innerText = "choose algorithms";
    startBtn.style.opacity = "0.6";
}

// Speed Listener
if (speedSlider) {
    speedSlider.addEventListener('input', (e) => {
        animationSpeed = parseInt(e.target.value);
        speedValSpan.innerText = animationSpeed;
    });
}

// Stop Listener
stopBtn.addEventListener('click', () => {
    isCancelled = true;
});

// Size Listener
if (sizeSlider) {
    sizeSlider.addEventListener('input', (e) => {
        arraySize = parseInt(e.target.value);
        sizeValSpan.innerText = arraySize;
        generateArrays();
    });
}

// generates identical arrays
function generateArrays() {
    container1.innerHTML = '';
    container2.innerHTML = '';
    baseArray = [];

    const gap = arraySize > 20 ? 2 : 6;
    container1.style.gap = `${gap}px`;
    container2.style.gap = `${gap}px`;

    for (let i = 0; i < arraySize; i++) {
        const value = Math.floor(Math.random() * 80) + 10;
        baseArray.push(value);

        const bar1 = document.createElement('div');
        bar1.classList.add('array-bar');
        bar1.style.height = `${value * 3}px`;
        if (arraySize <= 20) bar1.innerText = value;
        container1.appendChild(bar1);

        const bar2 = document.createElement('div');
        bar2.classList.add('array-bar');
        bar2.style.height = `${value * 3}px`;
        if (arraySize <= 20) bar2.innerText = value;
        container2.appendChild(bar2);
    }
}

resetBtn.addEventListener('click', generateArrays);
generateArrays();

// startbutton logic (send both req parallel)
startBtn.addEventListener('click', async () => {
    startBtn.disabled = true;
    resetBtn.disabled = true;
    if (sizeSlider) sizeSlider.disabled = true;
    if (speedSlider) speedSlider.disabled = true;
    stopBtn.disabled = false;
    isCancelled = false;

    document.getElementById('time-val-1').innerText = '-';
    document.getElementById('time-val-2').innerText = '-';

    try {
        // Parallel die Daten von eurem Backend holen
        const [res1, res2] = await Promise.all([
            fetch(`https://algosee.onrender.com/sortalgo?algo=${algoKey1}`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ values: [...baseArray] })
            }),
            fetch(`https://algosee.onrender.com/sortalgo?algo=${algoKey2}`, {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify({ values: [...baseArray] })
            })
        ]);

        if (!res1.ok || !res2.ok) throw new Error('Error with backend communication.');

        const data1 = await res1.json();
        const data2 = await res2.json();

        if (data1.TIME !== undefined) {
            document.getElementById('time-val-1').innerText = (data1.TIME / 1_000_000).toFixed(2);
        }
        if (data2.TIME !== undefined) {
            document.getElementById('time-val-2').innerText = (data2.TIME / 1_000_000).toFixed(2);
        }

        // start both vis asyc
        await Promise.all([
            visualizeAlgo(container1, data1, algoKey1),
            visualizeAlgo(container2, data2, algoKey2)
        ]);

    } catch (error) {
        console.error("Verbindungsfehler:", error);
        alert("Fehler beim Verbinden mit dem Backend.");
    } finally {
        startBtn.disabled = false;
        resetBtn.disabled = false;
        if (sizeSlider) sizeSlider.disabled = false;
        if (speedSlider) speedSlider.disabled = false;
        stopBtn.disabled = true;
    }
});

async function visualizeAlgo(containerElement, data, key) {
    const bars = containerElement.children;
    let moves = [];

    if (key === 'bogo') moves = data.TRIES || [];
    else if (key === 'merge') moves = data["MERGED (LEFT,RIGHT,RESULT)"] || [];
    else if (key === 'counting') moves = data.SORTED || [];
    else moves = data.MOVES || [];

    const dynamicSpeed = Math.max(2, Math.floor((animationSpeed * 15) / Math.max(moves.length, 1)));

    for (let k = 0; k < moves.length; k++) {
        if (isCancelled) return;

        if (key === 'merge') {
            const result = moves[k][2];
            for (let i = 0; i < result.length; i++) {
                if (!bars[i]) continue;
                bars[i].style.height = `${result[i] * 3}px`;
                if (bars.length <= 20) bars[i].innerText = result[i];
            }
        } else if (key === 'bogo' || key === 'counting') {
            const currentArr = moves[k];
            if (Array.isArray(currentArr)) {
                for (let i = 0; i < currentArr.length; i++) {
                    if (!bars[i]) continue;
                    bars[i].style.height = `${currentArr[i] * 3}px`;
                    if (bars.length <= 20) bars[i].innerText = currentArr[i];
                }
            }
        } else {
            // Standard Moves [i, j] Swap
            const [i, j] = moves[k];
            if (bars[i] && bars[j]) {
                bars[i].style.backgroundColor = '#ef4444';
                bars[j].style.backgroundColor = '#ef4444';

                let tempHeight = bars[i].style.height;
                let tempText = bars[i].innerText;

                bars[i].style.height = bars[j].style.height;
                bars[i].innerText = bars[j].innerText;
                bars[j].style.height = tempHeight;
                bars[j].innerText = tempText;

                await new Promise(resolve => setTimeout(resolve, dynamicSpeed));

                bars[i].style.backgroundColor = '#3b82f6';
                bars[j].style.backgroundColor = '#3b82f6';
            }
        }

        await new Promise(resolve => setTimeout(resolve, dynamicSpeed));
    }
}