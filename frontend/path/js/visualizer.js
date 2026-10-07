/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE: GNU General Public License v3 (GPLv3)
 */

document.addEventListener('DOMContentLoaded', () => {
    // 1.URL-parameter
    const urlParams = new URLSearchParams(window.location.search);
    const algoKey = urlParams.get('algo') || 'bfs';
    //1.2.Backend-URL parameters
    const serverParam = urlParams.get('server') || 'remote';
    const backendBase = serverParam === 'local' 
        ? 'http://localhost:8080' // depents which port *you* choose, change accordingly
        : 'https://algosee.onrender.com';

    // 2. title adjust 
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

    const algoTitleElem = document.getElementById('algo-title');
    if (algoTitleElem) {
        algoTitleElem.innerText = algoNames[algoKey] || "Visualization";
    }

    const container = document.getElementById('path-grid');
    const startBtn = document.getElementById('start-btn');
    const resetBtn = document.getElementById('reset-btn');
    const speedSlider = document.getElementById('speed-slider');
    const speedValSpan = document.getElementById('speed-val');
    const stopBtn = document.getElementById('stop-btn');

    let animationSpeed = 200;
    let isCancelled = false;
    let currentTool = 'wall'; // 'wall', 'cost-1', 'cost-5'

    // 3.tool selct listener
    document.querySelectorAll('.tool-btn').forEach(btn => {
        btn.addEventListener('click', (e) => {
            document.querySelectorAll('.tool-btn').forEach(b => b.classList.remove('active'));
            e.currentTarget.classList.add('active');
            currentTool = e.currentTarget.dataset.tool;
        });
    });

    // 4. web audio api
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

    // 5. speed listener
    if (speedSlider) {
        speedSlider.addEventListener('input', (e) => {
            animationSpeed = parseInt(e.target.value);
            if (speedValSpan) speedValSpan.innerText = animationSpeed;
        });
    }

    // 6. stop listener
    if (stopBtn) {
        stopBtn.addEventListener('click', () => {
            isCancelled = true;
        });
    }

    // 7.grid config and generation 
    const ROWS = 15;
    const COLS = 25;
    let grid = [];
    let startPos = { r: 7, c: 4 };
    let goalPos = { r: 7, c: 20 };
    let isMouseDown = false;

    function generateGrid() {
        if (!container) return;
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

                if (r === startPos.r && c === startPos.c) {
                    cell.classList.add('start');
                } else if (r === goalPos.r && c === goalPos.c) {
                    cell.classList.add('goal');
                }

                cell.addEventListener('mousedown', () => {
                    isMouseDown = true;
                    handleCellInteraction(r, c);
                });

                cell.addEventListener('mouseover', () => {
                    if (isMouseDown) handleCellInteraction(r, c);
                });

                container.appendChild(cell);
                currentRow.push(1); // base-value 1 
            }
            grid.push(currentRow);
        }
    }

    document.addEventListener('mouseup', () => {
        isMouseDown = false;
    });

    function handleCellInteraction(r, c) {
        if ((r === startPos.r && c === startPos.c) || (r === goalPos.r && c === goalPos.c)) {
            return;
        }

        const cell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
        if (!cell) return;

        cell.classList.remove('wall', 'cost-5');

        if (currentTool === 'wall') {
            cell.classList.add('wall');
            grid[r][c] = -1;
        } else if (currentTool === 'cost-1') {
            grid[r][c] = 1;
        } else if (currentTool === 'cost-5') {
            cell.classList.add('cost-5');
            grid[r][c] = 5;
        }
    }

    if (resetBtn) {
        resetBtn.addEventListener('click', generateGrid);
    }

    // instantly generate grid
    generateGrid();

   // 8. start button handler
    if (startBtn) {
        startBtn.addEventListener('click', async () => {
            let apiAlgo = algoKey.toUpperCase();
            if (algoKey === 'dij' || algoKey === 'dijkstra') apiAlgo = 'dijkstra';

            const backendUrl = `${backendBase}/pathalgo?algo=${apiAlgo}`;

            startBtn.disabled = true;
            if (resetBtn) resetBtn.disabled = true;
            if (speedSlider) speedSlider.disabled = true;
            if (stopBtn) stopBtn.disabled = false;
            isCancelled = false;

            const payload = {
                MAP: grid,
                START: [startPos.c, startPos.r],
                GOAL: [goalPos.c, goalPos.r]
            };

            try {
                const response = await fetch(backendUrl, {
                    method: 'POST',
                    headers: { 'Content-Type': 'application/json' },
                    body: JSON.stringify(payload)
                });

                const data = await response.json().catch(() => null);

                if (!response.ok) {
                    // uses message from backend or http status
                    const serverMessage = data?.error || data?.message || `Server-Fehler: ${response.status} ${response.statusText}`;
                    throw new Error(serverMessage);
                }

                if (data.TIME !== undefined) {
                    const timeMs = (data.TIME / 1_000_000).toFixed(2);
                    console.log(`Pathfinding time: ${timeMs} ms`);
                }

                if (data.PATH) {
                    await visualizePath(data.PATH);
                }

            } catch (error) {
                console.error("Connection-Error:", error);
                // dynamic error message (network or backend )
                alert(error.message || "Fehler bei der Verbindung zum Backend.");
            } finally {
                startBtn.disabled = false;
                if (resetBtn) resetBtn.disabled = false;
                if (speedSlider) speedSlider.disabled = false;
                if (stopBtn) stopBtn.disabled = true;
            }
        });
    }

    async function playSuccessPath(path) {
        for (let k = 0; k < path.length; k++) {
            if (isCancelled) return;

            const [c, r] = path[k];
            const cell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
            
            if (cell && !cell.classList.contains('start') && !cell.classList.contains('goal')) {
                cell.classList.add('path');
                playTone(300 + (k * 15));
            }

            await new Promise(resolve => setTimeout(resolve, Math.max(20, animationSpeed / 2)));
        }
    }

    async function visualizePath(path) {
        await playSuccessPath(path);
    }
});