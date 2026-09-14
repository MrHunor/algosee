// URL-Parameter auslesen (welcher Algorithmus?)
const urlParams = new URLSearchParams(window.location.search);
const algoKey = urlParams.get('algo') || 'selection';

// Titel anpassen
const algoNames = {
    bogo: "BogoSort",
    miracle: "MiracleSort",
    selection: "SelectionSort"
};
document.getElementById('algo-title').innerText = algoNames[algoKey] || "Visualisierung";

const container = document.getElementById('array-container');
const startBtn = document.getElementById('start-btn');
const resetBtn = document.getElementById('reset-btn');

let array = [];
const arraySize = 15;

// Zufälliges Array generieren und Balken zeichnen
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

// Direkt beim Laden ausführen
generateArray();
resetBtn.addEventListener('click', generateArray);

// Start-Button Logik
startBtn.addEventlistener('click', async () => {
    // WICHTIG FÜR CODESPACES: 
    // Entweder läuft der C++ Server lokal oder du musst die Port-Weiterleitung nutzen.
    // Für den Test nutzen wir hier erst mal den direkten Pfad:
    const backendUrl = `http://localhost:8080/sortalgo?algo=${algoKey}`;

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
            throw new Error('Fehler bei der Kommunikation mit dem C++ Server');
        }

        const moves = await response.json();
        await visualizeMoves(moves);

    } catch (error) {
        console.error("Verbindungsfehler:", error);
        alert("Konnte keine Verbindung zum C++-Backend herstellen. Läuft der Server im Codespace und ist Port 8080 freigegeben?");
    } finally {
        startBtn.disabled = false;
        resetBtn.disabled = false;
    }
});

// Animieren der Tausch-Operationen
async function visualizeMoves(moves) {
    const bars = container.children;

    for (let k = 0; k < moves.length; k++) {
        const [i, j] = moves[k];

        bars[i].style.backgroundColor = '#ef4444';
        bars[j].style.backgroundColor = '#ef4444';

        // Höhe und Text zwischenspeichern und tauschen
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
