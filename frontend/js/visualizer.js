// read URL-parameters (choose backend function+information displayed)
const urlParams = new URLSearchParams(window.location.search);
const algoKey = urlParams.get('algo') || 'selection';

// adjust title
const algoNames = {
    bogo: "BogoSort",
    miracle: "MiracleSort",
    selection: "SelectionSort"
};
document.getElementById('algo-title').innerText = algoNames[algoKey] || "Visualization";

const container = document.getElementById('array-container');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');

let array = [];
const arraySize = 15;

// generate random array and draw bars
function generateArray() {
    container.innerHTML = '';
    array = [];
    for (let i = 0; i < arraySize; i++) {
        const value = Math.floor(Math.random() * 90) + 10;
        array.push(value);

        const bar = document.createElement('div');
        bar.classList.add('array-bar');
        bar.style.height = `${value * 3}px`;
        bar.innerText = value;
        container.appendChild(bar);
    }
}

// run asap 
generateArray();
resetBtn.addEventListener('click', generateArray);

// start-button 
startBtn.addEventListener('click', async () => {
    // FOR CODESPACE: 
    // either c++ server runs local or use port-forwarding (P8080)
    // path for testing:
    const backendUrl = `https://algosee.onrender.com/sortalgo?algo=${algoKey}`;

    startBtn.disabled = true;
    resetBtn.disabled = true;

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

        const moves = await response.json();
        await visualizeMoves(moves);

    } catch (error) {
        console.error("Connection-Error:", error);
        alert("Couldn't connect to C++ Backend. Is Port Port forwarding to Port8080 active?");
    } finally {
        startBtn.disabled = false;
        resetBtn.disabled = false;
    }
});

// animation of moves
async function visualizeMoves(moves) {
    const bars = container.children;

    for (let k = 0; k < moves.length; k++) {
        const [i, j] = moves[k];

        bars[i].style.backgroundColor = '#ef4444';
        bars[j].style.backgroundColor = '#ef4444';

        // save and move position of text
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

    for (let bar of bars) {
        bar.style.backgroundColor = '#22c55e';
    }
