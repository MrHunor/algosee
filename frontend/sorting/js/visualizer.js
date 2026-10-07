/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

// read URL-parameters (choose backend function+information displayed)
const urlParams = new URLSearchParams(window.location.search);
const algoKey = urlParams.get('algo') || 'selection';

// adjust title
const algoNames = {
    bogo: "BogoSort",
    miracle: "MiracleSort",
    selection: "SelectionSort",
    merge: "MergeSort",
    bubble: "BubbleSort",
    counting: "counting"
};
document.getElementById('algo-title').innerText = algoNames[algoKey] || "Visualization";

const container = document.getElementById('array-container');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');
const sizeSlider = document.getElementById('array-size-slider');
const sizeValSpan = document.getElementById('size-val');

// speed&stop logic
const speedSlider = document.getElementById('speed-slider');
const speedValSpan = document.getElementById('speed-val');
const stopBtn = document.getElementById('stop-btn');

let animationSpeed = 200;
let isCancelled = false;

//---------------------------------------
//webaudio api 
let audioCtx = null;

function playTone(frequency) {
    if (!audioCtx) {
        audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    }

    const osc = audioCtx.createOscillator();
    const gainNode = audioCtx.createGain();

    osc.type = 'sine'; // sinussound
    osc.frequency.setValueAtTime(frequency, audioCtx.currentTime);

    // regulate noise: soften after 80ms
    gainNode.gain.setValueAtTime(0.1, audioCtx.currentTime);
    gainNode.gain.exponentialRampToValueAtTime(0.0001, audioCtx.currentTime + 0.08);

    osc.connect(gainNode);
    gainNode.connect(audioCtx.destination);

    osc.start();
    osc.stop(audioCtx.currentTime + 0.08);
}
// --------------------------------------------------------

// speed listener
if (speedSlider) {
    speedSlider.addEventListener('input', (e) => {
        animationSpeed = parseInt(e.target.value);
        speedValSpan.innerText = animationSpeed;
    });
}

// stop listener
stopBtn.addEventListener('click', () => {
    isCancelled = true;
});

let array = [];
let arraySize = 15;

// bogo-limiter
if (algoKey === 'bogo') {
    arraySize = 10;
    if (sizeSlider) {
        sizeSlider.max = 10;
        sizeSlider.value = 10;
    }
} else {
    if (sizeSlider) {
        sizeSlider.max = 140; // default max size
        sizeSlider.value = 15;
    }
}

if (sizeValSpan) {
    sizeValSpan.innerText = sizeSlider ? sizeSlider.value : arraySize;
}

// slider listener
if (sizeSlider) {
    sizeSlider.addEventListener('input', (e) => {
        let val = parseInt(e.target.value);
        
        //security check (incase of devtool mani)
        if (algoKey === 'bogo' && val > 10) {
            val = 10;
            sizeSlider.value = 10;
        }

        arraySize = val;w
        sizeValSpan.innerText = val;
        generateArray();
    });
}

// init an array asap 
generateArray();

// -------------------------------------------------------------
// generate random array and draw bars
function generateArray() {
    container.innerHTML = '';
    array = [];
    
    // dynamic width too correctly fit up to 50b
    const gap = arraySize > 25 ? 4 : 8;
    container.style.gap = `${gap}px`;

    for (let i = 0; i < arraySize; i++) {
        const value = Math.floor(Math.random() * 90) + 10;
        array.push(value);

        const bar = document.createElement('div');
        bar.classList.add('array-bar');
        
        // overflow prot
        const calculatedWidth = Math.max(8, Math.floor(700 / arraySize) - gap);
        bar.style.width = `${calculatedWidth}px`;
        bar.style.height = `${value * 3}px`;
        
        // hide size-text and 20 ar
        if (arraySize <= 20) {
            bar.innerText = value;
        } else {
            bar.style.fontSize = '0.6rem';
            bar.paddingBottom = '2px';
        }

        container.appendChild(bar);
    }
}

//new array listener
resetBtn.addEventListener('click', generateArray);

//--------------------------------------

//Start-button (!!!)
//run asap 
startBtn.addEventListener('click', async () => {
    if (algoKey === 'bogo' && array.length > 10) {
        alert("Bogo is capped at 10 Arrays (our server's boutta blow)");
        return;
    }

    const backendUrl = `https://algosee.onrender.com/sortalgo?algo=${algoKey}`;

    startBtn.disabled = true;
    resetBtn.disabled = true;
    if (sizeSlider) sizeSlider.disabled = true;
    if (speedSlider) speedSlider.disabled = true;
    stopBtn.disabled = false; // accses stop button
    isCancelled = false;      // reset cancel flag

    try {
        const response = await fetch(backendUrl, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({ values: array })
        });

        if (!response.ok) throw new Error('Error while communication with Backend');

        const data = await response.json();

        if (data.TIME !== undefined) {
            const timeMs = (data.TIME / 1_000_000).toFixed(2);
            document.getElementById('time-val').innerText = timeMs;
        }

        if (algoKey === 'bogo') {
            const steps = data.TRIES.length;
            const dynamicSpeed = Math.max(5, Math.floor((animationSpeed * 20) / steps));
            await visualizeTries(data.TRIES, dynamicSpeed);
        } else if (algoKey === 'merge') {
            const steps = data["MERGED (LEFT,RIGHT,RESULT)"].length;
            const dynamicSpeed = Math.max(5, Math.floor((animationSpeed * 50) / steps));
            await visualizeMerge(data["MERGED (LEFT,RIGHT,RESULT)"], dynamicSpeed);
        } else if (algoKey === 'counting') {
            const steps = data.SORTED.length;
            const dynamicSpeed = Math.max(5, Math.floor((animationSpeed * 10) / steps));
            await visualizeCounting(data.SORTED, dynamicSpeed);
        } else {
            const steps = data.MOVES.length;
            const dynamicSpeed = steps > 0 ? Math.max(5, Math.floor((animationSpeed * 20) / steps)) : animationSpeed;
            await visualizeMoves(data.MOVES, dynamicSpeed);
        }

    } catch (error) {
        console.error("Connection-Error:", error);
        alert("Error connecting to backend.");
    } finally {
        // unlock UI (this'll clean up)
        startBtn.disabled = false;
        resetBtn.disabled = false;
        if (sizeSlider) sizeSlider.disabled = false;
        if (speedSlider) speedSlider.disabled = false;
        stopBtn.disabled = true;
    }
});

