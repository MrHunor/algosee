/*
 * Copyright (C) 2026 MrHunor, siryanni
 * LICENSE: GNU General Public License v3 (GPLv3)
 */

document.addEventListener('DOMContentLoaded', () => {
    // 1.URL-parameter
    const urlParams = new URLSearchParams(window.location.search);
    const algoKey = urlParams.get('algo') || 'bfs';
    
    // 1.2. Backend-URL over central config
    const backendBase = getBackendBase();

        // frontend key -> backend algo name (see src/defs/implemented.h)
    const apiAlgoNames = {
        bfs: 'BFS',
        dfs: 'DFS',
        bibfs: 'BIBFS',
        dij: 'dijkstra',
        astar: 'ASTAR',
        greedy: 'GBFS',
        bf: 'bellman-ford'
    };

    // tile values sent to the backend (matches the tool buttons):
    // -1 = wall, 1 = default, 5 = hard. 0 is NOT allowed for A*.
    // Unweighted algos get the same map, backend treats -1 as blocked.
    const TILE_WALL = -1;
    const TILE_DEFAULT = 1;
    const TILE_HARD = 5;

    // 2. title adjust 
    const algoNames = {
        bfs: "Breadth First Search",
        dfs: "Depth First Search",
        dij: "Dijkstra",
        astar: "A*",
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
    const messageBox = document.getElementById('path-message');

    function showMessage(text) {
        if (!messageBox) return;
        messageBox.innerText = text;
        messageBox.hidden = false;
    }

    function hideMessage() {
        if (messageBox) messageBox.hidden = true;
    }

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
    let dragging = null; // 'start', 'goal' or null
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

                    // grab start/goal instead of drawing (not while an algorithm is running)
                    if (startBtn && !startBtn.disabled) {
                        if (r === startPos.r && c === startPos.c) { dragging = 'start'; clearVisualization(); return; }
                        if (r === goalPos.r && c === goalPos.c) { dragging = 'goal'; clearVisualization(); return; }
                    }
                    handleCellInteraction(r, c);
                });

                cell.addEventListener('mouseover', () => {
                    if (!isMouseDown) return;
                    if (dragging) moveMarker(dragging, r, c);
                    else handleCellInteraction(r, c);
                });

                container.appendChild(cell);
                currentRow.push(TILE_DEFAULT);
            }
            grid.push(currentRow);
        }
    }

        document.addEventListener('mouseup', () => {
        isMouseDown = false;
        dragging = null;
    });

    // moves the start or goal marker to another tile
    function moveMarker(type, r, c) {
        const pos = (type === 'start') ? startPos : goalPos;
        const other = (type === 'start') ? goalPos : startPos;

        if (r === pos.r && c === pos.c) return;       // already there
        if (r === other.r && c === other.c) return;   // other marker is on this tile

        const oldCell = document.querySelector(`[data-row='${pos.r}'][data-col='${pos.c}']`);
        const newCell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
        if (!oldCell || !newCell) return;

        oldCell.classList.remove(type);
        newCell.classList.remove('wall', 'cost-5', 'visited', 'path');
        newCell.classList.add(type);

        grid[r][c] = TILE_DEFAULT; // start and goal tiles are always passable
        pos.r = r;
        pos.c = c;
    }

    function handleCellInteraction(r, c) {
        if ((r === startPos.r && c === startPos.c) || (r === goalPos.r && c === goalPos.c)) {
            return;
        }

        const cell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
        if (!cell) return;

        cell.classList.remove('wall', 'cost-5');

        if (currentTool === 'wall') {
        cell.classList.add('wall');
        grid[r][c] = TILE_WALL;
        } 
        else if (currentTool === 'cost-1') {
        grid[r][c] = TILE_DEFAULT;
        } 
        else if (currentTool === 'cost-5') {
        cell.classList.add('cost-5');
        grid[r][c] = TILE_HARD;
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
              const apiAlgo = apiAlgoNames[algoKey] || algoKey.toUpperCase();

            const backendUrl = `${backendBase}/pathalgo?algo=${apiAlgo}`;

            startBtn.disabled = true;
            if (resetBtn) resetBtn.disabled = true;
            if (speedSlider) speedSlider.disabled = true;
            if (stopBtn) stopBtn.disabled = false;
            isCancelled = false;
            clearVisualization();
            hideMessage();
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

                // read response as text since backend sends JSON-string
                const responseText = await response.text();

                if (!response.ok) {
                let msg = responseText;
                try { const parsed = JSON.parse(responseText); if (typeof parsed === 'string') msg = parsed; } catch (_) {}
                throw new Error(msg || `Server-Fehler: ${response.status}`);
                }

                const data = JSON.parse(responseText);

                if (data.TIME !== undefined) {
                    const timeMs = (data.TIME / 1_000_000).toFixed(2);
                    console.log(`Pathfinding time: ${timeMs} ms`);
                }

                if (data.PATH) {
                    await visualizeExploration(data);
                    await visualizePath(data.PATH);
                }

            } catch (error) {
                console.error("Connection-Error:", error);
                showMessage(error instanceof TypeError ? 'Server not reachable. Is the backend running?' : error.message);
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

        // removes colours of the previous run (walls, start and goal stay)
    function clearVisualization() {
        document.querySelectorAll('.grid-cell.visited, .grid-cell.path')
            .forEach(cell => cell.classList.remove('visited', 'path'));
    }

    // Backend 2D arrays are sometimes [row][col] (SEARCHMAP of BFS/DFS/BiBFS) and sometimes
    // transposed [col][row] (DISTANCE, SEARCHMAP of Bellman-Ford).
    // The grid is not square (ROWS != COLS), so the array length tells them apart.
    function getExploredCells(data) {
        const source = (data.SEARCHMAP && data.SEARCHMAP.length) ? data.SEARCHMAP : data.DISTANCE;
        if (!source || !source.length) return [];

        const transposed = source.length === COLS; // [col][row]
        const cells = [];
        for (let a = 0; a < source.length; a++) {
            for (let b = 0; b < source[a].length; b++) {
                const value = source[a][b];
                                // unexplored = -1 (BFS/DFS/BiBFS), INT_MAX = 2147483647 (main backend)
                // or uint64 max ~1.8e19 (development backend)
                if (value < 0 || value >= 2147483647) continue;
                cells.push({ r: transposed ? b : a, c: transposed ? a : b, order: value });
            }
        }
        cells.sort((x, y) => x.order - y.order);
        return cells;
    }

    // colours explored tiles one by one (before the path is drawn)
    async function visualizeExploration(data) {
        const cells = getExploredCells(data);
        const delay = Math.max(2, animationSpeed / 10);

        for (let k = 0; k < cells.length; k++) {
            if (isCancelled) return;

            const { r, c } = cells[k];
            const cell = document.querySelector(`[data-row='${r}'][data-col='${c}']`);
            if (cell && !cell.classList.contains('start') && !cell.classList.contains('goal')) {
                cell.classList.add('visited');
                if (k % 5 === 0) playTone(200 + k);
            }

            await new Promise(resolve => setTimeout(resolve, delay));
        }
    }
});