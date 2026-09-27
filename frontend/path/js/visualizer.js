/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE:GNU General Public License v3 (GPLv3)
 */

// read URL-parameters (choose backend function+information displayed)
const urlParams = new URLSearchParams(window.location.search);
const algoKey = urlParams.get('algo') || 'bfs';

// adjust title
const algoNames = {
    bfs: "Breadth First Search",
    dfs: "Depth First Search",
    dij: "Dijkstra",
    bibfs: "Bidirectional BFS",
    greedy: "Greedy Best-First-Search",
    jps: "Jump Point Search",
    ds: "D*",
    bf: "Bellman-Ford",
    fw: "Floyd-Warshall"
};
document.getElementById('algo-title').innerText = algoNames[algoKey] || "Visualization";

const container = document.getElementById('path-grid');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');

// speed & stop logic
const speedSlider = document.getElementById('speed-slider');
const speedValSpan = document.getElementById('speed-val');
const stopBtn = document.getElementById('stop-btn');

let animationSpeed = 200;
let isCancelled = false;

//---------------------------------------
// web audio api 
let audioCtx = null;

function playTone(frequency) {
    if (!audioCtx) {
        audioCtx = new (window.AudioContext || window.webkitAudioContext)();
    }

    const osc = audioCtx.createOscillator();
    const gainNode = audioCtx.createGain();

    osc.type = 'sine';
    osc.frequency.setValueAtTime(frequency, audioCtx.currentTime);

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
        if (speedValSpan) speedValSpan.innerText = animationSpeed;
    });
}

// stop listener
stopBtn.addEventListener('click', () => {
    isCancelled = true;
});

// Grid configurations
const ROWS = 15;
const COLS = 25;
let grid = []; // 0 = empty, 1 = wall
let startPos = { r: 7, c: 4 };   // Standard-Startpunkt
let goalPos = { r: 7, c: 20 };  // Standard-Zielpunkt

let isMouseDown = false;
let mouseMode = 'wall'; // 'wall', 'start', 'goal'

// generate grid structure
function generateGrid() {
    container.innerHTML = '';
    grid = [];
    
    container.style.gridTemplateColumns = `repeat(${COLS}, 1fr)`;

    for (let r = 0; r < ROWS; r++) {
        let currentRow = [];
        for (let c = 0; c < COLS; c++) {
            const cell = document.createElement('div');
            cell.classList.add('grid-cell');
            cell.dataset.row = r;
            cell.dataset.col = c;

            // Set standard start and goal positions visually & in matrix
            if (r === startPos.r && c === startPos.c) {
                cell.classList.add('start');
            } else if (r === goalPos.r && c === goalPos.c) {
                cell.classList.add('goal');
            }

            // Mouse event listeners for drawing walls
            cell.addEventListener('mousedown', (e) => {
                isMouseDown = true;
                handleCellInteraction(r, c);
            });

            cell.addEventListener('mouseover', () => {
                if (isMouseDown) handleCellInteraction(r, c);
            });

            container.appendChild(cell);
            currentRow.push(0);
        }
        grid.push(currentRow);
    }
}

document.addEventListener('mouseup', () => {
    isMouseDown = false;
});

function handleCellInteraction(r, c) {
    // Prevent overwriting start or goal nodes with walls for now
    if ((r === startPos.r && c === startPos.c) || (r === goalPos.r && c === goalPos.c)) {
        return;
    }

    const cell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
    if (!cell) return;

    cell.classList.toggle('wall');
    grid[r][c] = cell.classList.contains('wall') ? 1 : 0;
}

// new grid listener
resetBtn.addEventListener('click', generateGrid);

// Initial call to draw the grid on load
generateGrid();

//--------------------------------------

// Start-button 
startBtn.addEventListener('click', async () => {
    // Backend expects uppercase or matching exact string depending on backend routing (e.g. "BFS" or "dijkstra")
    // Euer C++ Code prüft: algo == "BFS" || algo == "DFS" || algo == "dijkstra"
    let apiAlgo = algoKey.toUpperCase();
    if (algoKey === 'dijkstra') apiAlgo = 'dijkstra';

    const backendUrl = `https://algosee.onrender.com/pathalgo?algo=${apiAlgo}`;

    startBtn.disabled = true;
    resetBtn.disabled = true;
    if (speedSlider) speedSlider.disabled = true;
    stopBtn.disabled = false;
    isCancelled = false;

    const payload = {
        MAP: grid,
        START: [startPos.c, startPos.r], // [X, Y] bzw [Spalte, Zeile] analog zum C++ Backend Check
        GOAL: [goalPos.c, goalPos.r]
    };

    try {
        const response = await fetch(backendUrl, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify(payload)
        });

        if (!response.ok) throw new Error('Error while communicating with Backend');

        const data = await response.json();

        if (data.TIME !== undefined) {
            const timeMs = (data.TIME / 1_000_000).toFixed(2);
            // Falls ihr ein Element für die Zeit habt, könnt ihr das hier füllen:
            // document.getElementById('time-val').innerText = timeMs;
            console.log(`Pathfinding time: ${timeMs} ms`);
        }

        if (data.PATH) {
            await visualizePath(data.PATH);
        }

    } catch (error) {
        console.error("Connection-Error:", error);
        alert("Error connecting to backend or no valid path found.");
    } finally {
        startBtn.disabled = false;
        resetBtn.disabled = false;
        if (speedSlider) speedSlider.disabled = false;
        stopBtn.disabled = true;
    }
});

// success wave or final path drawing function
async function playSuccessPath(path) {
    for (let k = 0; k < path.length; k++) {
        if (isCancelled) return;

        const [c, r] = path[k]; // Koordinaten aus dem Backend [X, Y]
        const cell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
        
        if (cell && !cell.classList.contains('start') && !cell.classList.contains('goal')) {
            cell.classList.add('path');
            playTone(300 + (k * 15));
        }

        await new Promise(resolve => setTimeout(resolve, Math.max(20, animationSpeed / 2)));
    }
}

// animation of PATH
async function visualizePath(path) {
    // Hier können vorab gefundene "visited" Knoten animiert werden, 
    // falls das Backend diese mitschickt. Andernfalls direkt den finalen Pfad zeichnen:
    await playSuccessPath(path);
}