// wave-green function 
async function playSuccessWave() {
    const bars = container.children;

    for (let i = 0; i < bars.length; i++) {
        if (isCancelled) return;

        // turn single bar green
        bars[i].style.backgroundColor = '#22c55e';
        
        //audio wave
        playTone(200 + (i * 40));  //tone

        // wait (small time)
        await new Promise(resolve => setTimeout(resolve, Math.max(20, animationSpeed / 3)));
    }
}

// animation of MOVES
async function visualizeMoves(moves, speed) {
    const bars = container.children;

    for (let k = 0; k < moves.length; k++) {
        // cancel check via stop button
        if (isCancelled) {
            generateArray(); 
            return;
        }

        const [i, j] = moves[k];

        // sec-check incase of indizes out of array
        if (!bars[i] || !bars[j]) continue;

        bars[i].style.backgroundColor = '#ef4444';
        bars[j].style.backgroundColor = '#ef4444';

        //play sinus for every sort 
        playTone(150 + (array[i] * 6));  //tone
        
        let tempHeight = bars[i].style.height;
        let tempText = bars[i].innerText;
        let tempValue = array[i];

        bars[i].style.height = bars[j].style.height;
        bars[i].innerText = bars[j].innerText;

        bars[j].style.height = tempHeight;
        bars[j].innerText = tempText;

        array[i] = array[j];
        array[j] = tempValue;

        // dynamic speed
        await new Promise(resolve => setTimeout(resolve, speed));

        bars[i].style.backgroundColor = '#3b82f6';
        bars[j].style.backgroundColor = '#3b82f6';
    }

    if (!isCancelled) {
        await playSuccessWave();
    }
}

// animation of TRIES (specifically for bogo)
async function visualizeTries(tries, speed) {
    const bars = container.children;

    for (let k = 0; k < tries.length; k++) {
        // again, cancel check via stop button
        if (isCancelled) {
            generateArray(); 
            return;
        }
        const currentTry = tries[k];

        for (let i = 0; i < currentTry.length; i++) {
            if (!bars[i]) continue;

            const val = currentTry[i];
            bars[i].style.height = `${val * 3}px`;
            
            if (arraySize <= 20) {
                bars[i].innerText = val;
            }
            
            bars[i].style.backgroundColor = '#3b82f6';
        }
           
        if (currentTry.length > 0) {
            playTone(150 + (currentTry[0] * 6)); //tone
        }
    
        array = [...currentTry];

        await new Promise(resolve => setTimeout(resolve, speed));
    }
        
    // green marking (only if not cancelled)
    if (!isCancelled) {
        await playSuccessWave();
    }
}

//MergeSort animation
async function visualizeMerge(mergeSteps, speed) {
    const bars = container.children;

    for (let k = 0; k < mergeSteps.length; k++) {
        if (isCancelled) {
            generateArray();
            return;
        }

        // mergeSteps[k] includes [left, right, result]
        const result = mergeSteps[k][2];

        for (let i = 0; i < result.length; i++) {
            if (!bars[i]) continue;

            const val = result[i];
            bars[i].style.height = `${val * 3}px`;
            if (arraySize <= 20) {
                bars[i].innerText = val;
            }
            bars[i].style.backgroundColor = '#3b82f6';
        }

        if (result.length > 0) {
            playTone(150 + (result[0] * 6)); //tone
        }

        array = [...result];
        await new Promise(resolve => setTimeout(resolve, speed));
    }

    if (!isCancelled) {
        await playSuccessWave();
    }
}

// CountingSort Animation
async function visualizeCounting(sortedArray, speed) {
    const bars = container.children;

    for (let i = 0; i < sortedArray.length; i++) {
        if (isCancelled) {
            generateArray();
            return;
        }

        if (!bars[i]) continue;

        const val = sortedArray[i];
        
        // mark Bar red
        bars[i].style.backgroundColor = '#ef4444';
        await new Promise(resolve => setTimeout(resolve, speed / 2));

        // height and numb
        bars[i].style.height = `${val * 3}px`;
        if (arraySize <= 20) {
            bars[i].innerText = val;
        }
        array[i] = val;

        playTone(150 + (val * 6));

        // set green
        bars[i].style.backgroundColor = '#22c55e';
        await new Promise(resolve => setTimeout(resolve, speed / 2));
    }

    if (!isCancelled) {
        await playSuccessWave();
    }
}

//almost 400ll lol XD