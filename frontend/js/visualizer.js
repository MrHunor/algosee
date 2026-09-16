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
        sizeSlider.max = 50; // default max size
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

        arraySize = val;
        sizeValSpan.innerText = val;
        generateArray();
    });
}
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

// run asap 
generateArray();
resetBtn.addEventListener('click', generateArray);

// start-button 
startBtn.addEventListener('click', async () => {
    // double check
    if (algoKey === 'bogo' && array.length > 10) {
        alert("BogoSort ist auf maximal 10 Elemente beschränkt!");
        return;
    }

    const backendUrl = `https://algosee.onrender.com/sortalgo?algo=${algoKey}`;

    startBtn.disabled = true;
    resetBtn.disabled = true;
    if (sizeSlider) sizeSlider.disabled = true;

    try {
        const response = await fetch(backendUrl, {
            method: 'POST',
            headers: {
                'Content-Type': 'application/json'
            },
            body: JSON.stringify({ values: array })
        });

        if (!response.ok) {
            throw new Error('Error while communication with Backend');
        }

        const data = await response.json();
        
        //MOVE array extraction (new server structure)

        const moves = data.MOVES;
        
        if (!Array.isArray(moves)) {
            throw new Error('Invalid response structure: "MOVES" array missing');
        }

        // future: show time it took 
        console.log(`Sorting finished in ${data.TIME}ms (backend version: ${data.VERSION})`);

        await visualizeMoves(moves);

    } catch (error) {
        console.error("Connection-Error:", error);
        alert("Sorry, this algorithm isn't quite implemented yet");
    } finally {
        startBtn.disabled = false;
        resetBtn.disabled = false;
        if (sizeSlider) sizeSlider.disabled = false;
    }
});

// animation of moves
async function visualizeMoves(moves) {
    const bars = container.children;

    for (let k = 0; k < moves.length; k++) {
        const [i, j] = moves[k];

        // sec-check incase of indizes out of array
        if (!bars[i] || !bars[j]) continue;

        bars[i].style.backgroundColor = '#ef4444';
        bars[j].style.backgroundColor = '#ef4444';

        let tempHeight = bars[i].style.height;
        let tempText = bars[i].innerText;
        let tempValue = array[i];

        bars[i].style.height = bars[j].style.height;
        bars[i].innerText = bars[j].innerText;

        bars[j].style.height = tempHeight;
        bars[j].innerText = tempText;

        array[i] = array[j];
        array[j] = tempValue;

        await new Promise(resolve => setTimeout(resolve, 200));

        bars[i].style.backgroundColor = '#3b82f6';
        bars[j].style.backgroundColor = '#3b82f6';
    }

    for (let bar of bars) {
        bar.style.backgroundColor = '#22c55e';
    }
}